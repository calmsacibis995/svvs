#ident "@(#)lo.c	1.3"
/*
 * Loopback driver
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysmacros.h>
#include <sys/errno.h>
#include <sys/stropts.h>
#include <sys/stream.h>
#include <lo.h>
#include <sys/sema.h>
#include <sys/strlog.h>
#include <sys/log.h>

int nodev(), loopen(), loclose(), loput(), losrv();
static struct module_info lom_info = {40, "lo", 0, 256, 512, 256};
static struct qinit lorinit = { NULL, losrv, loopen, loclose, NULL, &lom_info, NULL};
static struct qinit lowinit = { putq, losrv, loopen, loclose, NULL, &lom_info, NULL};
struct streamtab loinfo = { &lorinit, &lowinit, NULL, NULL };

static dblk_t * holdb[NCLASS];	/* temporary holding array for free list */
extern dblk_t *dbfreelist[];
extern struct lo lo_lo[];
extern locnt;
extern getclass();

EXTERN_LOCK(qblksema);		/* block free list semaphore */


loopen(q, dev, flag, sflag)
queue_t *q;
{
	struct lo *lp;
	mblk_t *bp;
	struct stroptions *sop;
	int i;


	dev = minor(dev);
	if (sflag == CLONEOPEN) {
		for (dev=0; dev<locnt; dev++) 
			if (!(lo_lo[dev].lo_state & LOOPEN)) break;
	}
	if ((dev < 0) || (dev >= locnt)) return(OPENFAIL);
	lp = &lo_lo[dev];
	if (lp->lo_state & LOFAIL) {
		/* clear fail flag so it can be reopened later */
		lp->lo_state &= ~LOFAIL;
		return(OPENFAIL);
	}

	if (!(lp->lo_state & LOOPEN)) {
		for(i=0;i<NCLASS;holdb[i++]=NULL);
		lp->lo_state = LOOPEN;
		lp->lo_rdq = q;
		q->q_ptr = (caddr_t)lp;
		WR(q)->q_ptr = (caddr_t)lp;
	}
	else if (q != lp->lo_rdq) return(OPENFAIL); /* only one stream at a time! */

	/*
	 * Set up the correct stream head flow control parameters
	 */
	if (bp = allocb(sizeof(struct stroptions))) {
		bp->b_datap->db_type = M_SETOPTS;
		bp->b_wptr += sizeof(struct stroptions);
		sop = (struct stroptions *)bp->b_rptr;
		sop->so_flags = SO_HIWAT | SO_LOWAT;
		sop->so_hiwat = 512;
		sop->so_lowat = 256;
		putnext(q, bp);
		return(dev);
	}
	return(OPENFAIL);
}

loclose(q)
queue_t *q;
{
	int i;
	dblk_t * dp;
	register s;

	((struct lo *)(q->q_ptr))->lo_state &= ~(LOOPEN | LOWOFF);
	((struct lo *)(q->q_ptr))->lo_rdq = NULL;
	flushq(WR(q), 1);
	q->q_ptr = NULL;
	for (i=0;i<NCLASS;i++) 
		if (holdb[i] != NULL) {
			s = splstr();
			SPSEMA(&qblksema);
			if ((dp=dbfreelist[i])==NULL)
				dbfreelist[i]=holdb[i];
			else {
				while (dp->db_freep!=NULL) dp=dp->db_freep;
				dp->db_freep = holdb[i];
			}
			SVSEMA(&qblksema);
			splx(s);
			holdb[i]=NULL;
		} 
}


/*
 * Service routine takes messages off write queue and
 * sends them back up the read queue, processing them
 * along the way.
 */
losrv(q)
queue_t *q;
{
	mblk_t *bp;

	/* if losrv called from read side set q to write side */
	q = ((q)->q_flag&QREADR ? WR(q) : q); 

	while ((bp = getq(q)) != NULL) {
		/* if upstream queue full, process only priority messages */
		if ((bp->b_datap->db_type) < QPCTL && !canput(RD(q)->q_next) ) {
			putbq(q, bp);
			return;
		}

		switch(bp->b_datap->db_type) {

		  case M_IOCTL:
			loioctl(q, bp);

			if (((struct lo *)(q->q_ptr))->lo_state & LOSLPTEST) 
				return;
			break;

		  case M_DATA:
			/* if testing offset, calculate and place at start */
			/* of data message. */
			if (((struct lo *)(q->q_ptr))->lo_state & LOWOFF) 
				(*(short *)(bp->b_rptr)) =
					 (short)(bp->b_rptr - bp->b_datap->db_base);
			/* flow through */

		  case M_PROTO:
		  case M_PCPROTO:
			qreply(q, bp);
			break;

		  case M_CTL:
			freemsg(bp);
			break;

		  case M_FLUSH:
			if (*bp->b_rptr & FLUSHW) {
				flushq(q, FLUSHALL);
				*bp->b_rptr &= ~FLUSHW;
			}
			if (*bp->b_rptr & FLUSHR) 
				qreply(q,bp);
			else freemsg(bp);
			break;

		  default:
			freemsg(bp);
			break;
		}
	}
}


loioctl(q, bp)
queue_t *q;
mblk_t *bp;
{
	register s;

	mblk_t *tmp;
	dblk_t *dp;
	struct iocblk *iocbp;
	int i, n;
	struct stroptions *so;
	static int	saveminval = 0;
	static	struct strevent 	*savesefree = 0;
	extern struct strevent *sefreelist;
	extern	struct	linkblk	linkblk[];
	static struct linkblk	*savelink[100];

	/* test for no offset in ioctl */
	if ((((struct lo *)(q->q_ptr))->lo_state & LOWOFF)
	    && (bp->b_rptr != bp->b_datap->db_base)) {
		bp->b_datap->db_type = M_IOCNAK;
		qreply(q, bp);
		((struct lo *)(q->q_ptr))->lo_state &= ~LOWOFF;
		if ((bp=allocb(sizeof(struct stroptions))) == NULL) {
			printf("losrv couldn't allocate data block\n");
			return;
		}
		bp->b_datap->db_type = M_SETOPTS;
		so = (struct stroptions *)bp->b_rptr;
		bp->b_wptr += sizeof(struct stroptions);
		so->so_flags = SO_WROFF;
		so->so_wroff = 0;
		qreply(q, bp);
		return;
	}
			

	/*
	 * This is the start of the main block of code.  Each particular
	 * ioctl has a special function for testing out the streams 
	 * mechanism.
	 */

	iocbp = (struct iocblk *)bp->b_rptr;

	switch(iocbp->ioc_cmd) {

	/*
	 * I_SETRANGE and I_UNSETRANGE were added to allow RANGE tests.
	 */

	  case I_SETRANGE:
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		saveminval = q->q_minpsz;
		q->q_minpsz = 64;
		return;

	  case I_UNSETRANGE:
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		q->q_minpsz = saveminval;
		return;


	  case I_SLPTEST:
		if((((struct lo *)(q->q_ptr))->lo_state&LOSLPTEST))
			{
			((struct lo *)(q->q_ptr))->lo_state &= (~LOSLPTEST);
			bp->b_datap->db_type = M_IOCACK;
			qreply(q, bp);
			return;
			}
		((struct lo *)(q->q_ptr))->lo_state |= LOSLPTEST;
		putbq(q,bp);
		timeout(qenable,q,10*HZ);
		return;


	  case I_NOARG:
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	  case I_INTARG:
		/*
		 * Send integer argument back as return
		 * value
		 */
		if (bp->b_cont == NULL) {
			freemsg(bp);
			return;
		}
		iocbp->ioc_rval = *((int *)bp->b_cont->b_rptr);
		tmp = unlinkb(bp);
		freeb(tmp);
		iocbp->ioc_count = 0;
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;


	  case I_STRTEST:
		/*
		 * sleep 2 seconds for delay
		 */

		/* Wait a couple of seconds. */
		for (i=0; i<10000; i++) {
			n = i;
		}
			iocbp->ioc_rval = *((int *)bp->b_cont->b_rptr);
			tmp = unlinkb(bp);
			freeb(tmp);
			iocbp->ioc_count = 0;
			bp->b_datap->db_type = M_IOCACK;
			qreply(q, bp);
			return;

	  case I_TIMEOUT:
		/*
		 * sleep 45 seconds for delay
		 */
		if((((struct lo *)(q->q_ptr))->lo_state&LOSLPTEST))
			{
			((struct lo *)(q->q_ptr))->lo_state &= (~LOSLPTEST);
			bp->b_datap->db_type = M_IOCACK;
			qreply(q, bp);
			return;
			}
		((struct lo *)(q->q_ptr))->lo_state |= LOSLPTEST;
		putbq(q,bp);
		timeout(qenable,q,45*HZ);
		return;

	  case I_QPTR:
		/*
		 * Send queue pointer argument back as return
		 * value
		 */
		if (bp->b_cont == NULL) {
			freemsg(bp);
			return;
			}
		iocbp->ioc_rval = (int) RD(q);
		tmp = unlinkb(bp);
		freeb(tmp);
		iocbp->ioc_count = 0;
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	case I_GRABSEVENT:
		s = splstr();
		savesefree = sefreelist;
		sefreelist = NULL;
		splx(s);
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	case I_RELSEVENT:
		s = splstr();
		sefreelist = savesefree;
		splx(s);
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;


	case I_GRABLINK:
		s = splstr();
		for(i=0; i < nmuxlink; i++)
			{
			savelink[i] = (struct linkblk *)linkblk[i].l_qtop;
			linkblk[i].l_qtop = (queue_t *)1;
			}
		splx(s);
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	case I_RELLINK:
		s = splstr();
		for(i=0; i < nmuxlink; i++)
			{
			linkblk[i].l_qtop = (queue_t *)savelink[i];
			}
		splx(s);
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;


	  case I_DDARG:
		/*
		 * Take the data portion of M_IOCTL msg
		 * and turn it into a M_DATA msg and
		 * send back upstream for read verification.
		 */
		tmp = unlinkb(bp);
		bp->b_datap->db_type = M_IOCACK;
		iocbp->ioc_count = 0;
		qreply(q, bp);
		tmp->b_datap->db_type = M_DATA;
		qreply(q, tmp);
		return;

	  case I_UDARG:
		/*
		 * Send 2 blocks of different data upstream
		 * in M_IOCACK message.
		 */
		n = IOCTLSZ/2;
		if ((tmp = allocb(n)) == NULL) {
			printf("losrv couldn't allocate data block\n");
			return;
		}
		while (n > 0)
			for (i = 0; i < 128; i++) {
				*tmp->b_wptr = i;
				tmp->b_wptr++;
				if (--n <= 0)
					break;
			}
		bp->b_cont = tmp;
		n = IOCTLSZ/2;
		if ((tmp = allocb(n)) == NULL) {
			printf("losrv couldn't allocate data block\n");
			return;
		}
		while (n > 0)
			for (i = 128; i > 0; i--) {
				*tmp->b_wptr = i;
				tmp->b_wptr++;
				if (--n <= 0)
					break;
			}
		bp->b_cont->b_cont = tmp;
		bp->b_datap->db_type = M_IOCACK;
		iocbp->ioc_count = IOCTLSZ;
		qreply(q, bp);
		return;

	  case I_ERROR:
		/*
		 * Verify that error return works.
		 */
		iocbp->ioc_error = EPERM;
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	  case I_ERRNAK:
		/*
		 * Send a NAK back with an error value.
		 */
		iocbp->ioc_error = EPERM;
		bp->b_datap->db_type = M_IOCNAK;
		qreply(q, bp);
		return;

	  case I_MODCMD:
		/*
		 * This same command is processed by one of
		 * the modules.  The module sends back 1 in
		 * rval, so we'll return 2 to distinguish
		 * them.  This should never be executed if
		 * module is pushed on.
		 */
		iocbp->ioc_rval = 2;
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	  case I_TIMOUT:
		/* Drop the message on the floor - this is to
		 * ensure that the ioctl times out at the head
		 */
		freemsg(bp);
		return;

	  case I_SETHANG:
		/*
		 * Send ACK followed by M_HANGUP upstream.
		 */
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		putctl(RD(q)->q_next, M_HANGUP);
		return;

	  case I_SETERR:
		/*
		 * Send ACK followed by M_ERROR upstream - value is
		 * sent in second message block.
		 */
		tmp = unlinkb(bp);
		bp->b_datap->db_type = M_IOCACK;
		((struct iocblk *)bp->b_rptr)->ioc_count = 0;
		qreply(q, bp);
		tmp->b_datap->db_type = M_ERROR;
		qreply(q, tmp);
		return;

	  case I_SETOFAIL:
		/*
		 * Used to simulate failure on opens.
		 */
		((struct lo *)(q->q_ptr))->lo_state |= LOFAIL;
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		return;

	  case I_SETWOFF:
	  	/*
	  	 * Used to test write offsets - sets 
	  	 * LOWOFF flag and sends a write 
	  	 * offset request upstream.
		 */
		((struct lo *)(q->q_ptr))->lo_state |= LOWOFF;
		tmp=unlinkb(bp);
		bp->b_datap->db_type = M_IOCACK;
		((struct iocblk *)bp->b_rptr)->ioc_count = 0;
		qreply(q,bp);
		bp = allocb(sizeof(struct stroptions));
		if (!bp) {
			freemsg(tmp);
			return;
		}
		so = (struct stroptions *)bp->b_rptr;
		bp->b_wptr += sizeof(struct stroptions);
		bp->b_datap->db_type = M_SETOPTS;
		so->so_flags = SO_WROFF;
		so->so_wroff = *((short *)tmp->b_rptr);
		qreply(q, bp);
		freemsg(tmp);
		return;

	  case I_CLRWOFF:
	  	/* 
	  	 * Clears LOWOFF flag and sends a 0
	  	 * offset request upstream.
	  	 */
		((struct lo *)(q->q_ptr))->lo_state &= ~LOWOFF;
		bp->b_datap->db_type =M_IOCACK;
		qreply(q, bp);
		if ((bp=allocb(sizeof(struct stroptions))) == NULL){
			printf("losrv couldn't allocate data block\n");
			return;
		}
		bp->b_datap->db_type=M_SETOPTS;
		so = (struct stroptions *)bp->b_rptr;
		bp->b_wptr += sizeof(struct stroptions);
		so->so_flags = SO_WROFF;
		so->so_wroff = 0;
		qreply(q, bp);
		return;

	  case I_SETBIGB:
		bp->b_datap->db_type =M_IOCACK;
		qreply(q, bp);
		q->q_maxpsz=4096;
		return;

	  case I_CLRBIGB:
		bp->b_datap->db_type =M_IOCACK;
		qreply(q, bp);
		q->q_maxpsz=256;
		return;

	/********* The following two ioctls should only be used   */
	/********* when no other streams are active (STANDALONE). */
			
	  case I_GRAB :
	  	/* 
	  	 * Used to grab blocks of a particular size
	  	 * in order to test streams block allocation
	  	 * algorithms
	  	 */
		i = getclass( * ((int *)(bp->b_cont->b_rptr)));
		/*strlog(40, 0, 1, SL_TRACE, "lo I_GRAB: size=%d", *((int *)(bp->b_cont->b_rptr)));*/
		tmp = unlinkb(bp);
		iocbp->ioc_count = 0;
		if (holdb[i] != NULL) {
			bp->b_datap->db_type = M_IOCNAK;
			qreply(q, bp);
			freeb(tmp);
			return;
		}
		s = splstr();
		SPSEMA(&qblksema);
		holdb[i] = dbfreelist[i];
		dbfreelist[i] = NULL;
		SVSEMA(&qblksema);
		splx(s);
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		freeb(tmp);				
		return;

	  case I_FREE :
	  	/* 
	  	 * Frees all of the blocks of a particular size
	  	 * that were grabbed previously via I_GRAB
	  	 */
		i = getclass( * ((int *)(bp->b_cont->b_rptr)));
		tmp = unlinkb(bp);
		iocbp->ioc_count = 0;
		if (holdb[i] == NULL) {
			bp->b_datap->db_type = M_IOCNAK;
			qreply(q,bp);
			freeb(tmp);
			return;
		}
		s = splstr();
		SPSEMA(&qblksema);
		if ((dp=dbfreelist[i])==NULL)
			dbfreelist[i]=holdb[i];
		else {
			while (dp->db_freep!=NULL) dp=dp->db_freep;
			dp->db_freep = holdb[i];
		}
		SVSEMA(&qblksema);
		splx(s);
		holdb[i] = NULL;
		bp->b_datap->db_type = M_IOCACK;
		qreply(q, bp);
		freeb(tmp);
		return;

	  default:
		/*
		 * NAK anything else.
		 */
		bp->b_datap->db_type = M_IOCNAK;
		qreply(q, bp);
		return;
	}
}

/*	<@(#)lo.c	5.2>	*/
