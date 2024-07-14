#ident "@(#)lmodb.c	1.1"
/*
 * Bottom loopback module (between driver and top module)
 */

#include "sys/types.h"
#include "sys/param.h"
#include "sys/errno.h"
#include "sys/stropts.h"
#include "sys/stream.h"

#define I_MODCMD 30

int lmodbopen(), lmodbclose(), lmodbput();
static struct module_info lmodb_info = {1003, "lmodb", 0, 256, 512, 256 };
static struct qinit lmodbrinit = 
		{ lmodbput, NULL, lmodbopen, lmodbclose, NULL, &lmodb_info, NULL };
static struct qinit lmodbwinit = 
		{ lmodbput, NULL, NULL, NULL, NULL, &lmodb_info, NULL};
struct streamtab lmbinfo = { &lmodbrinit, &lmodbwinit };

lmodbopen(q, dev, flag, sflag)
queue_t *q;
{
	q->q_ptr = NULL;
	WR(q)->q_ptr = NULL;
	return(0);
}

lmodbclose(q)
queue_t *q;
{
	flushq(WR(q), 1);
}

/*
 * Use same put procedure for write and read queues.
 */
lmodbput(q, bp)
queue_t *q;
mblk_t *bp;
{
	mblk_t *tmp;
	struct iocblk *iocbp;
	int i, n;


	switch(bp->b_datap->db_type) {
		case M_IOCTL:
			iocbp = (struct iocblk *)bp->b_rptr;
			switch(iocbp->ioc_cmd) {
			  case I_MODCMD:
				/*
				 * This same ioctl is in loopback
				 * driver, so return 1 here, while
				 * driver returns 2 to distiguish.
				 */
				iocbp->ioc_rval = 1;
				bp->b_datap->db_type = M_IOCACK;
				qreply(q, bp);
				break;
			  default:
				/*
				 * Just pass other ioctl's on to
				 * next module.
				 */
				if (canput(q->q_next))
					(*q->q_next->q_qinfo->qi_putp)(q->q_next, bp);
				else {
					freemsg(bp);
				}
				break;
			}
			break;
		case M_FLUSH:
			if (q->q_flag&QREADR)
				if (*bp->b_rptr & FLUSHR) flushq(q,0);
				else ;
			else if (*bp->b_rptr & FLUSHW) flushq(q,0);
			(*q->q_next->q_qinfo->qi_putp)(q->q_next,bp);
			break;
		case M_DATA:
		default:
			if((q->q_next->q_flag & QFULL) == 0)
				(*q->q_next->q_qinfo->qi_putp)(q->q_next, bp);
			else {
				freemsg(bp);
			}
			break;
	}
}
/*	<@(#)lmodb.c	5.1>	*/
