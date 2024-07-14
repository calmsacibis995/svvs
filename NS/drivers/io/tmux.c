#ident "@(#)tmux.c	1.2"
/*
 * Loopback driver
 */

#include "sys/types.h"
#include "sys/sysmacros.h"
#include "sys/param.h"
#include "sys/errno.h"
#include "sys/stropts.h"
#include "sys/stream.h"
#include "tmux.h"

int tmxopen(), tmxclose(), tmxursrv(), tmxuwsrv(), tmxlrsrv(), tmxlwsrv();

static struct module_info tmxm_info = {63, "tmx", 0, 256, 512, 256};

static struct qinit tmxurinit = { NULL, tmxursrv, tmxopen, tmxclose, NULL, &tmxm_info, NULL};
static struct qinit tmxuwinit = { putq, tmxuwsrv, NULL, NULL, NULL, &tmxm_info, NULL};
static struct qinit tmxlrinit = { putq, tmxlrsrv, NULL, NULL, NULL, &tmxm_info, NULL};
static struct qinit tmxlwinit = { NULL, tmxlwsrv, NULL, NULL, NULL, &tmxm_info, NULL};

struct streamtab tmxinfo = { &tmxurinit, &tmxuwinit, &tmxlrinit, &tmxlwinit };

extern struct tmx tmx_tmx[];
extern struct tmxl tmx_low[];
extern tmxcnt;
extern tmxlcnt;

tmxopen(q, dev, flag, sflag)
queue_t *q;
{
	mblk_t *bp;
	struct stroptions *sop;
	struct tmx *tp;

	dev = minor(dev);
	if (sflag == CLONEOPEN) {
		for (dev=0; dev<tmxcnt; dev++) 
			if (!(tmx_tmx[dev].tmx_state & TMXOPEN)) break;
	}
	if (dev < 0 || dev >= tmxcnt) return(OPENFAIL);
	if (!(bp = allocb(sizeof(struct stroptions)))) return(OPENFAIL);
	tp = &tmx_tmx[dev];
	tp->tmx_state = TMXOPEN;
	tp->tmx_rdq = q;
	q->q_ptr = (caddr_t)tp;
	WR(q)->q_ptr = (caddr_t)tp;
	bp->b_datap->db_type = M_SETOPTS;
	bp->b_wptr += sizeof(struct stroptions);
	sop = (struct stroptions *)bp->b_rptr;
	sop->so_flags = SO_LOWAT | SO_HIWAT;
	sop->so_lowat = 256;
	sop->so_hiwat = 512;
	putnext(q, bp);
	return(dev);
}

tmxclose(q)
queue_t *q;
{
	register i;

	((struct tmx *)(q->q_ptr))->tmx_state  = 0;
	((struct tmx *)(q->q_ptr))->tmx_rdq = NULL;
	flushq(WR(q), 1);
	q->q_ptr = NULL;
	for (i=0; i<tmxlcnt; i++) 
		if (tmx_low[i].ctlq == WR(q)) {
			tmx_low[i].muxq = tmx_low[i].ctlq = NULL;
			tmx_low[i].muxid = 0;
		}
}



/*
 * Upper side read service procedure.  No messages are ever placed on 
 * the read queue here, this just back-enables all of the lower side
 * read service procedures.
 */
tmxursrv(q)
register queue_t *q;
{
	register i;
	for (i=0; i<tmxlcnt; i++) 
		if (tmx_low[i].muxq) qenable(RD(tmx_low[i].muxq));	
}



tmxuwsrv(q)
queue_t *q;
{
	register mblk_t *mp;
	queue_t *qp;
	struct iocblk *ioc;
	struct linkblk *lp;
	int i;


	while (mp = getq(q)) {
		switch(mp->b_datap->db_type) {
		case M_FLUSH:
			if (*mp->b_rptr & FLUSHW) {
				*mp->b_rptr &= ~FLUSHW;
				flushq(q, FLUSHALL);
			}
			if (*mp->b_rptr & FLUSHR) qreply(q, mp);
			continue;

		case M_DATA:
		case M_PROTO:
		case M_PCPROTO:
			/*
			 * The first byte of the first message block
			 * is the address of the lower stream to put
			 * the message out on.  
			 */

			i = *mp->b_rptr;
			if ((i > tmxlcnt) || !tmx_low[i].muxq) {
				mp->b_datap->db_type = M_ERROR;
				*mp->b_rptr = EIO;
				qreply(q, mp);
				continue;
			}
			qp = tmx_low[i].muxq->q_next;
			if (!canput(qp) && (mp->b_datap->db_type < QPCTL)) {
				putbq(q, mp);
				return;
			}
			mp->b_rptr++;
			(*qp->q_qinfo->qi_putp)(qp, mp);
			continue;

		case M_IOCTL:
			if ((mp->b_wptr - mp->b_rptr) != sizeof(*ioc)) {
				mp->b_datap->db_type = M_IOCNAK;
				qreply(q, mp);
				continue;
			}
			ioc = (struct iocblk *)mp->b_rptr;
			switch (ioc->ioc_cmd) {
			case I_LINK:
				if ((mp->b_cont->b_wptr - mp->b_cont->b_rptr)
								 != sizeof(*lp)) {
					mp->b_datap->db_type = M_IOCNAK;
					ioc->ioc_count = 0;
					freemsg(mp->b_cont);
					mp->b_cont = NULL;
					qreply(q, mp);
					continue;
				}
				lp = (struct linkblk *)mp->b_cont->b_rptr;
				for (i=0; i<tmxlcnt; i++) 
					if (!tmx_low[i].muxq) break;
				if (i >= tmxlcnt) {
					mp->b_datap->db_type = M_IOCNAK;
					ioc->ioc_count = 0;
					freemsg(mp->b_cont);
					mp->b_cont = NULL;
					qreply(q, mp);
					continue;
				}
				tmx_low[i].muxq = lp->l_qbot;
				tmx_low[i].muxid = lp->l_index;
				tmx_low[i].ctlq = q;
				mp->b_datap->db_type = M_IOCACK;
				ioc->ioc_count = 0;
				freemsg(mp->b_cont);
				mp->b_cont = NULL;
				qreply(q, mp);
				continue;

			case I_UNLINK:
				if ((mp->b_cont->b_wptr - mp->b_cont->b_rptr)
								 != sizeof(*lp)) {
					mp->b_datap->db_type = M_IOCNAK;
					ioc->ioc_count = 0;
					freemsg(mp->b_cont);
					mp->b_cont = NULL;
					qreply(q, mp);
					continue;
				}
				lp = (struct linkblk *)mp->b_cont->b_rptr;
				for (i=0; i<tmxlcnt; i++) 
					if (tmx_low[i].muxq == lp->l_qbot) break;
				if (i >= tmxlcnt) {
					mp->b_datap->db_type = M_IOCNAK;
					ioc->ioc_count = 0;
					freemsg(mp->b_cont);
					mp->b_cont = NULL;
					qreply(q, mp);
					continue;
				}
				tmx_low[i].muxq = NULL;
				tmx_low[i].ctlq = NULL;
				tmx_low[i].muxid = 0;
				mp->b_datap->db_type = M_IOCACK;
				ioc->ioc_rval = i;
				ioc->ioc_count = 0;
				freemsg(mp->b_cont);
				mp->b_cont = NULL;
				qreply(q, mp);
				continue;

			default:
				mp->b_datap->db_type = M_IOCNAK;
				qreply(q, mp);
				continue;
			}

		default:
			freemsg(mp);
			continue;
		}
	}				
}



tmxlrsrv(q)
queue_t *q;
{
	register mblk_t *mp;
	queue_t *qp;
	struct iocblk *ioc;
	struct linkblk *lp;
	int i;

	while (mp = getq(q)) {
		switch(mp->b_datap->db_type) {
		case M_FLUSH:
			if (*mp->b_rptr & FLUSHW) {
				*mp->b_rptr &= ~FLUSHW;
				flushq(q, FLUSHALL);
			}
			if (*mp->b_rptr & FLUSHR) qreply(q, mp);
			continue;

		case M_DATA:
		case M_PROTO:
		case M_PCPROTO:
			/*
			 * The first byte of the first message block
			 * is the address of the upper stream to put
			 * the message out on.  
			 */

			i = *mp->b_rptr;
			if (i > tmxcnt) {
				freemsg(mp);
				continue;
			}
			qp = tmx_tmx[i].tmx_rdq->q_next;
			if (!canput(qp) && (mp->b_datap->db_type < QPCTL)) {
				putbq(q, mp);
				return;
			}
			mp->b_rptr++;
			(*qp->q_qinfo->qi_putp)(qp, mp);
			continue;
		default:
			freemsg(mp);
			continue;			
		}
	}				
}



/*
 * Lower side write service procedure.  No messages are ever placed on
 * the write queue here, this just back-enables all of the upper side
 * write service procedures.
 */
tmxlwsrv(q)
queue_t *q;
{
	register i;
	for (i=0; i<tmxcnt; i++) 
		if (tmx_tmx[i].tmx_rdq) qenable(WR(tmx_tmx[i].tmx_rdq));
}

