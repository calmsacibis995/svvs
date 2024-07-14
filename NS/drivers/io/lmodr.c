#ident "@(#)lmodr.c	1.1"
/*
 * Bottom loopback module (between driver and top module)
 * This module just returns an error for testing purposes.
 * It returns OPENFAIL.
 */

#include "sys/types.h"
#include "sys/param.h"
#include "sys/errno.h"
#include "sys/stropts.h"
#include "sys/stream.h"

#define I_MODCMD 30

int lmodropen(), lmodrclose(), lmodrput();
static struct module_info lmodr_info = {1003, "lmodr", 0, 256, 512, 256 };
static struct qinit lmodrrinit = 
		{ lmodrput, NULL, lmodropen, lmodrclose, NULL, &lmodr_info, NULL };
static struct qinit lmodrwinit = 
		{ lmodrput, NULL, NULL, NULL, NULL, &lmodr_info, NULL};
struct streamtab lmrinfo = { &lmodrrinit, &lmodrwinit };

lmodropen(q, dev, flag, sflag)
queue_t *q;
{
	q->q_ptr = NULL;
	WR(q)->q_ptr = NULL;
	return(OPENFAIL);
}

lmodrclose(q)
queue_t *q;
{
	flushq(WR(q), 1);
}

/*
 * Use same put procedure for write and read queues.
 */
lmodrput(q, bp)
queue_t *q;
mblk_t *bp;
{
	mblk_t *tmp;
	struct iocblk *iocbp;
	int i, n;

	putnext(q,bp);
}
/*	<@(#)lmodr.c	5.1>	*/
