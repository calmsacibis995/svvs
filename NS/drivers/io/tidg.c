/*
 * Transport Interface Library connection oriented test driver - issue 1
 */

#include "sys/param.h"
#include "sys/types.h"
#include "sys/sysmacros.h"
#include "sys/stream.h"
#include "sys/stropts.h"
#include "sys/tihdr.h"
#include "tidg.h"
#include "sys/tiuser.h"
#include "sys/strlog.h"
#include "sys/log.h"
#include "sys/debug.h"
#include "sys/signal.h"
#include "sys/fs/s5dir.h"
#include "sys/psw.h"
#include "sys/pcb.h"
#include "sys/user.h"
#include "sys/errno.h"


extern struct ti_tidg ti_tidg[];
extern int tidg_cnt;
extern nulldev();

/* tidg identifcation id (should not be static should be
 * configurable).
 * Also a Interface ID string should be included so modules
 * may not be pushed with unlike interfaces
 */
#define TIDG_ID	2223

#define NR 	127

/* stream data structure definitions */
int tidgopen(), tidgclose(), tidgwput(), tidgrsrv(), tidgwsrv();
static struct module_info tidg_info = {TIDG_ID, "tidg", 0, TIDU_SIZE, 4*TIDU_SIZE, TIDU_SIZE};
static struct qinit tidgrinit = { NULL, tidgrsrv, tidgopen, tidgclose, nulldev, &tidg_info, NULL};
static struct qinit tidgwinit = { tidgwput, tidgwsrv, tidgopen, tidgclose, nulldev, &tidg_info, NULL};
struct streamtab tidinfo = { &tidgrinit, &tidgwinit, NULL, NULL };


extern char ti_statetbl[TE_NOEVENTS][TS_NOSTATES];


struct tidg_addr {
	unsigned long addr;
	int used;
};


struct tidg_addr tidg_addr[MAXADDR] = {
				 {0x00010000, 0},
				 {0x00020000, 0},
				 {0x00030000, 0},
				 {0x00000001, 0},
				 {0x00000002, 0},
				 {0x00000003, 0},
				 {0x00000100, 0},
				 {0x00000200, 0},
				 {0x00000300, 0},
				 {0x01000000, 0},
				 {0x02000000, 0},
				 {0x03000000, 0},
				};


/* macro to change state NEXTSTATE(event, current state) */
#define NEXTSTATE(X,Y) ti_statetbl[X][Y]

/*
 * tidgopen - open routine gets called when the
 *	       driver gets opened.
 */

tidgopen(q, dev, flag, sflag)
register queue_t *q;
{
	register struct ti_tidg *tiptr;

	ASSERT(q != NULL);

	/* is it already opened */
	if (q->q_ptr) {
		STRLOG(TIDG_ID, TI_DG_NUM(q->q_ptr),0, SL_TRACE,
		       "tidgopen: opened already, open succeeded\n");
		return(TI_DG_NUM(q->q_ptr));
	}

	if (sflag == CLONEOPEN) {
		for (tiptr = ti_tidg; tiptr < &ti_tidg[tidg_cnt]; tiptr++)
			if (!(tiptr->ti_flags & USED))
				break;
		if (tiptr > &ti_tidg[tidg_cnt-1]) {
			STRLOG(TIDG_ID, -1,0, SL_TRACE|SL_ERROR,
			       "tidgopen: opened failed: couldn't allocate ti_tidg data structure for q = %x\n",
			       (caddr_t)q);
			u.u_error = ENOSPC;
			return(OPENFAIL);
		}
	} else {
		dev = minor(dev);
		if (dev > (tidg_cnt - 1)) {
			u.u_error = ENODEV;
			return(OPENFAIL);
		}

		tiptr = &ti_tidg[dev];
	}

	ASSERT(tiptr <= &ti_tidg[tidg_cnt-1]);

	/* initialize data structure */
	tiptr->ti_state = TS_UNBND;
	tiptr->ti_flags = USED;
	tiptr->ti_rdq = q;
	tiptr->ti_addr = 0;


	/* assign tiptr to queue private pointers */
	q->q_ptr = (caddr_t)tiptr;
	WR(q)->q_ptr = (caddr_t)tiptr;

	STRLOG(TIDG_ID, TI_DG_NUM(q->q_ptr),0, SL_TRACE,
	       "tidgopen: open succeeded\n");

	return(TI_DG_NUM(tiptr));
}


/*
 * tidgclose - This routine gets called when the module
 *              gets closed.
 */

tidgclose(q)
register queue_t *q;
{
	register struct ti_tidg *tiptr;
	register struct tidg_addr *tmp;

	tiptr = (struct ti_tidg *)q->q_ptr;

	ASSERT(tiptr != NULL);


	if (tiptr->ti_addr)
		for (tmp = tidg_addr; tmp < &tidg_addr[MAXADDR]; tmp++)
			if (tmp->addr == tiptr->ti_addr) {
				tmp->used = 0;
				break;
			}
	tidg_finished(q);

	tiptr->ti_state = TS_UNBND;
	tiptr->ti_flags = 0;
	tiptr->ti_rdq = 0;

	STRLOG(TIDG_ID, TI_DG_NUM(q->q_ptr),0, SL_TRACE,
	       "tidgclose: close succeeded\n");
}




/*
 * tidgwput - Driver write queue put procedure.
 */

tidgwput(q, mp)
register queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tidg *tiptr;
	int sps;

	ASSERT(q != NULL);

	tiptr = (struct ti_tidg *)q->q_ptr;

	ASSERT(tiptr != NULL);

	STRLOG(TIDG_ID, TI_DG_NUM(tiptr),0, SL_TRACE,
	       "tidgwput: STRmsg type %x received mp = %x\n",
	       mp->b_datap->db_type, mp);

	/* switch on message type */
	switch(mp->b_datap->db_type) {

	case M_DATA:
		STRLOG(TIDG_ID, TI_DG_NUM(tiptr) ,0, SL_TRACE|SL_ERROR,
		    "tidgwput: bad STRmsg type = %x\n", mp->b_datap->db_type);
		tidg_sendfatal(q, mp);
		return;
	case M_PROTO:
	case M_PCPROTO:
		ASSERT((mp->b_wptr - mp->b_rptr) >= sizeof(long));

		pptr = (union T_primitives *)mp->b_rptr;

		STRLOG(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE,
		       "tidgwput: TImsg type = %d received\n", pptr->type);

		if ((mp->b_wptr - mp->b_rptr) < sizeof(long)) {
			strlog(TIDG_ID, TI_DG_NUM(tiptr) ,0, SL_TRACE|SL_ERROR,
				"tidgwput: msg wptr < rptr for msg = %x\n", mp);
			tidg_sendfatal(q, mp);
			return;
		}

		sps = splstr();
		/* switch on primitive type */
		switch (pptr->type) {

		case T_INFO_REQ:
			tidg_ireq(q, mp);
			break;

		case T_UNITDATA_REQ:
			if (tiptr->ti_flags & FATAL) {
				freemsg(mp);
				splx(sps);
				return;
			}
			if (NEXTSTATE(TE_UNITDATA_REQ, tiptr->ti_state) == NR) {
				strlog(TIDG_ID, TI_DG_NUM(tiptr) ,0,
				  SL_TRACE|SL_ERROR,
				  "tidgwput: bad state for TE_UNITDATA_REQ in state %d\n",
				  pptr->type, tiptr->ti_state);
				tidg_sendfatal(q, mp);
				splx(sps);
				return;
			}
			putq(q, mp);
			break;

		case T_UNBIND_REQ:
		case T_BIND_REQ:
		case T_OPTMGMT_REQ:
			tidg_checkmsg(q, mp);
			break;

		default:
			strlog(TIDG_ID, TI_DG_NUM(tiptr) ,0, SL_TRACE|SL_ERROR,
				"tidgwput: bad prim type = %d\n", pptr->type);
			tidg_sendfatal(q, mp);
			break;
		}
		splx(sps);
		return;

	case M_IOCTL:
		mp->b_datap->db_type = M_IOCNAK;
		qreply(q, mp);
		return;
 	case M_FLUSH:
		if (*mp->b_rptr & FLUSHW)
			flushq(q, 0);
		if (!(*mp->b_rptr & FLUSHR))
			freemsg(mp);
		else {
			*mp->b_rptr &= ~FLUSHW;
			flushq(OTHERQ(q), 0);
			qreply(q, mp);
		}
		return;
		
	default:
		freemsg(mp);
		return;
	}
}


/*
 * tidgwsrv - tidg service routine for write queue
 */
tidgwsrv(q)
register queue_t *q;
{
	register mblk_t *mp;

	while((mp = getq(q)) != NULL) {
		/*
		 * The only thing on the write queue can be
		 * M_PROTO containing T_UNITDATA_REQ
		 */
		if (!do_udata(q, mp))
			return;
	}
}

/*
 * tidgrsrv - tidg service routine for read queue
 */
tidgrsrv(q)
register queue_t *q;
{
	register struct ti_tidg *tiptr;

	tiptr = (struct ti_tidg *)q->q_ptr;

	ASSERT(tiptr->ti_backwq);
	if (tiptr->ti_state == TS_IDLE)
		qenable(tiptr->ti_backwq);
}

tidg_checkmsg(q, mp)
register queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tidg *tiptr;

	/*
	 * check to see if the interface is in the correct 
	 * state for event
	 */

	tiptr = (struct ti_tidg *)q->q_ptr;
	pptr = (union T_primitives *)mp->b_rptr;

	if (tiptr->ti_flags&FATAL) {
		freemsg(mp);
		return;
	}

	switch(pptr->type) {

	case T_OPTMGMT_REQ:
		if (NEXTSTATE(TE_OPTMGMT_REQ, tiptr->ti_state) == NR) {
			tidg_snd_errack(q, mp, TOUTSTATE, 0);
			return;
		}
		tiptr->ti_state = NEXTSTATE(TE_OPTMGMT_REQ, tiptr->ti_state);
		tidg_optmgmt(q, mp);
		return;

	case T_UNBIND_REQ:
		if (NEXTSTATE(TE_UNBIND_REQ, tiptr->ti_state) == NR) {
			tidg_snd_errack(q, mp, TOUTSTATE, 0);
			return;
		}
		tiptr->ti_state = NEXTSTATE(TE_UNBIND_REQ, tiptr->ti_state);
		tidg_unbind(q, mp);
		return;

	case T_BIND_REQ:
		if (NEXTSTATE(TE_BIND_REQ, tiptr->ti_state) == NR) {
			tidg_snd_errack(q, mp, TOUTSTATE, 0);
			return;
		}
		tiptr->ti_state = NEXTSTATE(TE_BIND_REQ, tiptr->ti_state);
		tidg_bind(q, mp);
		return;
	}
}


tidg_sendfatal(q, mp)
register queue_t *q;
register mblk_t *mp;
{
	register struct ti_tidg *tiptr;

	tiptr = (struct ti_tidg *)q->q_ptr;

	tidg_finished(q);
	tiptr->ti_flags |= FATAL;
	mp->b_datap->db_type = M_ERROR;
	*mp->b_datap->db_base = EPROTO;
	mp->b_rptr = mp->b_datap->db_base;
	mp->b_wptr = mp->b_datap->db_base + sizeof(char);
	freemsg(unlinkb(mp));
	if (q->q_flag&QREADR)
		putnext(q, mp);
	else
		qreply(q, mp);
}

tidg_finished(q)
register queue_t *q;
{
	register struct ti_tidg *tiptr;

	tiptr = (struct ti_tidg *)q->q_ptr;
	flushq(q, 1);
	tiptr->ti_state = TS_UNBND;
}


tidg_snd_errack(q, mp, tli_error, unix_error)
queue_t *q;
register mblk_t *mp;
long tli_error;
long unix_error;
{
	mblk_t *tmp;
	long type;
	register union T_primitives *pptr;

	pptr = (union T_primitives *)mp->b_rptr;
	type = pptr->type;
	/*
	 * is message large enough to send
	 * up a T_ERROR_ACK primitive
	 */
	 if ((mp->b_datap->db_lim - mp->b_datap->db_base) < sizeof(struct T_error_ack)) {
		if ((tmp=allocb(sizeof(struct T_error_ack), BPRI_HI)) == NULL) {
			strlog(TIDG_ID, -1, 0, SL_TRACE|SL_ERROR,
			   "snd_errack: couldn't allocate msg\n");
			tidg_sendfatal(q, mp);
			return;
		}
		freemsg(mp);
		mp = tmp;
	 }
	 mp->b_rptr = mp->b_datap->db_base;
	 mp->b_wptr = mp->b_rptr + sizeof(struct T_error_ack);
	 pptr = (union T_primitives *)mp->b_rptr;
	 pptr->error_ack.ERROR_prim = type;
	 pptr->error_ack.TLI_error = tli_error;
	 pptr->error_ack.UNIX_error = unix_error;
	 pptr->error_ack.PRIM_type = T_ERROR_ACK;
	 mp->b_datap->db_type = M_PCPROTO;
	 freemsg(unlinkb(mp));
	 if (q->q_flag&QREADR)
		putnext(q, mp);
	 else
		qreply(q, mp);
	 return;
}

tidg_ireq(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	struct ti_tidg *tiptr;
	mblk_t *ack;

	tiptr = (struct ti_tidg *)q->q_ptr;
	if ((ack=allocb(sizeof(struct T_info_ack), BPRI_HI)) == NULL) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		   "tidg_ireq: couldn't flush or allocate ack msg\n");
		tidg_sendfatal(q, mp);
		return;
	}
	freemsg(mp);
	ack->b_wptr = ack->b_rptr + sizeof(struct T_info_ack);
	ack->b_datap->db_type = M_PCPROTO;
	pptr = (union T_primitives *)ack->b_rptr;
	pptr->info_ack.PRIM_type = T_INFO_ACK;
	pptr->info_ack.TSDU_size = TIDU_SIZE;
	pptr->info_ack.ETSDU_size = -2;
	pptr->info_ack.CDATA_size = -2;
	pptr->info_ack.DDATA_size = -2;
	pptr->info_ack.ADDR_size = sizeof(long);
	pptr->info_ack.OPT_size = sizeof(long);
	pptr->info_ack.TIDU_size = TIDU_SIZE;
	pptr->info_ack.SERV_type = T_CLTS;
	pptr->info_ack.CURRENT_state = tiptr->ti_state;
	qreply(q, ack);
}

tidg_bind(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tidg *tiptr;
	register mblk_t *sptr;
	register struct stroptions *soptr;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tidg *)q->q_ptr;

	if ((sptr = allocb(sizeof(struct stroptions), BPRI_HI)) == NULL) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		   "tidg_bind: couldn't allocate stropts msg\n");
		tidg_snd_errack(q, mp, TSYSERR, EAGAIN);
		return;
	}
	soptr = (struct stroptions *)sptr->b_rptr;
	sptr->b_wptr = sptr->b_rptr + sizeof(struct stroptions);
	soptr->so_flags = SO_MINPSZ|SO_MAXPSZ|SO_HIWAT|SO_LOWAT;
	soptr->so_readopt = 0;
	soptr->so_wroff = 0;
	soptr->so_minpsz = 0;
	soptr->so_maxpsz = TIDU_SIZE;
	soptr->so_hiwat = 4*TIDU_SIZE;
	soptr->so_lowat = TIDU_SIZE;
	sptr->b_datap->db_type = M_SETOPTS;
	qreply(q, sptr);

	if (((mp->b_wptr - mp->b_rptr) < (sizeof(struct T_bind_req))) ||
	    ((mp->b_wptr - mp->b_rptr) < (pptr->bind_req.ADDR_offset +
					 pptr->bind_req.ADDR_length))) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		   "tidg_bind: bad control info in request msg\n");
		tidg_snd_errack(q, mp, TSYSERR, EINVAL);
		return;
	}

	tiptr->ti_addr = 0;

	if (pptr->bind_req.ADDR_length == 0) {
		register struct tidg_addr *tmp;
		register mblk_t *new;

		if ((new = allocb(sizeof(struct T_bind_ack) + sizeof(long), BPRI_HI)) == NULL) {
			strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		 	  "tidg_bind: couldn't allocate ack msg\n");
			tidg_snd_errack(q, mp, TSYSERR, EAGAIN);
			return;
		}

		freemsg(mp);
		mp = new;
		pptr = (union T_primitives *)mp->b_rptr;
		mp->b_wptr = mp->b_rptr + sizeof(struct T_bind_ack) + sizeof(long);

		pptr->bind_ack.CONIND_number = 0;
		for (tmp = tidg_addr; tmp < &tidg_addr[MAXADDR]; tmp++) {
			if (!tmp->used) {
				tmp->used++;
				*(unsigned long *)(mp->b_rptr + sizeof(struct T_bind_ack)) = tmp->addr;
				tiptr->ti_addr = tmp->addr;
				pptr->bind_ack.ADDR_offset = sizeof(struct T_bind_ack);
				pptr->bind_ack.ADDR_length = sizeof(long);
				break;
			}
		}
		if (tmp > &tidg_addr[MAXADDR-1]) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tidg_snd_errack(q, mp, TNOADDR, 0);
			return;
		}
	} else {
		register struct tidg_addr *tmp;
		unsigned long *addr;
		int addrinuse = 0;

		if ((pptr->bind_req.ADDR_length != sizeof(long)) ||
		    (pptr->bind_req.ADDR_offset < sizeof(struct T_bind_req)) ||
		    ((pptr->bind_req.ADDR_offset + sizeof(long)) < (mp->b_wptr - mp->b_rptr))) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tidg_snd_errack(q, mp, TBADADDR, 0);
			return;
		}
		addr = (unsigned long *)(mp->b_rptr + pptr->bind_req.ADDR_offset);

		for (tmp = tidg_addr; tmp < &tidg_addr[MAXADDR]; tmp++) {
			if (tmp->addr == *addr) {
				if (tmp->used)
					addrinuse++;
				else
					tmp->used++;
				break;
			}
		}
		if (tmp >= &tidg_addr[MAXADDR-1]) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tidg_snd_errack(q, mp, TBADADDR, 0);
			return;
		}
		if (addrinuse) {
			/*
			 * Address already in use, so assign another one.
			 */
			for (tmp = tidg_addr; tmp < &tidg_addr[MAXADDR]; tmp++) {
				if (!tmp->used) {
					tmp->used++;
					*addr = tmp->addr;
					break;
				}
			}
			if (tmp >= &tidg_addr[MAXADDR]) {
				tiptr->ti_state = NEXTSTATE(T_ERROR_ACK, tiptr->ti_state);
				tidg_snd_errack(q, mp, TNOADDR, 0);
				return;
			}
		}
		tiptr->ti_addr = *addr;
	}
	mp->b_datap->db_type = M_PCPROTO;
	pptr->bind_ack.PRIM_type = T_BIND_ACK;
	tiptr->ti_state = NEXTSTATE(TE_BIND_ACK,tiptr->ti_state);
	qreply(q, mp);
}

tidg_unbind(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register struct ti_tidg *tiptr;
	register struct tidg_addr *taddr;

	tiptr = (struct ti_tidg *)q->q_ptr;
	if (!snd_flushrw(q)) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		  "tidg_unbind: couldn't flush queues\n");
		tidg_snd_errack(q, mp, TSYSERR, EPROTO);
		return;
	}
	for (taddr = tidg_addr; taddr < &tidg_addr[MAXADDR]; taddr++)
		if (taddr->addr == tiptr->ti_addr) {
			taddr->used = 0;
			break;
		}
	tiptr->ti_addr = 0;
	tiptr->ti_state = NEXTSTATE(TE_OK_ACK1, tiptr->ti_state);
	tidg_snd_okack(q, mp);
	return;
}


tidg_snd_okack(q, mp)
queue_t *q;
register mblk_t *mp;
{
	mblk_t *tmp;
	long type;
	register union T_primitives *pptr;

	pptr = (union T_primitives *)mp->b_rptr;
	type = pptr->type;
	/*
	 * is message large enough to send
	 * up a T_OK_ACK primitive
	 */
	 if ((mp->b_datap->db_lim - mp->b_datap->db_base) < sizeof(struct T_ok_ack)) {
		if ((tmp=allocb(sizeof(struct T_ok_ack), BPRI_HI)) == NULL) {
			strlog(TIDG_ID, -1, 0, SL_TRACE|SL_ERROR,
			  "tidg_snd_okack: couldn't allocate ack msg\n");
			tidg_sendfatal(q, mp);
			return;
		}
		freemsg(mp);
		mp = tmp;
	 }
	 mp->b_rptr = mp->b_datap->db_base;
	 mp->b_wptr = mp->b_rptr + sizeof(struct T_ok_ack);
	 pptr = (union T_primitives *)mp->b_rptr;
	 pptr->ok_ack.CORRECT_prim = type;
	 pptr->ok_ack.PRIM_type = T_OK_ACK;
	 mp->b_datap->db_type = M_PCPROTO;
	 freemsg(unlinkb(mp));
	 if (q->q_flag&QREADR)
		putnext(q, mp);
	 else
		qreply(q, mp);
	 return;
}

tidg_optmgmt(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tidg *tiptr;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tidg *)q->q_ptr;

	if (((mp->b_wptr - mp->b_rptr) < (sizeof(struct T_optmgmt_req))) ||
	    ((mp->b_wptr - mp->b_rptr) < (pptr->optmgmt_req.OPT_offset +
					 pptr->optmgmt_req.OPT_length))) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		  "tidg_optmgmt: bad control part of msg\n");
		tidg_snd_errack(q, mp, TSYSERR, EINVAL);
		return;
	}
		
	switch(pptr->optmgmt_req.MGMT_flags) {

	mblk_t *tmp;
	case T_DEFAULT:
		if ((tmp = allocb(sizeof(struct T_optmgmt_ack) + sizeof(long), BPRI_HI)) == NULL) {
			strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
			  "tidg_optmgmt: couldn't allocate DEFAULT ack msg\n");
			tidg_snd_errack(q, mp, TSYSERR, EAGAIN);
			return;
		}
		freemsg(mp);
		pptr = (union T_primitives *)tmp->b_rptr;
		tmp->b_wptr = tmp->b_rptr + sizeof(struct T_optmgmt_ack) +sizeof(long);
		tmp->b_datap->db_type = M_PCPROTO;
		pptr->optmgmt_ack.MGMT_flags = T_DEFAULT;
		pptr->optmgmt_ack.PRIM_type = T_OPTMGMT_ACK;
		pptr->optmgmt_ack.OPT_length = sizeof(long);
		pptr->optmgmt_ack.OPT_offset = sizeof(struct T_optmgmt_ack);
		*(long *)(tmp->b_rptr + sizeof(struct T_optmgmt_ack)) = DEFAULTOPT;
		tiptr->ti_state = NEXTSTATE(TE_OPTMGMT_ACK,tiptr->ti_state);
		qreply(q, tmp);
		return;

	case T_CHECK:
		if ((pptr->optmgmt_req.OPT_length != sizeof(long)) ||
		    (pptr->optmgmt_req.OPT_offset < sizeof(struct T_optmgmt_req))) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tidg_snd_errack(q, mp, TBADOPT, 0);
			return;
		}
		pptr->optmgmt_ack.PRIM_type = T_OPTMGMT_ACK;
		if (*(long *)(mp->b_rptr + pptr->optmgmt_req.OPT_offset) != DEFAULTOPT)
			pptr->optmgmt_ack.MGMT_flags |= T_FAILURE;
		else
			pptr->optmgmt_ack.MGMT_flags |= T_SUCCESS;
		tiptr->ti_state = NEXTSTATE(TE_OPTMGMT_ACK,tiptr->ti_state);
		mp->b_datap->db_type = M_PCPROTO;
		qreply(q, mp);
		return;
		
	case T_NEGOTIATE:
		if ((pptr->optmgmt_req.OPT_length != sizeof(long)) ||
		    (pptr->optmgmt_req.OPT_offset < sizeof(struct T_optmgmt_req))) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tidg_snd_errack(q, mp, TBADOPT, 0);
			return;
		}
		pptr->optmgmt_ack.PRIM_type = T_OPTMGMT_ACK;
		*(long *)(mp->b_rptr + pptr->optmgmt_req.OPT_offset) = DEFAULTOPT;
		tiptr->ti_state = NEXTSTATE(TE_OPTMGMT_ACK,tiptr->ti_state);
		mp->b_datap->db_type = M_PCPROTO;
		qreply(q, mp);
		return;
		
	default:
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		tidg_snd_errack(q, mp, TBADFLAG, 0);
		return;
	}
}

do_udata(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tidg *tiptr, *rtiptr;
	register struct tidg_addr *tmp;
	int size;
	int error = 0;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tidg *)q->q_ptr;

	if ((size = mp->b_wptr - mp->b_rptr) < sizeof(struct T_unitdata_req))
		error++;
	if (size < (pptr->unitdata_req.DEST_length + pptr->unitdata_req.DEST_offset))
		error++;
	if (pptr->unitdata_req.OPT_length) {
		if (size < (pptr->unitdata_req.OPT_length + pptr->unitdata_req.OPT_offset))
			error++;
	}

	if (error) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		  "do_udata: bad control part of T_UNITDATA_REQ\n");
		tidg_sendfatal(q, mp);
		return(0);
	}
	if (msgdsize(mp) > TIDU_SIZE) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		  "do_udata: exceeded TIDU size on T_UNITDATA_REQ\n");
		tidg_sendfatal(q, mp);
		return(0);
	}

	if (pptr->unitdata_req.DEST_length != sizeof(long)) {
		tidg_snd_uderr(q, mp, UDBADADDR);
#ifndef	OLDBUG
		return(1);
#else
		return(0);
#endif
	}
	for (tmp = tidg_addr; tmp < &tidg_addr[MAXADDR]; tmp++) {
		if (tmp->addr == *(long *)(mp->b_rptr + pptr->unitdata_req.DEST_offset))
			break;
	}
	if (tmp >= &tidg_addr[MAXADDR]) {
		tidg_snd_uderr(q, mp, UDBADADDR);
#ifndef	OLDBUG
		return(1);
#else
		return(0);
#endif
	}
	if (pptr->unitdata_req.OPT_length)
		if ((pptr->unitdata_req.OPT_length != sizeof(long)) ||
		    (*(long *)(mp->b_rptr + pptr->unitdata_req.OPT_offset) != DEFAULTOPT)) {
			tidg_snd_uderr(q, mp, UDBADOPTS);
#ifndef	OLDBUG
		return(1);
#else
		return(0);
#endif
		}

	for (rtiptr = ti_tidg; rtiptr < &ti_tidg[tidg_cnt]; rtiptr++)
		if ((rtiptr->ti_addr == tmp->addr) && (rtiptr->ti_state == TS_IDLE))
			break;

	if (rtiptr >= &ti_tidg[tidg_cnt]) {
		tidg_snd_uderr(q, mp, UDUNREACHABLE);
#ifndef	OLDBUG
		return(1);
#else
		return(0);
#endif
	}

	/*
	 * Make sure you can put a message on rd queue, and store if not.
	 * Must do this before you convert T_UNITDATA_REQ into T_UNITDATA_IND.
	 */
	if (!canput(rtiptr->ti_rdq->q_next)) {
		rtiptr->ti_backwq = q;
		putbq(q, mp);
		return(0);
	}

	/*
	 * Request is valid - create the indication to
	 * send to remote destination, using the control
	 * buffer from the T_UNITDATA_REQ.  It's ok to overlay
	 * the indication structure on the request structure
	 * since fields are same size.  Must insert src addr
	 * but leave options untouched.
	 */
	pptr->type = T_UNITDATA_IND;
	*(long *)(mp->b_rptr + pptr->unitdata_ind.SRC_offset) = tiptr->ti_addr;
	rtiptr->ti_state = NEXTSTATE(TE_UNITDATA_IND, rtiptr->ti_state);
	putnext(rtiptr->ti_rdq, mp);
	return(1);
}


tidg_snd_uderr(q, mp, error)
register queue_t *q;
register mblk_t *mp;
long error;
{
	register union T_primitives *pptr, *tpptr;
	register struct ti_tidg *tiptr;
	unsigned long destaddr = 0;
	long options = 0;
	long uderr_size;
	mblk_t *tmp;

	tiptr = (struct ti_tidg *)q->q_ptr;
	pptr = (union T_primitives *)mp->b_rptr;
	destaddr = *((long *)(mp->b_rptr + pptr->unitdata_req.DEST_offset));
	if (pptr->unitdata_req.OPT_length)
		options = *((long *)(mp->b_rptr + pptr->unitdata_req.OPT_offset));
	uderr_size = sizeof(struct T_uderror_ind) + 2*sizeof(long);

	if ((tmp = allocb(uderr_size, BPRI_HI)) == NULL) {
		strlog(TIDG_ID, TI_DG_NUM(tiptr), 0, SL_TRACE|SL_ERROR,
		  "snd_uderr: couldn't allocate T_UDERROR_IND msg\n");
		tidg_sendfatal(q, mp);
		return;
	}
	tpptr = (union T_primitives *)tmp->b_rptr;
	tmp->b_datap->db_type = M_PROTO;
	tmp->b_wptr = tmp->b_rptr + sizeof(struct T_uderror_ind) + sizeof(long);
	tpptr->uderror_ind.PRIM_type = T_UDERROR_IND;
	tpptr->uderror_ind.ERROR_type = error;
	tpptr->uderror_ind.DEST_length = sizeof(long);
	tpptr->uderror_ind.DEST_offset = sizeof(struct T_uderror_ind);
	*(long *)(tmp->b_rptr + tpptr->uderror_ind.DEST_offset) = destaddr;
	if (options) {
		tpptr->uderror_ind.OPT_length = sizeof(long);
		tpptr->uderror_ind.OPT_offset = sizeof(struct T_uderror_ind) + sizeof(long);
		*(long *)(tmp->b_rptr + tpptr->uderror_ind.OPT_offset) = options;
		tmp->b_wptr += sizeof(long);
	} else {
		tpptr->uderror_ind.OPT_length = 0;
		tpptr->uderror_ind.OPT_offset = 0;
	}
	tiptr->ti_state = NEXTSTATE(TE_UDERROR_IND, tiptr->ti_state);
	freemsg(mp);
	if (q->q_flag & QREADR)
		putnext(q, tmp);
	else
		qreply(q, tmp);
	return;
}
