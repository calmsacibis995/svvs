/*
 * Transport Interface Library connection oriented test driver - issue 1
 */

#include "sys/param.h"
#include "sys/types.h"
#include "sys/sysmacros.h"
#include "sys/stream.h"
#include "sys/stropts.h"
#include "sys/tihdr.h"
#include "tivc.h"
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


extern struct ti_tivc ti_tivc[];
extern int tivc_cnt;
extern nulldev();

#define NR 	127

/* tivc identifcation id (should not be static should be
 * configurable).
 * Also a Interface ID string should be included so modules
 * may not be pushed with unlike interfaces
 */
#define TIVC_ID	2222


/* stream data structure definitions */
int tivcopen(), tivcclose(), tivcwput(), tivcrsrv(), tivcwsrv();
static struct module_info tivc_info = {TIVC_ID, "tivc", 0, TIDU_SIZE, 4096, 1024};
static struct qinit tivcrinit = { NULL, tivcrsrv, tivcopen, tivcclose, nulldev, &tivc_info, NULL};
static struct qinit tivcwinit = { tivcwput, tivcwsrv, tivcopen, tivcclose, nulldev, &tivc_info, NULL};
struct streamtab tivinfo = { &tivcrinit, &tivcwinit, NULL, NULL };


extern char ti_statetbl[TE_NOEVENTS][TS_NOSTATES];


struct tivc_addr {
	unsigned long addr;
	int used;
};


struct tivc_addr tivc_addr[MAXADDR] = {
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

ushort seq_counter;


/* macro to change state NEXTSTATE(event, current state) */
#define NEXTSTATE(X,Y) ti_statetbl[X][Y]

/*
 * tivcopen - open routine gets called when the
 *	       driver gets opened.
 */

tivcopen(q, dev, flag, sflag)
register queue_t *q;
{
	register struct ti_tivc *tiptr;

	ASSERT(q != NULL);

	/* is it already opened */
	if (q->q_ptr) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       "tivcopen: opened already, open succeeded\n");
		return(TI_VC_NUM(q->q_ptr));
	}

	if (sflag == CLONEOPEN) {
		for (tiptr = ti_tivc; tiptr < &ti_tivc[tivc_cnt]; tiptr++)
			if (!(tiptr->ti_flags & USED))
				break;
		if (tiptr > &ti_tivc[tivc_cnt-1]) {
			STRLOG(TIVC_ID, -1,0, SL_TRACE|SL_ERROR,
			       "tivcopen: opened failed: couldn't allocate ti_tivc data structure for q = %x\n",
			       (caddr_t)q);
			u.u_error = ENOSPC;
			return(OPENFAIL);
		}
	} else {
		dev = minor(dev);
		if (dev > (tivc_cnt -1)) {
			u.u_error = ENODEV;
			return(OPENFAIL);
		}

		tiptr = &ti_tivc[dev];
	}

	ASSERT(tiptr <= &ti_tivc[tivc_cnt-1]);

	/* initialize data structure */
	tiptr->ti_state = TS_UNBND;
	tiptr->ti_flags = USED;
	tiptr->ti_rdq = q;
	tiptr->ti_qlen = 0;
	tiptr->ti_addr = 0;
	tiptr->ti_remoteq = NULL;
	tiptr->ti_conseq = 0;
	tiptr->ti_outq = NULL;
	tiptr->ti_seq[0].used = FALSE;
	tiptr->ti_seq[1].used = FALSE;
	tiptr->ti_seq[2].used = FALSE;
	tiptr->ti_seq[3].used = FALSE;
	tiptr->ti_seqcnt = 0;
	tiptr->ti_etsdu = 0;
	tiptr->ti_tsdu = 0;


	/* assign tiptr to queue private pointers */
	q->q_ptr = (caddr_t)tiptr;
	WR(q)->q_ptr = (caddr_t)tiptr;

	STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
	       "tivcopen: open succeeded\n");

	return(TI_VC_NUM(tiptr));
}


/*
 * tivcclose - This routine gets called when the module
 *              gets closed.
 */

tivcclose(q)
register queue_t *q;
{
	register struct ti_tivc *tiptr;
	register struct ti_tivc *tmp;
	register struct tivc_addr *taddr;
	unsigned long addr;
	int used = 0;

	tiptr = (struct ti_tivc *)q->q_ptr;

	ASSERT(tiptr != NULL);


	if (tiptr->ti_addr) {
		addr = tiptr->ti_addr;
		tiptr->ti_addr = 0;
	
		for (tmp = ti_tivc; tmp < &ti_tivc[tivc_cnt]; tmp++)
			if (addr == tmp->ti_addr)
				used++;
		for (taddr = tivc_addr; taddr < &tivc_addr[MAXADDR]; taddr++)
			if (addr == taddr->addr)
				taddr->used = used;
	}
	tivc_finished(q);

	tiptr->ti_state = TS_UNBND;
	tiptr->ti_flags = 0;
	tiptr->ti_rdq = 0;

	STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
	       "tivcclose: close succeeded\n");
}




/*
 * tivcwput - Driver write queue put procedure.
 */

tivcwput(q, mp)
register queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tivc *tiptr;
	int sps;

	ASSERT(q != NULL);

	tiptr = (struct ti_tivc *)q->q_ptr;

	ASSERT(tiptr != NULL);


	/* switch on message type */
	switch(mp->b_datap->db_type) {

	case M_DATA:
		STRLOG(TIVC_ID, TI_VC_NUM(tiptr),0, SL_TRACE,
	       		"tivcwput: M_DATA type %o recieved mp = %x\n",
	       		mp->b_datap->db_type, mp);
		sps = splstr();
		if ((tiptr->ti_state == TS_IDLE) || (tiptr->ti_flags & FATAL)) {
			freemsg(mp);
			splx(sps);
			return;
		}
		if (NEXTSTATE(TE_DATA_REQ, tiptr->ti_state) == NR) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       		"tivcwput 1: data message places interface out of state\n");
			tivc_sendfatal(q, mp);
			splx(sps);
			return;
		}
		putq(q, mp);
		splx(sps);
		return;
	case M_PROTO:
	case M_PCPROTO:
		ASSERT((mp->b_wptr - mp->b_rptr) >= sizeof(long));

		pptr = (union T_primitives *)mp->b_rptr;

		STRLOG(TIVC_ID, TI_VC_NUM(tiptr),0, SL_TRACE,
		       "tivcwput: TImsg type = %d recieved\n", pptr->type);

		if ((mp->b_wptr - mp->b_rptr) < sizeof(long)) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       		"tivcwput 2: PROTO msg less than size of long\n");
			tivc_sendfatal(q, mp);
			return;
		}

		sps = splstr();
		/* switch on primitive type */
		switch (pptr->type) {

		case T_INFO_REQ:
			tivc_ireq(q, mp);
			break;

		case T_ORDREL_REQ:
			if (NEXTSTATE(TE_ORDREL_REQ, tiptr->ti_state) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       			"tivcwput 3: ordrel_req message places interface out of state\n");
				tivc_sendfatal(q, mp);
				splx(sps);
				return;
			}
			putq(q, mp);
			splx(sps);
			break;

		case T_DATA_REQ:
		case T_EXDATA_REQ:
			if ((tiptr->ti_state == TS_IDLE) || (tiptr->ti_flags & FATAL)) {
				freemsg(mp);
				splx(sps);
				return;
			}
			if (NEXTSTATE(TE_DATA_REQ, tiptr->ti_state) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       			"tivcwput 4: data message places interface out of state\n");
				tivc_sendfatal(q, mp);
				splx(sps);
				return;
			}
			putq(q, mp);
			splx(sps);
			return;

		case T_CONN_REQ:
		case T_CONN_RES:
		case T_DISCON_REQ:
		case T_UNBIND_REQ:
		case T_BIND_REQ:
		case T_OPTMGMT_REQ:
			tivc_checkmsg(q, mp);
			splx(sps);
			return;
		default:
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       		"tivcwput 5: bad TI message type received\n");
			tivc_sendfatal(q, mp);
			splx(sps);
			return;
		}
		splx(sps);
		return;

	case M_IOCTL:
		STRLOG(TIVC_ID, TI_VC_NUM(tiptr),0, SL_TRACE,
	       		"tivcwput: M_IOCTL type %o recieved mp = %x\n",
	       		mp->b_datap->db_type, mp);
		mp->b_datap->db_type = M_IOCNAK;
		qreply(q, mp);
		return;
 	case M_FLUSH:
		STRLOG(TIVC_ID, TI_VC_NUM(tiptr),0, SL_TRACE,
	       		"tivcwput: M_FLUSH type %o recieved mp = %x\n",
	       		mp->b_datap->db_type, mp);
		if (*mp->b_rptr & FLUSHW)
			flushq(q, 0);
		if (!(*mp->b_rptr & FLUSHR))
			freemsg(mp);
		else {
			flushq(OTHERQ(q), 0);
			*mp->b_rptr &= ~FLUSHW;
			qreply(q, mp);
		}
		return;
		
	default:
		STRLOG(TIVC_ID, TI_VC_NUM(tiptr),0, SL_TRACE,
	       		"tivcwput: Non understood STR msg type %o recieved mp = %x\n",
	       		mp->b_datap->db_type, mp);
		freemsg(mp);
		return;
	}
}


/*
 * tivcwsrv - tivc service routine for write queue
 */
tivcwsrv(q)
register queue_t *q;
{
	register mblk_t *mp;
	register union T_primitives *pptr;

	while((mp = getq(q)) != NULL) {

	switch (mp->b_datap->db_type) {
		case M_DATA:
			if (!do_data(q, mp, TE_DATA_REQ))
				return;
			break;
		case M_PROTO:
		case M_PCPROTO:
			pptr = (union T_primitives *)mp->b_rptr;

			switch (pptr->type) {
			case T_ORDREL_REQ:
				if (!do_data(q, mp, TE_ORDREL_REQ))
					return;
				break;
			case T_DATA_REQ:
				if (!do_data(q, mp, TE_DATA_REQ))
					return;
				break;
			case T_EXDATA_REQ:
				if (!do_data(q, mp, TE_EXDATA_REQ))
					return;
				break;
			}
			break;
		}
	}
}

/*
 * tivcrsrv - tivc service routine for read queue
 */
tivcrsrv(q)
register queue_t *q;
{
	register struct ti_tivc *tiptr;

	tiptr = (struct ti_tivc *)q->q_ptr;

	if ((tiptr->ti_state == TS_DATA_XFER) ||
	    (tiptr->ti_state == TS_WIND_ORDREL) ||
	    (tiptr->ti_state == TS_WREQ_ORDREL))
		qenable(WR(tiptr->ti_remoteq));
}

tivc_checkmsg(q, mp)
register queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tivc *tiptr;

	/*
	 * check to see if the interface is in the correct 
	 * state for event
	 */

	tiptr = (struct ti_tivc *)q->q_ptr;
	pptr = (union T_primitives *)mp->b_rptr;

	if (tiptr->ti_flags&FATAL) {
		freemsg(mp);
		return;
	}

	switch(pptr->type) {
		case T_CONN_REQ:
			if (NEXTSTATE(TE_CONN_REQ, tiptr->ti_state) == NR) {
				snd_error_ack(q, mp, TOUTSTATE, 0);
				return;
			}
			tiptr->ti_state = NEXTSTATE(TE_CONN_REQ, tiptr->ti_state);
			tivc_creq(q, mp);
			return;

		case T_CONN_RES:
			if (NEXTSTATE(TE_CONN_RES, tiptr->ti_state) == NR) {
				snd_error_ack(q, mp, TOUTSTATE, 0);
				return;
			}
			tiptr->ti_state = NEXTSTATE(TE_CONN_RES, tiptr->ti_state);
			tivc_cres(q, mp);
			return;

		case T_DISCON_REQ:
			if (NEXTSTATE(TE_DISCON_REQ, tiptr->ti_state) == NR) {
				snd_error_ack(q, mp, TOUTSTATE, 0);
				return;
			}
			tiptr->ti_state = NEXTSTATE(TE_DISCON_REQ, tiptr->ti_state);
			tivc_dreq(q, mp);
			return;

		case T_OPTMGMT_REQ:
			if (NEXTSTATE(TE_OPTMGMT_REQ, tiptr->ti_state) == NR) {
				snd_error_ack(q, mp, TOUTSTATE, 0);
				return;
			}
			tiptr->ti_state = NEXTSTATE(TE_OPTMGMT_REQ, tiptr->ti_state);
			tivc_optmgmt(q, mp);
			return;

		case T_UNBIND_REQ:
			if (NEXTSTATE(TE_UNBIND_REQ, tiptr->ti_state) == NR) {
				snd_error_ack(q, mp, TOUTSTATE, 0);
				return;
			}
			tiptr->ti_state = NEXTSTATE(TE_UNBIND_REQ, tiptr->ti_state);
			tivc_unbind(q, mp);
			return;

		case T_BIND_REQ:
			if (NEXTSTATE(TE_BIND_REQ, tiptr->ti_state) == NR) {
				snd_error_ack(q, mp, TOUTSTATE, 0);
				return;
			}
			tiptr->ti_state = NEXTSTATE(TE_BIND_REQ, tiptr->ti_state);
			tivc_bind(q, mp);
			return;
			}
	return;
}


tivc_sendfatal(q, mp)
register queue_t *q;
register mblk_t *mp;
{
	register struct ti_tivc *tiptr;

	tiptr = (struct ti_tivc *)q->q_ptr;

	tivc_finished(q);
	tiptr->ti_flags |= FATAL;
	mp->b_datap->db_type = M_ERROR;
	*mp->b_datap->db_base = EPROTO;
	mp->b_rptr = mp->b_datap->db_base;
	mp->b_wptr = mp->b_datap->db_base + sizeof(char);
	freemsg(unlinkb(mp));
	if (q->q_flag&QREADR)
		(*q->q_next->q_qinfo->qi_putp)(q->q_next, mp);
	else
		qreply(q, mp);
}

tivc_finished(q)
register queue_t *q;
{
	register union T_primitives *dpptr;
	register struct ti_tivc *tiptr;
	register struct ti_tivc *rtiptr;
	register mblk_t *dp;
	struct tiseq *seq;

	tiptr = (struct ti_tivc *)q->q_ptr;

	flushq(q, 1);

	if (tiptr->ti_seqcnt) {
		for (seq = tiptr->ti_seq; seq <= &tiptr->ti_seq[MAX_CONN_IND-1] ; seq++) {
			if (seq->used == TRUE) {

				for (rtiptr = ti_tivc; rtiptr < &ti_tivc[tivc_cnt]; rtiptr++)
					if (rtiptr->ti_conseq == seq->seqno)
						break;
				if ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL) {
					printf("tivc_finished: couldn't allocate block to send discon\n");
					break;
				}
				dpptr = (union T_primitives *)dp->b_rptr;
				dp->b_datap->db_type = M_PROTO;
				rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND1, rtiptr->ti_state);
				rtiptr->ti_remoteq = NULL;
				rtiptr->ti_outq = NULL;
				rtiptr->ti_conseq = 0;
				dp->b_wptr = dp->b_rptr + sizeof(struct T_discon_ind);
				dpptr->discon_ind.PRIM_type = T_DISCON_IND;
				dpptr->discon_ind.DISCON_reason = PROVIDERINITIATED;
				dpptr->discon_ind.SEQ_number = -1;
				(*rtiptr->ti_rdq->q_next->q_qinfo->qi_putp)(rtiptr->ti_rdq->q_next, dp);
			}
		}
	}

	if (tiptr->ti_seqcnt == 0) {
		if ((tiptr->ti_remoteq == 0)&&(tiptr->ti_outq != NULL)) {
			rtiptr = (struct ti_tivc *)tiptr->ti_outq->q_ptr;
			if ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL) {
				printf("tivc_finished: couldn't allocate block to send discon\n");
				goto endit;
			}
			for (seq = rtiptr->ti_seq; seq < &rtiptr->ti_seq[MAX_CONN_IND]; seq++)
				if (seq->seqno == tiptr->ti_conseq)
					seq->used = FALSE;
			dpptr = (union T_primitives *)dp->b_rptr;
			dpptr->discon_ind.SEQ_number = tiptr->ti_conseq;
			if (rtiptr->ti_seqcnt > 1)
				rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND3, rtiptr->ti_state);
			else
				rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND2, rtiptr->ti_state);
			rtiptr->ti_seqcnt--;
			tiptr->ti_remoteq = NULL;
			tiptr->ti_outq = NULL;
			tiptr->ti_conseq = 0;
			dp->b_wptr = dp->b_rptr + sizeof(struct T_discon_ind);
			dp->b_datap->db_type = M_PROTO;
			dpptr->discon_ind.PRIM_type = T_DISCON_IND;
			dpptr->discon_ind.DISCON_reason = PROVIDERINITIATED;
			(*rtiptr->ti_rdq->q_next->q_qinfo->qi_putp)(rtiptr->ti_rdq->q_next, dp);
		} else {
			if (tiptr->ti_remoteq != NULL) {
				rtiptr = (struct ti_tivc *)tiptr->ti_remoteq->q_ptr;
				if (!snd_flushrw(tiptr->ti_remoteq) || ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL)) {
					printf("tivc_finished: couldn't allocate block to send discon\n");
					goto endit;
				}
				flushq(WR(rtiptr->ti_rdq), 1);
				dpptr = (union T_primitives *)dp->b_rptr;
				dpptr->discon_ind.SEQ_number = -1;
				rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND1, rtiptr->ti_state);
				rtiptr->ti_remoteq = NULL;
				rtiptr->ti_outq = NULL;
				rtiptr->ti_conseq = 0;
				tiptr->ti_remoteq = NULL;
				tiptr->ti_outq = NULL;
				tiptr->ti_conseq = 0;
				dp->b_wptr = dp->b_rptr + sizeof(struct T_discon_ind);
				dp->b_datap->db_type = M_PROTO;
				dpptr->discon_ind.PRIM_type = T_DISCON_IND;
				dpptr->discon_ind.DISCON_reason = PROVIDERINITIATED;
				(*rtiptr->ti_rdq->q_next->q_qinfo->qi_putp)(rtiptr->ti_rdq->q_next, dp);
			}
		}
	}
endit:
	tiptr->ti_state = TS_UNBND;
	tiptr->ti_qlen = 0;
	tiptr->ti_addr = 0;
	tiptr->ti_remoteq = NULL;
	tiptr->ti_conseq = 0;
	tiptr->ti_outq = NULL;
	tiptr->ti_seq[0].used = FALSE;
	tiptr->ti_seq[1].used = FALSE;
	tiptr->ti_seq[2].used = FALSE;
	tiptr->ti_seq[3].used = FALSE;
	tiptr->ti_seqcnt = 0;
	tiptr->ti_etsdu = 0;
	tiptr->ti_tsdu = 0;
}

snd_error_ack(q, mp, tli_error, unix_error)
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
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		       		"tivc send_error_ack 7: couldn't allocate buffer for error_ack\n");
			tivc_sendfatal(q, mp);
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
		(*q->q_next->q_qinfo->qi_putp)(q->q_next, mp);
	 else
		qreply(q, mp);
	 return;
}

tivc_ireq(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	struct ti_tivc *tiptr;
	mblk_t *ack;

	if ((ack=allocb(sizeof(struct T_info_ack), BPRI_HI)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		     	"tivc_ireq 8: couldn't allocate buffer for info_ack\n");
		tivc_sendfatal(q, mp);
		return;
	}
	tiptr = (struct ti_tivc *)q->q_ptr;
	freemsg(mp);
	ack->b_wptr = ack->b_rptr + sizeof(struct T_info_ack);
	ack->b_datap->db_type = M_PCPROTO;
	pptr = (union T_primitives *)ack->b_rptr;
	pptr->info_ack.PRIM_type = T_INFO_ACK;
	pptr->info_ack.TSDU_size = TSDU_SIZE;
	pptr->info_ack.ETSDU_size = ETSDU_SIZE;
	pptr->info_ack.CDATA_size = CDATA_SIZE;
	pptr->info_ack.DDATA_size = DDATA_SIZE;
	pptr->info_ack.ADDR_size = sizeof(long);
	pptr->info_ack.OPT_size = sizeof(long);
	pptr->info_ack.TIDU_size = TIDU_SIZE;
	pptr->info_ack.SERV_type = T_COTS_ORD;
	pptr->info_ack.CURRENT_state = tiptr->ti_state;
	qreply(q, ack);
}

snd_flushrw(q)
queue_t *q;
{
	mblk_t *mp;

	if ((mp = allocb(1, BPRI_HI)) == NULL)
		return(0);
	mp->b_wptr++;
	mp->b_datap->db_type = M_FLUSH;
	*mp->b_rptr = FLUSHRW;
	if (q->q_flag&QREADR)
		(*q->q_next->q_qinfo->qi_putp)(q->q_next, mp);
	else
		qreply(q, mp);
	return(1);
}

tivc_bind(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tivc *tiptr;
	register mblk_t *sptr;
	register struct stroptions *soptr;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tivc *)q->q_ptr;

	if ((sptr = allocb(sizeof(struct stroptions), BPRI_HI)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		     	"tivc_bind 8: couldn't allocate buffer for stroptions\n");
		snd_error_ack(q, mp, TSYSERR, ENOMEM);
		return;
	}
	soptr = (struct stroptions *)sptr->b_rptr;
	sptr->b_wptr = sptr->b_rptr + sizeof(struct stroptions);
	soptr->so_flags = SO_MINPSZ|SO_MAXPSZ|SO_HIWAT|SO_LOWAT;
	soptr->so_readopt = 0;
	soptr->so_wroff = 0;
	soptr->so_minpsz = 0;
	soptr->so_maxpsz = TIDU_SIZE;
	soptr->so_hiwat = 4096;
	soptr->so_lowat = 1024;
	sptr->b_datap->db_type = M_SETOPTS;
	qreply(q, sptr);

	if (((mp->b_wptr - mp->b_rptr) < (sizeof(struct T_bind_req))) ||
	    ((mp->b_wptr - mp->b_rptr) < ( pptr->bind_req.ADDR_offset +
					 pptr->bind_req.ADDR_length))) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		     	"tivc_bind 9: bind_req incorrect message size\n");
		snd_error_ack(q, mp, TSYSERR, EINVAL);
		return;
	}

	if (pptr->bind_req.CONIND_number > MAX_CONN_IND)
		pptr->bind_ack.CONIND_number = MAX_CONN_IND;
	else
		pptr->bind_ack.CONIND_number = pptr->bind_req.CONIND_number;
	
	tiptr->ti_qlen = pptr->bind_ack.CONIND_number;
	tiptr->ti_addr = 0;

	if (pptr->bind_req.ADDR_length == 0) {
		register struct tivc_addr *tmp;
		register mblk_t *new;

		if ((new = allocb(sizeof(struct T_bind_ack) + sizeof(long), BPRI_HI)) == NULL) {
			tiptr->ti_qlen = 0;
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		     		"tivc_bind 10: couldn't alloc buffer for bind_ack\n");
			snd_error_ack(q, mp, TSYSERR, ENOMEM);
			return;
		}

		freemsg(mp);
		mp = new;
		pptr = (union T_primitives *)mp->b_rptr;
		mp->b_wptr = mp->b_rptr + sizeof(struct T_bind_ack) + sizeof(long);

		mp->b_datap->db_type = M_PCPROTO;
		/* this is here so error acks if any have right ERROR_prim */
		pptr->type = T_BIND_REQ;
		pptr->bind_ack.CONIND_number = tiptr->ti_qlen;
		for (tmp = tivc_addr; tmp < &tivc_addr[MAXADDR]; tmp++) {
			if (!tmp->used) {
				unsigned long  *addrptr;
				addrptr = (unsigned long *)(mp->b_rptr + sizeof(struct T_bind_ack));
				tmp->used++;
				*addrptr = tmp->addr;
				tiptr->ti_addr = tmp->addr;
				pptr->bind_ack.ADDR_offset = sizeof(struct T_bind_ack);
				pptr->bind_ack.ADDR_length = sizeof(long);
				break;
			}
		}
		if (tmp > &tivc_addr[MAXADDR-1]) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tiptr->ti_qlen = 0;
			snd_error_ack(q, mp, TNOADDR, 0);
			return;
		}
	} else {
		unsigned long *addr;
		register struct tivc_addr *tmp;

		if ((pptr->bind_req.ADDR_length != sizeof(long)) ||
		    (pptr->bind_req.ADDR_offset < sizeof(struct T_bind_req)) ||
		    ((pptr->bind_req.ADDR_offset + sizeof(long)) < (mp->b_wptr - mp->b_rptr))) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tiptr->ti_qlen = 0;
			snd_error_ack(q, mp, TBADADDR, 0);
			return;
		}
		addr = (unsigned long *)(mp->b_rptr + pptr->bind_req.ADDR_offset);

		for (tmp = tivc_addr; tmp < &tivc_addr[MAXADDR]; tmp++) {
			if (tmp->addr == *addr)
				break;
		}
		if (tmp >= &tivc_addr[MAXADDR]) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			tiptr->ti_qlen = 0;
			snd_error_ack(q, mp, TBADADDR, 0);
			return;
		}
		if (tiptr->ti_qlen > 0) {
			register struct ti_tivc *vtmp;

			for (vtmp = ti_tivc; vtmp < &ti_tivc[tivc_cnt]; vtmp++) {

				if ((vtmp->ti_addr == *addr) &&
				   		 (vtmp->ti_qlen > 0)) {
					register struct tivc_addr *ttmp;

					for (ttmp = tivc_addr; ttmp < &tivc_addr[MAXADDR]; ttmp++) {
						if (!ttmp->used) {
							ttmp->used++;
							*addr = ttmp->addr;
							break;
						}
					}
					if (ttmp >= &tivc_addr[MAXADDR]) {
						tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
						tiptr->ti_qlen = 0;
						snd_error_ack(q, mp, TNOADDR, 0);
						return;
					}
				}
			}
		}
		for (tmp = tivc_addr; tmp < &tivc_addr[MAXADDR]; tmp++)
			if (tmp->addr == *addr)
				tmp->used++;
		tiptr->ti_addr = *addr;
		
	}
	pptr->bind_ack.PRIM_type = T_BIND_ACK;
	mp->b_datap->db_type = M_PCPROTO;
	tiptr->ti_state = NEXTSTATE(TE_BIND_ACK,tiptr->ti_state);
	qreply(q, mp);
}

tivc_unbind(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register struct ti_tivc *tiptr;
	register struct ti_tivc *tmp;
	register struct tivc_addr *taddr;
	unsigned long addr;
	int used = 0;

	if (!snd_flushrw(q)) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		    	"tivc_unbind 10: snd_flushrw failed\n");
		snd_error_ack(q, mp, TSYSERR, EPROTO);
		return;
	}
	tiptr = (struct ti_tivc *)q->q_ptr;
	addr = tiptr->ti_addr;
	tiptr->ti_addr = 0;
	tiptr->ti_qlen = 0;

	for (tmp = ti_tivc; tmp < &ti_tivc[tivc_cnt]; tmp++)
		if (addr == tmp->ti_addr)
			used++;
	for (taddr = tivc_addr; taddr < &tivc_addr[MAXADDR]; taddr++)
		if (addr == taddr->addr)
			taddr->used = used;
	tiptr->ti_state = NEXTSTATE(TE_OK_ACK1, tiptr->ti_state);
	snd_ok_ack(q, mp);
	return;
}

snd_ok_ack(q, mp)
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
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		    		"tivc snd_ok_ack 11: couldn't alloc buffer for ok_ack\n");
			tivc_sendfatal(q, mp);
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
		(*q->q_next->q_qinfo->qi_putp)(q->q_next, mp);
	 else
		qreply(q, mp);
	 return;
}

tivc_optmgmt(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tivc *tiptr;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tivc *)q->q_ptr;

	if (((mp->b_wptr - mp->b_rptr) < (sizeof(struct T_optmgmt_req))) ||
	    ((mp->b_wptr - mp->b_rptr) < (pptr->optmgmt_req.OPT_offset +
					 pptr->optmgmt_req.OPT_length))) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		   	"tivc_optmgmt 12: optmgmt_req message incorrect size\n");
		snd_error_ack(q, mp, TSYSERR, EINVAL);
		return;
	}
		
	switch(pptr->optmgmt_req.MGMT_flags) {
	mblk_t *tmp;

	case T_DEFAULT:
		if ((tmp = allocb(sizeof(struct T_optmgmt_ack) + sizeof(long), BPRI_HI)) == NULL) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		   		"tivc_optmgmt 13: couldn't alloc buffer for optmgmt_ack\n");
			snd_error_ack(q, mp, TSYSERR, ENOMEM);
			return;
		}
		freemsg(mp);
		pptr = (union T_primitives *)tmp->b_rptr;
		tmp->b_datap->db_type = M_PCPROTO;
		tmp->b_wptr = tmp->b_rptr + sizeof(struct T_optmgmt_ack) +sizeof(long);
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
			snd_error_ack(q, mp, TBADOPT, 0);
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
			snd_error_ack(q, mp, TBADOPT, 0);
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
		snd_error_ack(q, mp, TBADFLAG, 0);
		return;
	}
}

tivc_creq(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	int size;
	int error = 0;
	register struct ti_tivc *tiptr;
	register struct tivc_addr *tmp;
	mblk_t *okptr;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tivc *)q->q_ptr;

	if ((size = mp->b_wptr - mp->b_rptr) < sizeof(struct T_conn_req)) {
		error++;
	}
	if (size < (pptr->conn_req.DEST_length + pptr->conn_req.DEST_offset)) {
		error++;
	}
	if (size < (pptr->conn_req.OPT_length + pptr->conn_req.OPT_offset)) {
		error++;
	}

	if (error) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  	"tivc_creq 14: conn_req message incorrect size\n");
		snd_error_ack(q, mp, TSYSERR, EINVAL);
		return;
	}

	if (pptr->conn_req.DEST_length != sizeof(long)) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK, tiptr->ti_state);
		snd_error_ack(q, mp, TBADADDR, 0);
		return;
	}
	for (tmp = tivc_addr; tmp < &tivc_addr[MAXADDR]; tmp++) {
		if (tmp->addr == *(unsigned long *)(mp->b_rptr + pptr->conn_req.DEST_offset))
			break;
	}
	if (tmp > &tivc_addr[MAXADDR-1]) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		snd_error_ack(q, mp, TBADADDR, 0);
		return;
	}
	if (pptr->conn_req.OPT_length != 0)
		if ((pptr->conn_req.OPT_length != sizeof(long)) ||
		    (*(long *)(mp->b_rptr + pptr->conn_req.OPT_offset) != DEFAULTOPT)) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			snd_error_ack(q, mp, TBADOPT, 0);
			return;
		}
	if (msgdsize(mp) > CDATA_SIZE) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		snd_error_ack(q, mp, TBADDATA, 0);
		return;
	}
	if ((okptr = copymsg(mp)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  	"tivc_creq 15: copymsg failed\n");
		snd_error_ack(q, mp, TSYSERR, ENOMEM);
		return;
	}
	tiptr->ti_state = NEXTSTATE(TE_OK_ACK1,tiptr->ti_state);
	snd_ok_ack(q, okptr);
	do_creq(q, mp);
	return;
}

tivc_cres(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	int size;
	int error = 0;
	register struct ti_tivc *tiptr;
	register struct ti_tivc *tmp;
	mblk_t *okptr;
	struct tiseq *seq;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tivc *)q->q_ptr;

	if ((size = mp->b_wptr - mp->b_rptr) < sizeof(struct T_conn_res))
		error++;
	if (size < (pptr->conn_res.OPT_length + pptr->conn_res.OPT_offset))
		error++;

	if (error) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  	"tivc_cres 16: conn_res message incorrect size\n");
		snd_error_ack(q, mp, TSYSERR, ENOMEM);
		return;
	}

	if (pptr->conn_res.OPT_length != 0)
		if ((pptr->conn_res.OPT_length != sizeof(long)) ||
		    (*(long *)(mp->b_rptr + pptr->conn_res.OPT_offset) != DEFAULTOPT)) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			snd_error_ack(q, mp, TBADOPT, 0);
			return;
		}

	if (pptr->conn_res.QUEUE_ptr != OTHERQ(q))  {
		for (tmp = ti_tivc; tmp < &ti_tivc[tivc_cnt]; tmp++) {
			if ((pptr->conn_res.QUEUE_ptr == tmp->ti_rdq) && 
			    ((tmp->ti_flags & (USED|FATAL)) == USED))
				break;
		}
		if (tmp > &ti_tivc[tivc_cnt -1]) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			snd_error_ack(q, mp, TBADF, 0);
			return;
		}
		if (tmp->ti_state != TS_IDLE) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			snd_error_ack(q, mp, TOUTSTATE, 0);
			return;
		}
	}
	for (seq = tiptr->ti_seq; seq < &tiptr->ti_seq[MAX_CONN_IND]; seq++) {
		if (pptr->conn_res.SEQ_number == seq->seqno) {
			seq->used = FALSE;
			break;
		}
	}
	if (seq > &tiptr->ti_seq[MAX_CONN_IND -1]) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		snd_error_ack(q, mp, TBADSEQ, 0);
		return;
	}
	if ((tiptr->ti_seqcnt > 1) && (pptr->conn_res.QUEUE_ptr == OTHERQ(q))) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		snd_error_ack(q, mp, TBADF, 0);
		return;
	}
	if (msgdsize(mp) > CDATA_SIZE) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		snd_error_ack(q, mp, TBADDATA, 0);
		return;
	}
	if ((okptr = copymsg(mp)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  	"tivc_cres 17: copymsg failed\n");
		snd_error_ack(q, mp, TSYSERR, ENOMEM);
		return;
	}
	if ((tiptr->ti_seqcnt==1) && (pptr->conn_res.QUEUE_ptr == OTHERQ(q))) 
		tiptr->ti_state = NEXTSTATE(TE_OK_ACK2,tiptr->ti_state);
	else
		if (tiptr->ti_seqcnt == 1)
			tiptr->ti_state = NEXTSTATE(TE_OK_ACK3,tiptr->ti_state);
		else
			tiptr->ti_state = NEXTSTATE(TE_OK_ACK4,tiptr->ti_state);
	snd_ok_ack(q, okptr);
	do_cres(q, mp);
	return;
}

tivc_dreq(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tivc *tiptr;
	mblk_t *okptr;
	struct tiseq *seq;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tivc *)q->q_ptr;

	if (mp->b_wptr - mp->b_rptr < sizeof(struct T_discon_req)) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  	"tivc_dreq 18: discon_req message incorrect size\n");
		snd_error_ack(q, mp, TSYSERR, ENOMEM);
		return;
	}

	if (tiptr->ti_seqcnt) {
		for (seq = tiptr->ti_seq; seq < &tiptr->ti_seq[MAX_CONN_IND]; seq++) {
			if (pptr->discon_req.SEQ_number == seq->seqno) {
				seq->used = FALSE;
				break;
			}
		}
		if (seq > &tiptr->ti_seq[MAX_CONN_IND -1]) {
			tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
			snd_error_ack(q, mp, TBADSEQ, 0);
			return;
		}
	}
	if (msgdsize(mp) > DDATA_SIZE) {
		tiptr->ti_state = NEXTSTATE(TE_ERROR_ACK,tiptr->ti_state);
		snd_error_ack(q, mp, TBADDATA, 0);
		return;
	}
	if ((okptr = copymsg(mp)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  	"tivc_dreq 19: copymsg failed\n");
		snd_error_ack(q, mp, TSYSERR, ENOMEM);
		return;
	}
	if (tiptr->ti_seqcnt <= 1) {
		if (tiptr->ti_seqcnt == 0)
			tiptr->ti_state = NEXTSTATE(TE_OK_ACK1, tiptr->ti_state);
		else
			tiptr->ti_state = NEXTSTATE(TE_OK_ACK2, tiptr->ti_state);
		if (!snd_flushrw(q)) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  		"tivc_dreq 20: snd_flushrw failed\n");
			snd_error_ack(q, mp, TSYSERR, EIO);
			return;
		}
	} else
		tiptr->ti_state = NEXTSTATE(TE_OK_ACK4, tiptr->ti_state);
	snd_ok_ack(q, okptr);
	do_dreq(q, mp);
	return;
}


do_data(q, mp, type)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register struct ti_tivc *tiptr;
	register struct ti_tivc *rtiptr;
	queue_t *r_rq;
	int more = 0;

	tiptr = (struct ti_tivc *)q->q_ptr;
	switch (mp->b_datap->db_type) {
	case M_DATA:
		break;
	case M_PROTO:
	case M_PCPROTO:
		if (type == TE_ORDREL_REQ) {
			if ((mp->b_wptr - mp->b_rptr) < sizeof(struct T_ordrel_req)) {
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  			"tivc do_data 21: ordrel_req message incorrect size\n");
				tivc_sendfatal(q, mp);
				return(0);
			}
		} else {
			if ((mp->b_wptr - mp->b_rptr) < sizeof(struct T_data_req)) {
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		  			"tivc do_data 22: data message incorrect size\n");
				tivc_sendfatal(q, mp);
				return(0);
			}
		}
		pptr = (union T_primitives *)mp->b_rptr;
		if (type != TE_ORDREL_REQ)
			more = pptr->data_req.MORE_flag;
		break;
	}

	if ((r_rq = tiptr->ti_remoteq) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 	"tivc do_data 23: remoteq ptr == NULL\n");
		tivc_sendfatal(q, mp);
		return(0);
	}
	rtiptr = (struct ti_tivc *)r_rq->q_ptr;
	if (rtiptr->ti_conseq != tiptr->ti_conseq) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 	"tivc do_data 24: remote conseq != local conseq\n");
		tivc_sendfatal(q, mp);
		return(0);
	}
	if (!canput(r_rq->q_next)) {
		putbq(q, mp);
		return(0);
	}

	switch(type) {
	case TE_ORDREL_REQ:
		tiptr->ti_tsdu = 0;
		tiptr->ti_etsdu = 0;
		break;
	case TE_DATA_REQ:
		tiptr->ti_tsdu += msgdsize(mp);
		if (tiptr->ti_tsdu > TSDU_SIZE) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 		"tivc do_data 25: TSDU size exceeded\n");
			tivc_sendfatal(q, mp);
			return(0);
		}
		if (!more)
			tiptr->ti_tsdu = 0;
		break;
	case TE_EXDATA_REQ:
		tiptr->ti_etsdu += msgdsize(mp);
		if (tiptr->ti_etsdu > ETSDU_SIZE) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 		"tivc do_data 26: ETSDU size exceeded\n");
			tivc_sendfatal(q, mp);
			return(0);
		}
		if (!more)
			tiptr->ti_etsdu = 0;
		break;
	}

	switch (mp->b_datap->db_type) {
	case M_DATA:
		if ((rtiptr->ti_state = NEXTSTATE(TE_DATA_IND,rtiptr->ti_state)) == NR) {
			STRLOG(TIVC_ID, TI_VC_NUM(WR(r_rq)->q_ptr),0, SL_TRACE,
		 		"tivc do_data 32: data_ind places remote interface out of state\n");
			tivc_sendfatal(WR(r_rq), mp);
			return(0);
		}
		if ((tiptr->ti_state = NEXTSTATE(TE_DATA_IND,tiptr->ti_state)) == NR) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 		"tivc do_data 33: data_ind places local interface out of state\n");
			tivc_sendfatal(q, mp);
			return(0);
		}
		break;
	case M_PROTO:
	case M_PCPROTO:
		pptr = (union T_primitives *)mp->b_rptr;
		switch (type) {
		case TE_ORDREL_REQ:
			if ((rtiptr->ti_state = NEXTSTATE(TE_ORDREL_IND,rtiptr->ti_state)) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(WR(r_rq)->q_ptr),0, SL_TRACE,
		 			"tivc do_data 34: ordrel_ind places remote interface out of state\n");
				tivc_sendfatal(WR(r_rq), mp);
				return(0);
			}
			if ((tiptr->ti_state = NEXTSTATE(TE_ORDREL_REQ,tiptr->ti_state)) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 			"tivc do_data 35: ordrel_req places local interface out of state\n");
				tivc_sendfatal(q, mp);
				return(0);
			}
			pptr->type = T_ORDREL_IND;
			mp->b_datap->db_type = M_PROTO;
			if (rtiptr->ti_state == TS_IDLE) {
				rtiptr->ti_remoteq = NULL;
				rtiptr->ti_conseq = 0;
				rtiptr->ti_outq = NULL;
				tiptr->ti_outq = NULL;
				tiptr->ti_remoteq = NULL;
				tiptr->ti_conseq = 0;
			}
			break;
		case TE_DATA_REQ:
			if ((rtiptr->ti_state = NEXTSTATE(TE_DATA_IND,rtiptr->ti_state)) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(WR(r_rq)->q_ptr),0, SL_TRACE,
		 			"tivc do_data 36: data_ind places remote interface out of state\n");
				tivc_sendfatal(WR(r_rq), mp);
				return(0);
			}
			if ((tiptr->ti_state = NEXTSTATE(TE_DATA_REQ,tiptr->ti_state)) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 			"tivc do_data 37: data_ind places local interface out of state\n");
				tivc_sendfatal(q, mp);
				return(0);
			}
			pptr->type = T_DATA_IND;
			mp->b_datap->db_type = M_PROTO;
			break;
		case TE_EXDATA_REQ:
			if ((rtiptr->ti_state = NEXTSTATE(TE_EXDATA_IND,rtiptr->ti_state)) == NR) {
				STRLOG(TIVC_ID, TI_VC_NUM(WR(r_rq)->q_ptr),0, SL_TRACE,
		 			"tivc do_data 38: exdata_ind places remote interface out of state\n");
				tivc_sendfatal(WR(r_rq), mp);
				return(0);
			}
#ifndef	OLDBUG
			if ((tiptr->ti_state = NEXTSTATE(TE_EXDATA_REQ,tiptr->ti_state)) == NR) {
#else
			if ((tiptr->ti_state = NEXTSTATE(TE_EXDATA_IND,tiptr->ti_state)) == NR) {
#endif
				STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 			"tivc do_data 39: exdata_ind places local interface out of state\n");
				tivc_sendfatal(q, mp);
				return(0);
			}
			pptr->type = T_EXDATA_IND;
			mp->b_datap->db_type = M_PROTO;
			break;
		}
		break;
	}
	(*r_rq->q_next->q_qinfo->qi_putp)(r_rq->q_next, mp);
	return(1);
}

do_creq(q, mp)
queue_t *q;
mblk_t *mp;
{
	register struct ti_tivc *rtiptr;
	register struct ti_tivc *tiptr;
	register union T_primitives *pptr;
	register union T_primitives *dpptr;
	register union T_primitives *cpptr;
	unsigned long *addr;
	unsigned long *caddr;
	unsigned long *opts;
	unsigned long *copts;
	register mblk_t *cp;
	register mblk_t *dp;
	struct tiseq *seq;

	++seq_counter;
	tiptr = (struct ti_tivc *)q->q_ptr;
	pptr = (union T_primitives *)mp->b_rptr;

	addr = (unsigned long *)(mp->b_rptr + pptr->conn_req.DEST_offset);
	opts = (unsigned long *)(mp->b_rptr + pptr->conn_req.OPT_offset);

	for (rtiptr = ti_tivc; rtiptr < &ti_tivc[tivc_cnt]; rtiptr++)
		if ((rtiptr->ti_addr == *addr) &&
		    (rtiptr->ti_qlen > 0) &&
		    (rtiptr->ti_seqcnt < rtiptr->ti_qlen) &&
		    ((rtiptr->ti_state == TS_IDLE) ||
		     (rtiptr->ti_state == TS_WRES_CIND)))
			break;

	if (rtiptr > &ti_tivc[tivc_cnt -1]) {
		if ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL) {
			STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
		 		"tivc do_creq 40: couldn't alloc buffer for discon_ind\n");
			tivc_sendfatal(q, mp);
			return;
		}
		dpptr = (union T_primitives *)dp->b_rptr;
		dp->b_wptr = dp->b_rptr + sizeof(struct T_discon_ind);
		dpptr->type = T_DISCON_REQ;
		dpptr->discon_ind.SEQ_number = -1;
		dpptr->discon_ind.DISCON_reason = UNREACHABLE;
		tiptr->ti_state = NEXTSTATE(TE_DISCON_IND1, tiptr->ti_state);
		dp->b_datap->db_type = M_PROTO;
		qreply(q, dp);
		freemsg(mp);
		return;
	}

	if ((cp = allocb(sizeof(struct T_conn_ind) + 2*sizeof(long), BPRI_MED)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
			"tivc do_creq 41: couldn't alloc buffer for conn_ind\n");
		tivc_sendfatal(q, mp);
		return;
	}
	cpptr = (union T_primitives *)cp->b_rptr;
	cp->b_wptr = cp->b_rptr + sizeof(struct T_conn_ind) + 2*sizeof(long);
	cp->b_datap->db_type = M_PROTO;
	cpptr->type = T_CONN_IND;
	cpptr->conn_ind.SRC_offset = sizeof(struct T_conn_ind);
	cpptr->conn_ind.SRC_length = sizeof(long);
	cpptr->conn_ind.SEQ_number = seq_counter;
	caddr = (unsigned long *)(cp->b_rptr + cpptr->conn_ind.SRC_offset);
	*caddr = tiptr->ti_addr;
	if (pptr->conn_req.OPT_length != 0) {
		cpptr->conn_ind.OPT_offset = sizeof(struct T_conn_ind) + sizeof(long);
		cpptr->conn_ind.OPT_length = sizeof(long);
		copts = (unsigned long *)(cp->b_rptr + cpptr->conn_ind.OPT_offset);
		*copts = *opts;
	} else {
		cpptr->conn_ind.OPT_offset = 0;
		cpptr->conn_ind.OPT_length = 0;
	}
	cp->b_cont = mp->b_cont;
	mp->b_cont = NULL;
	freemsg(mp);

	for (seq = rtiptr->ti_seq; seq < &rtiptr->ti_seq[MAX_CONN_IND]; seq++)
		if (seq->used == FALSE) {
			seq->seqno = seq_counter;
			seq->used = TRUE;
			break;
		}
	rtiptr->ti_seqcnt++;
	tiptr->ti_conseq = seq_counter;
	tiptr->ti_outq = rtiptr->ti_rdq;
	rtiptr->ti_state = NEXTSTATE(TE_CONN_IND, rtiptr->ti_state);
	(*rtiptr->ti_rdq->q_next->q_qinfo->qi_putp)(rtiptr->ti_rdq->q_next, cp);
	return;
}

do_cres(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register union T_primitives *dpptr;
	register union T_primitives *cpptr;
	register struct ti_tivc *restiptr;
	register struct ti_tivc *tiptr;
	register struct ti_tivc *remtiptr;
	register mblk_t *dp;
	register mblk_t *cp;
	unsigned long *caddr;
	unsigned long *copts;

	tiptr = (struct ti_tivc *)q->q_ptr;
	pptr = (union T_primitives *)mp->b_rptr;

	if (pptr->conn_res.QUEUE_ptr != tiptr->ti_rdq) {
		for (restiptr = ti_tivc; restiptr < &ti_tivc[tivc_cnt]; restiptr++)
			if (restiptr->ti_rdq == pptr->conn_res.QUEUE_ptr)
				break;
		restiptr->ti_state = NEXTSTATE(TE_PASS_CONN, restiptr->ti_state);
	} else {
		restiptr = tiptr;
	}
	
	tiptr->ti_seqcnt--;
	for (remtiptr = ti_tivc; remtiptr < &ti_tivc[tivc_cnt]; remtiptr++)
		if (remtiptr->ti_conseq == pptr->conn_res.SEQ_number)
			break;
	if ((remtiptr > &ti_tivc[tivc_cnt-1]) || (remtiptr->ti_state != TS_WCON_CREQ)) {
		if (!snd_flushrw(restiptr->ti_rdq) || ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL)) {
			STRLOG(TIVC_ID, TI_VC_NUM(restiptr->ti_rdq->q_ptr),0, SL_TRACE,
				"tivc do_cres 42: couldn't alloc buffer for discon_ind\n");
			tivc_sendfatal(restiptr->ti_rdq, mp);
			return;
		}
		dpptr = (union T_primitives *)dp->b_rptr;
		dp->b_wptr = dp->b_rptr + sizeof(struct T_discon_ind);
		dp->b_datap->db_type = M_PROTO;
		dpptr->type = T_DISCON_REQ;
		dpptr->discon_ind.SEQ_number = -1;
		dpptr->discon_ind.DISCON_reason = REMOTEBADSTATE;
		restiptr->ti_state = NEXTSTATE(TE_DISCON_IND1, restiptr->ti_state);
		(*restiptr->ti_rdq->q_next->q_qinfo->qi_putp)(restiptr->ti_rdq->q_next, dp);
		freemsg(mp);
		return;
	}
	if ((cp = allocb(sizeof(struct T_conn_con) + 2*sizeof(long), BPRI_MED)) == NULL) {
		STRLOG(TIVC_ID, TI_VC_NUM(q->q_ptr),0, SL_TRACE,
			"tivc do_cres 43: couldn't alloc buffer for conn_con\n");
		tivc_sendfatal(q, mp);
		return;
	}
	cp->b_datap->db_type = M_PROTO;
	cpptr = (union T_primitives *)cp->b_rptr;
	cp->b_wptr = cp->b_rptr + sizeof(struct T_conn_con) + 2*sizeof(long);
	cpptr->type = T_CONN_CON;
	cpptr->conn_con.RES_offset = sizeof(struct T_conn_con);
	cpptr->conn_con.RES_length = sizeof(long);
	caddr = (unsigned long *)(cp->b_rptr + cpptr->conn_con.RES_offset);
	*caddr = restiptr->ti_addr;
	if (pptr->conn_res.OPT_length != 0) {
		cpptr->conn_con.OPT_offset = sizeof(struct T_conn_con) + sizeof(long);
		cpptr->conn_con.OPT_length = sizeof(long);
		copts = (unsigned long *)(cp->b_rptr + cpptr->conn_con.OPT_offset);
		*copts = *(unsigned long *)(mp->b_rptr +pptr->conn_res.OPT_offset);
	} else {
		cpptr->conn_con.OPT_offset = 0;
		cpptr->conn_con.OPT_length = 0;
	}
	cp->b_cont = mp->b_cont;
	mp->b_cont = NULL;
	freemsg(mp);
	restiptr->ti_conseq = remtiptr->ti_conseq;
	remtiptr->ti_remoteq = restiptr->ti_rdq;
	restiptr->ti_remoteq = remtiptr->ti_rdq;
	remtiptr->ti_state = NEXTSTATE(TE_CONN_CON, remtiptr->ti_state);
	(*remtiptr->ti_rdq->q_next->q_qinfo->qi_putp)(remtiptr->ti_rdq->q_next, cp);
	return;
}



do_dreq(q, mp)
queue_t *q;
register mblk_t *mp;
{
	register union T_primitives *pptr;
	register union T_primitives *dpptr;
	register struct ti_tivc *tiptr;
	register struct ti_tivc *rtiptr;
	register mblk_t *dp;
	struct tiseq *seq;

	pptr = (union T_primitives *)mp->b_rptr;
	tiptr = (struct ti_tivc *)q->q_ptr;

	if (tiptr->ti_seqcnt == 0) {
		if (tiptr->ti_remoteq == 0) {
			rtiptr = (struct ti_tivc *)tiptr->ti_outq->q_ptr;
			if ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL) {
				STRLOG(TIVC_ID, TI_VC_NUM(tiptr->ti_outq->q_ptr),0, SL_TRACE,
					"tivc do_dreq 50: couldn't alloc buffer for discon_ind\n");
				tivc_sendfatal(tiptr->ti_outq, mp);
				return;
			}
			for (seq = rtiptr->ti_seq; seq < &rtiptr->ti_seq[MAX_CONN_IND]; seq++)
				if (seq->seqno == tiptr->ti_conseq)
					seq->used = FALSE;
			dpptr = (union T_primitives *)dp->b_rptr;
			dpptr->discon_ind.SEQ_number = tiptr->ti_conseq;
			if (rtiptr->ti_seqcnt > 1)
				rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND3, rtiptr->ti_state);
			else
				rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND2, rtiptr->ti_state);
			rtiptr->ti_seqcnt--;
		} else {
			rtiptr = (struct ti_tivc *)tiptr->ti_remoteq->q_ptr;
			if (!snd_flushrw(tiptr->ti_remoteq) || ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL)) {
				STRLOG(TIVC_ID, TI_VC_NUM(tiptr->ti_remoteq->q_ptr),0, SL_TRACE,
					"tivc do_dreq 51: couldn't alloc buffer for discon_ind\n");
				tivc_sendfatal(tiptr->ti_remoteq, mp);
				return;
			}
			flushq(WR(rtiptr->ti_rdq), 1);
			flushq(q, 1);
			dpptr = (union T_primitives *)dp->b_rptr;
			dpptr->discon_ind.SEQ_number = -1;
			rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND1, rtiptr->ti_state);
			rtiptr->ti_remoteq = NULL;
			rtiptr->ti_outq = NULL;
			rtiptr->ti_conseq = 0;
		}
		tiptr->ti_remoteq = NULL;
		tiptr->ti_outq = NULL;
		tiptr->ti_conseq = 0;
	} else {
		tiptr->ti_seqcnt--;
		for (rtiptr = ti_tivc; rtiptr < &ti_tivc[tivc_cnt]; rtiptr++)
			if (rtiptr->ti_conseq == pptr->discon_req.SEQ_number)
				break;
		if ((dp = allocb(sizeof(struct T_discon_ind), BPRI_HI)) == NULL) {
			STRLOG(TIVC_ID, TI_VC_NUM(rtiptr),0, SL_TRACE,
				"tivc do_dreq 55: couldn't alloc buffer for discon_ind\n");
			tivc_sendfatal(rtiptr->ti_rdq, mp);
			return;
		}
		dpptr = (union T_primitives *)dp->b_rptr;
		dpptr->discon_ind.SEQ_number = -1;
		rtiptr->ti_state = NEXTSTATE(TE_DISCON_IND1, rtiptr->ti_state);
		rtiptr->ti_remoteq = NULL;
		rtiptr->ti_outq = NULL;
		rtiptr->ti_conseq = 0;
	}
	dp->b_datap->db_type = M_PROTO;
	dp->b_cont = mp->b_cont;
	mp->b_cont = NULL;
	freemsg(mp);
	dp->b_wptr = dp->b_rptr + sizeof(struct T_discon_ind);
	dpptr->discon_ind.PRIM_type = T_DISCON_IND;
	dpptr->discon_ind.DISCON_reason = USERINITIATED;
	(*rtiptr->ti_rdq->q_next->q_qinfo->qi_putp)(rtiptr->ti_rdq->q_next, dp);
	return;
}
