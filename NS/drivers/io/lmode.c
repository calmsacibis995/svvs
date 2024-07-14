#ident "@(#)lmode.c	1.1"
/*
 * Bottom loopback module (between driver and top module)
 * This module just returns an error for testing purposes.
 * It returns it's own error return of 255.
 */

#include "sys/types.h"
#include "sys/param.h"
#include "sys/errno.h"
#include "sys/stropts.h"
#include "sys/stream.h"
#include <sys/dir.h>
#include <sys/signal.h>
#include <sys/psw.h>
#include <sys/pcb.h>
#include <sys/user.h>


#define I_MODCMD 30

int lmodeopen(), lmodeclose(), lmodeput();
static struct module_info lmode_info = {1003, "lmode", 0, 256, 512, 256 };
static struct qinit lmoderinit = 
		{ lmodeput, NULL, lmodeopen, lmodeclose, NULL, &lmode_info, NULL };
static struct qinit lmodewinit = 
		{ lmodeput, NULL, NULL, NULL, NULL, &lmode_info, NULL};
struct streamtab lmeinfo = { &lmoderinit, &lmodewinit };

lmodeopen(q, dev, flag, sflag)
queue_t *q;
{
	q->q_ptr = NULL;
	WR(q)->q_ptr = NULL;
	u.u_error = 255;
	return(OPENFAIL);
}

lmodeclose(q)
queue_t *q;
{
	flushq(WR(q), 1);
}

/*
 * Use same put procedure for write and read queues.
 */
lmodeput(q, bp)
queue_t *q;
mblk_t *bp;
{
	mblk_t *tmp;
	struct iocblk *iocbp;
	int i, n;

	putnext(q,bp);
}
/*	<@(#)lmode.c	5.1>	*/
