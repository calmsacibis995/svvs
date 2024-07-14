#ident "@(#)lmodt.c	1.1"
/*
 * Top Loopback Module (between stream head and bottom module)
 */

#include "sys/types.h"
#include "sys/param.h"
#include "sys/errno.h"
#include "sys/stropts.h"
#include "sys/stream.h"

int lmodtopen(), lmodtclose(), lmodtput();
static struct module_info lmodtm_info = {1004, "lmodt", 0, 256, 512, 256 };
static struct qinit lmodtrinit = 
		{ lmodtput, NULL, lmodtopen, lmodtclose, NULL, &lmodtm_info, NULL };
static struct qinit lmodtwinit = 
		{ lmodtput, NULL, NULL, NULL, NULL, &lmodtm_info, NULL };
struct streamtab lmtinfo = { &lmodtrinit, &lmodtwinit };

lmodtopen(q, dev, flag, sflag)
queue_t *q;
{
	q->q_ptr = NULL;
	WR(q)->q_ptr = NULL;
	return(0);
}

lmodtclose(q)
queue_t *q;
{
	flushq(WR(q), 1);
}

lmodtput(q, bp)
queue_t *q;
mblk_t *bp;
{
	/*
	 * Pass all messages on without processing them, except
	 * for M_FLUSH, which must be processed.
	 */
	switch(bp->b_datap->db_type) {
		case M_FLUSH:
			if (q->q_flag&QREADR)
				if (*bp->b_rptr & FLUSHR) flushq(q,0);
				else ;
			else if (*bp->b_rptr & FLUSHW) flushq(q,0);
			(*q->q_next->q_qinfo->qi_putp)(q->q_next,bp);
			break;
		case M_DATA:
		default:
			if (canput(q->q_next))
				(*q->q_next->q_qinfo->qi_putp)(q->q_next, bp);
			else {
				freemsg(bp);
			}
			break;
	}
}
/*	<@(#)lmodt.c	5.1>	*/
