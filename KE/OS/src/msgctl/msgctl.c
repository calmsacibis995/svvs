#include <sv_base.h> /*replacement macros */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <svvs.h>
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)msgctl.c	1.14"

# ifdef MSG

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

/*
**	msgctl test
**
**	This test runs setuid root.  
**
**	The test starts by doing a msgget/msgctl(IPC_STAT) to determine
**	the value of msg_qbytes for later use.  This test case and the 
**	last test case, which checks for EINVAL, are hard coded.  All 
**	of the other test cases are table driven.
**	
**	For each test case, a child process is forked to creat
**	a message queue.  Upon its return, a second child is
**	forked to actually do the test.  Upon its return, the
**	parent will check the results of the second process, and
**	if needed, remove the message queue.
**
**	See the table "test[]" for the test cases.
**
**	The creat process does a msgget to creat the messge queue.
**	The message queue should not exist before this, it is an
**	error if it does exist, and the process will terminate.
**	If the message queue is successfully created, the creat process
**	will use msgctl( IPC_SET ) to configure the queue to the 
**	initial values specified in the table.  The initial
**	values were put into (struct msqid_ds *cur) by get_a_test().
**	Note that the creat process must setuid/setgid to cuid/cgid.
**	If the creat process fails, the parent will proceed with the
**	next test.
**
**	The test process has to setuid/setgid to tuid/tgid.  It will
**	attempt one of IPC_STAT, IPC_SET, or IPC_RMID.  The expected 
**	return value and expected errno are checked by this process.
**	If the test process fails, the parent will proceed with cleanup
**	as though it had succeeded.
**
**	If the operation is IPC_STAT, the test process will clr the 
**	buffer passed to msgctl, (struct msqid_ds *buf), and then 
**	check the returned buffer against the initial values, 
**	(struct msqid_ds *cur).  Mismatches are reported as errors.
**
**	If the operation is IPC_SET or IPC_RMID, the test process will 
**	set the buffer to the same values as (struct msqid_ds *nxt) before 
**	passing to msgctl.  Note that for IPC_RMID, the buffer contents
**	are really of no concern.
**
**	The parent process will IPC_STAT the message queue if it is 
**	expected to exist after the test process terminates and the
**	test process was not testing IPC_STAT  (otherwise stat mismatches
**	would most likely be reported twice).  If the expected outcome of 
**	the test was success, the parent process will test that the stat
**	matches (struct msqid_ds *nxt).  If the expected outcome was
**	failure, the parent process will test that the stat matches
**	(struct msqid_ds *cur), to confirm that the queue has not been
**	corrupted by the failed attempt.  Any mismatches are reported as 
**	errors.  If the message queue is expected to exist after the test
**	process terminates, the parent will attempt to remove it with 
**	msgctl(IPC_RMID).  The last operation for each test case is to
**	attempt a msgget without the IPC_CREAT flag.  This should fail.
**	If this does not fail, then the queue was not correctly removed
**	from the system.
**
**	Any time a child process reports an error or exception, the parent
**	should report "child process terminated abnormally".  The parent
**	keeps track of the number of test cases with errors, and will exit
**	after 10 failures.  
**
**	It should be noted again that chk_buf(), which reports problems
**	between the expected and actual stat buffers, is called from the
**	test process if the operation is IPC_STAT. Otherwise, it is 
**	called from the parent process.  All other error messages should
**	give some indication of which process generated the message.
**
**	It is possible that the system under test has set qbytes to 
**	the hardware limit for the type of element in which qbytes is 
**	stored.  If this is true, then:
**
**		(long)qbytes+1l != (long)(qbytes+1)
**
**	This condition will be noted with a warning.  To eliminate the
**	warning, rebuild the system under test with the maximum queue 
**	size set to a smaller value.
**
**
**
*/

#define TESTKEY ((key_t) 23)		/* a most random number	*/

int tuid;			/* owner of test proc	*/
int tgid;			/* group of test proc	*/

int oper;			/* operation for test	*/
int extant;			/* msg is not killed 	*/

struct msqid_ds mybuf;
struct msqid_ds *buf = &mybuf;	/* pass to IPC_STAT	*/
struct msqid_ds mycur;
struct msqid_ds *cur = &mycur;	/* current state, creat	*/
struct msqid_ds mynxt;
struct msqid_ds *nxt = &mynxt;	/* future state, test	*/

int errcnt = 10;		/* abort after 10 errs	*/

int expected;			/* expected return val	*/
int exerrno;			/* expected errno	*/

int qbytes;			/* sys limit msg_qbytes	*/
int kludge;			/* sys lim is h/w limit	*/

ttime	time();
/*
** The following #defines allow the table which follows to fit on 
** an 80 column screen.
**
**  Originally, #define TUID	UID worked for the initialization
**  because UID was a constant.  Now that it is part of a structure,
**  the static initialization is a dummy value, filled in with the actual
**  value at run-time.
*/

#define TUID	55	/*  UID  */
#define TGID	56	/*  GID  */
#define RUID	57	/*  ROOT_UID  */
#define RGID	58	/*  ROOT_GID  */
#define BUID	59	/*  BIN_UID  */
#define BGID	60	/*  BIN_GID  */
#define QBYT	-1
#define QBIG	-2
#define STAT	IPC_STAT
#define SET	IPC_SET
#define RMID	IPC_RMID
#define EAK	-61	/*  EACCES  */
#define EPR	-62	/*  EPERM  */

struct test_case {
	int tu;			/* test user id 	*/
	int tg;			/* test group id	*/
	int mu;			/* msg owner		*/
	int mg;			/* msg group		*/
	int cu;			/* msg creator owner	*/
	int cg;			/* msg creator group	*/
	int qb;			/* msg qbytes		*/
	int mo;			/* msg mode		*/
	int op;			/* operation		*/
	int nm;			/* new mode		*/
	int nu;			/* new owner		*/
	int ng;			/* new group		*/
	int nq;			/* new qbytes 0,1,-1,-2	*/
	int xt;			/* exists after test	*/
	int ex;			/* expected ret val	*/
	int xr;			/* expected errno	*/
} test[] = {
/*-------- ----------------------------- ---- ------------------- -----
 test_proc    msg_q attributes at creat         attr after tst	  retrn 
  uid gid   uid gid  cuid cgid qbyt mode oper nmde nuid ngid newq X R E
---------- ----------------------------- ---- ------------------- -----*/
{TUID,TGID,TUID,BGID,BUID,BGID,QBYT,0400,STAT,0400,TUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,QBYT,0400,STAT,0400,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,TGID,BUID,BGID,QBYT,0040,STAT,0040,BUID,TGID,QBYT,1,0,0},
{TUID,TGID,BUID,BGID,BUID,TGID,QBYT,0040,STAT,0040,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,BGID,BUID,BGID,QBYT,0004,STAT,0004,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,TUID,TGID,TUID,TGID,QBYT,0000,STAT,0000,TUID,TGID,QBYT,1,-1,EAK},
{RUID,RGID,TUID,TGID,TUID,TGID,QBYT,0000,STAT,0000,TUID,TGID,QBYT,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,QBYT,0000,SET, 0000,TUID,TGID,QBYT,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,QBYT,0000,SET, 0000,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,QBYT,0000,SET, 0000,TUID,TGID,QBYT,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,QBYT,0000,SET, 0000,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,QBYT,0000,SET, 0000,BUID,TGID,QBYT,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,QBYT,0000,SET, 0000,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,QBYT,0000,SET, 0600,TUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,QBYT,0000,SET, 0600,BUID,BGID,QBYT,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,QBYT,0000,SET, 0600,BUID,TGID,QBYT,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,QBYT,0000,SET, 0600,TUID,TGID,QBYT,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,QBYT,0000,SET, 0600,TUID,TGID,QBYT,1,-1,EPR},
{TUID,TGID,TUID,TGID,TUID,TGID,QBYT,0000,SET, 0000,TUID,TGID,0,1,0,0},
{TUID,TGID,TUID,TGID,TUID,TGID,QBYT,0000,SET, 0000,TUID,TGID,1,1,0,0},
{TUID,TGID,TUID,TGID,TUID,TGID,   0,0000,SET, 0000,TUID,TGID,1,1,-1,EPR},
{TUID,TGID,TUID,TGID,TUID,TGID,QBYT,0000,SET, 0000,TUID,TGID,QBIG,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,   0,0000,SET, 0000,TUID,TGID,1,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,QBYT,0000,SET, 0600,BUID,BGID,1,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,QBYT,0000,SET, 0000,TUID,TGID,QBIG,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,QBYT,0000,RMID,0000,TUID,TGID,QBYT,0,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,QBYT,0000,RMID,0000,TUID,TGID,QBYT,0,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,QBYT,0000,RMID,0000,TUID,TGID,QBYT,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,QBYT,0000,RMID,0000,TUID,TGID,QBYT,0,0,0},
};

#define NTESTS	( sizeof( test ) / sizeof( struct test_case ) )

main()
{
	setup("msgctl");
	funct("msgctl");

	prep ("initialize the array of test cases");
	start
		int i;

		for (i=0 ; i<NTESTS ; i++)
			{
			test [i].tu = mapid (test [i].tu);
			test [i].tg = mapid (test [i].tg);
			test [i].mu = mapid (test [i].mu);
			test [i].mg = mapid (test [i].mg);
			test [i].cu = mapid (test [i].cu);
			test [i].cg = mapid (test [i].cg);
			test [i].nu = mapid (test [i].nu);
			test [i].ng = mapid (test [i].ng);
			if (test [i].xr == EAK)
				test [i].xr = EACCES;
			else if (test [i].xr == EPR)
				test [i].xr = EPERM;
			}
	finis
	

	testing ("Get the value of msg_qbytes for later use");
	testing ("msgget, msgctl(IPC_STAT), and msgctl(IPC_RMID) ");
	/*  testing("should all succeed");  */
	start

	init_qbytes();

	finis

	while( get_a_test() && (errcnt > 0) ) {
		start
		if(!do_tst_expct_msg()) 
		    if(creat_a_queue())	/* child to make queue 		*/
		    {
			errcnt--;
		    }
		    else
		    {
		        if(do_a_test())	/* child for test operation 	*/
			    errcnt--;
		        if(check_results())/* parent chk/clean after child*/
			    errcnt--;
		    }
		finis
	}
	if( errcnt <= 0 )
		error ("Too many errors: test aborted");

	testing ("msgctl(IPC_STAT) with invalid msqid of (-1)");
	testing ("should fail to stat, with an errno of EINVAL");
	start
	
	expecting (-1);
	expecting (EINVAL);
	msgctl((-1), IPC_STAT, buf);
	finis

	done();
/*NOTREACHED*/
}
get_a_test()
{
	static int i = 0;

	if( i >= NTESTS ) return(0);

	tuid = test[i].tu;	/* uid of test proc	*/
	tgid = test[i].tg;	/* group of test proc	*/

	oper = test[i].op;	/* operation for test	*/
	extant= test[i].xt;	/* msg is not killed 	*/
	expected = test[i].ex;	/* expected return val	*/
	exerrno = test[i].xr;	/* expected errno	*/


	cur->msg_perm.mode=test[i].mo;
	cur->msg_perm.uid=test[i].mu;
	cur->msg_perm.gid=test[i].mg;
	cur->msg_perm.cuid=test[i].cu;
	cur->msg_perm.cgid=test[i].cg;

	switch( test[i].qb ) {
		case QBYT:
			cur->msg_qbytes = qbytes;
			break;
		case QBIG:
			error ("bad data in msgctl test");
		default:
			cur->msg_qbytes = test[i].qb;
			break;

	}

	cur->msg_qnum = 0;		
	cur->msg_lspid = 0;
	cur->msg_lrpid = 0;
	cur->msg_stime = 0;
	cur->msg_rtime = 0;
	cur->msg_ctime = 0;

	nxt->msg_perm.mode=test[i].nm;
	nxt->msg_perm.uid=test[i].nu;
	nxt->msg_perm.gid=test[i].ng;
	nxt->msg_perm.cuid=test[i].cu;
	nxt->msg_perm.cgid=test[i].cg;

	kludge = 0;
	switch( test[i].nq ) {
		case QBYT:
			nxt->msg_qbytes = qbytes;
			break;
		case QBIG:
			nxt->msg_qbytes = qbytes + 1;
			if((long) nxt->msg_qbytes != ((long) qbytes + 1L))
				kludge = 1;
			break;
		default:
			nxt->msg_qbytes = test[i].nq;
			break;
	}

	nxt->msg_qnum = 0;		
	nxt->msg_lspid = 0;
	nxt->msg_lrpid = 0;
	nxt->msg_stime = 0;
	nxt->msg_rtime = 0;
	nxt->msg_ctime = 0;

	i++;

	return(1);
}	
creat_a_queue()
{
	int pid;
	int sts;
	int ret_val;
	int msgflg;
	int qid;

	prep ("Fork a subtest.");
	start

	nxt->msg_ctime = cur->msg_ctime = (time_t)time((ttime *)0);
	expecting  (SUCCESS);
	if(!(pid=fork()))
	{
		setgid( cur->msg_perm.cgid );

		setuid( cur->msg_perm.cuid );

		msgflg = IPC_CREAT | IPC_EXCL;
		expecting (SUCCESS);
		/*  creat process can not create message queue?  */
		qid = msgget(TESTKEY, msgflg);

		/*  creat process can not set owner/perm of message queue?  */
		msgctl(qid, IPC_SET, cur);
		exit(0);
/*NOTREACHED*/
	}
	finis

	expecting (VOID);
	if( (ret_val=wait(&sts)) != pid )
	{
		error("wait() returned unexpected value for creat process of %d\n\
%d was expected",
		  ret_val,pid);
	}
	if ( sts ) 
	{
		error("creat process terminated abnormally");
		return(1);
	}
	return(0);
}
do_a_test()
{
	int pid;
	int sts;
	int qid;
	int ret_val;
	int msgflg;
	int errflag;
	extern	char	*err_lookup ();

	prep ("Fork a subtest.");
	start

	expecting (SUCCESS);
	if(!(pid=fork()))
	{
		errflag = 0;

		setgid( tgid );

		setuid( tuid );

		msgflg = 0;
		/*  test process can not get message queue?  */
		expecting (SUCCESS);
		qid = msgget(TESTKEY, msgflg);

		if( oper == IPC_STAT )
			fill(buf,sizeof(struct msqid_ds),'\0');
		else
			bcpy(buf,nxt,sizeof(struct msqid_ds));
		
		expecting (VOID);
		if( (ret_val = msgctl(qid, oper, buf)) != expected )
		{
			error("msgctl returned unexpected value of %d, \
%d was expected",
				ret_val, expected);
			errflag = 1;
		}
		else if (exerrno && exerrno != errno)
		{
			error ("unexpected errno = %s, %s was expected",
			  err_lookup (exerrno), err_lookup (errno));
			errflag = 1;
		}
		if( oper == IPC_STAT && !expected )
			if ( chk_buf( buf, nxt ) )
				errflag = 1;
		exit( errflag );
	}
	finis

	expecting (VOID);
	if( (ret_val=wait(&sts)) != pid )
	{
		error("wait() returned %d for test process, %d was expected",
		  ret_val,pid);
	}
	if ( sts ) 
	{
		error("test process terminated abnormally with status %d", sts);
		return(1);
	}
	return(0);
}
check_results()
{
	int qid;
	int ret_val;
	int errflag;

	errflag =0;
	if (extant) 
	{
		if ( (qid = msgget(TESTKEY, 0)) < 0)
			error ("msgget failed in parent process");

		if( oper != IPC_STAT )
		{
			fill(buf,sizeof(struct msqid_ds),'\0');

			if(ret_val=msgctl(qid,IPC_STAT,buf) )
			{
				error("msgctl(IPC_STAT) failed in parent \
process, returning %d",
				ret_val);
				errflag = 1;
			}
			else if ( expected )
			{
				if(chk_buf( buf, cur ))
				{
					errflag = 1;
				}
			}
			else
			{
				if(chk_buf( buf, nxt ))
				{
					errflag = 1;
				}
			}
		}
		if ( (ret_val = msgctl(qid, IPC_RMID, buf)) != 0 )
			error("parent process can not remove msg queue, \
msgctl returned %d", ret_val);
	}		
	/*  Was the message queue not removed as expected?  */
	expecting (-1);
	expecting (ENOENT);
	msgget(TESTKEY, 0);
	return(errflag);
}
init_qbytes()
{
	int msgflg;
	int ret_val;
	int qid;


	msgflg = IPC_CREAT | IPC_EXCL | 0600 ;
	/* 
	 *  If this fails, abort the test since no queues are available
	 */
	exit_on (ERROR | SEVERR);

	/*  If  the msgget fails - can not creat message queue  */
	expecting (SUCCESS);
	qid = msgget(TESTKEY, msgflg);

	/*
	 *  verify that the queue was created
	 */
	msgctl(qid,IPC_STAT,buf);
	qbytes = buf->msg_qbytes;

	/*  If the msgctl fails - we can not remove message queue */
	expecting (SUCCESS);
	msgctl(qid, IPC_RMID, buf);

	expecting (-1);  /*  Was the message queue removed as expected?  */
	expecting (ENOENT);
	msgget(TESTKEY, 0);

}
chk_buf( p1, p2 )
struct msqid_ds *p1, *p2;
{
	int errflag;

	errflag = 0;
	
	if ( p1->msg_qnum != p2->msg_qnum )
	{
		error ("unexpected msg_qnum of %d, %d was expected",
		p2->msg_qnum, p1->msg_qnum);
		errflag = 1;
	}
	if ( p1->msg_lspid != p2->msg_lspid )
	{
		warning ("%s\nmsg_lspid = %d, %d was expected",
			ADVISORY, p2->msg_lspid, p1->msg_lspid);
		errflag = 1;
	}
	if ( p1->msg_lrpid != p2->msg_lrpid )
	{
		warning ("%s\nmsg_lrpid = %d, %d was expected",
			ADVISORY, p2->msg_lspid, p1->msg_lrpid);
		errflag = 1;
	}
	if ( p1->msg_stime != p2->msg_stime )
	{
		error("msg_stime = %d, %d was expected",
			p2->msg_stime, p1->msg_stime);
		errflag = 1;
	}
	if ( p1->msg_rtime != p2->msg_rtime )
	{
		error("msg_rtime = %d, %d was expected", 
			p2->msg_rtime, p1->msg_rtime);
		errflag = 1;
	}
	if ( p1->msg_ctime < p2->msg_ctime )
	{
		error("msg_ctime = %d, %d was expected", 
			p2->msg_ctime, p1->msg_ctime);
		errflag = 1;
	}
	if ( p1->msg_perm.uid != p2->msg_perm.uid )
	{
		error ("msg_perm.uid = %d, %d was expected", 
			p2->msg_perm.uid, p1->msg_perm.uid);
		errflag = 1;
	}
	if ( p1->msg_perm.gid != p2->msg_perm.gid )
	{
		error ("msg_perm.gid = %d, %d was expected", 
			p2->msg_perm.gid, p1->msg_perm.gid);
		errflag = 1;
	}
	if ( p1->msg_perm.cuid != p2->msg_perm.cuid )
	{
		error ("msg_perm.cuid = %d, %d was expected", 
			p2->msg_perm.cuid, p1->msg_perm.cuid);
		errflag = 1;
	}
	if ( p1->msg_perm.cgid != p2->msg_perm.cgid )
	{
		error ("msg_perm.cgid = %d, %d was expected", 
			p2->msg_perm.cgid, p1->msg_perm.cgid);
		errflag = 1;
	}
	if ( p1->msg_qbytes != p2->msg_qbytes )
	{
		error ("msg_qbytes = %d, %d was exopected", 
			p2->msg_qbytes, p1->msg_qbytes);
		errflag = 1;
	}
	return(errflag);
}
do_tst_expct_msg()
{
	char buf[512];

	testing ("While executing with uid: %d and gid: %d,",
	  cur->msg_perm.cuid,
	  cur->msg_perm.cgid);
	testing ("create a message queue with mode: 0%o,",
	  cur->msg_perm.mode );
	testing ("uid: %d, gid: %d, and qbytes: %d.",
	  cur->msg_perm.uid,
	  cur->msg_perm.gid, cur->msg_qbytes);
	testing (
	  "Then while executing with uid:%d and gid:%d, attempt using",
		tuid, tgid);
	switch( oper )
	{
	case IPC_STAT:
		testing("msgctl(IPC_STAT).");
		if ( expected )
		{
			testing ("which should not to be able to stat the \
message queue,");
			testing("returning an errno of EACCES.");
		}
		else
		{
			testing ("stat message queue should place the expected results in buffer.");
		}
		break;
	case IPC_RMID:
		testing("msgctl(IPC_RMID).");
		if( expected ) 
		{
			testing ("Then attempt msgctl(IPC_STAT)");
			testing ("to check for unchanged queue.");
			testing ("This should failure to remove the");
			testing ("message queue and return an errno of EPERM.");
		}
		else				
			testing ("should remove message queue.");
		break;
	case IPC_SET:
		testing("msgctl(IPC_SET) should set mode: 0%o, uid: %d, gid: %d, ",
		  nxt->msg_perm.mode,
		  nxt->msg_perm.uid,
		  nxt->msg_perm.gid);
		testing ("and qbytes: %ld.",(long)nxt->msg_qbytes);
		testing("Then attempt msgctl(IPC_STAT) to check results.");
		if (expected)
		{
			testing ("should fail to set mode, uid, gid, and qbytes,");
			testing ("returning an errno of EPERM.");
		}
		else
		{
			testing ("should set mode, uid, gid and qbytes.");
		}
		break;
	}
	if (kludge)
	{
		warning ("msg_qbytes already at hardware limit, test suppressed");
		return(1);
	}
	return(0);
}
fill( ptr, cnt, fillchar )
char *ptr;
int cnt;
char fillchar;
{
	int i;

	for(i=0;i<cnt;i++)
		*ptr++ = fillchar;
}
bcpy( p1, p2, cnt )
char *p1, *p2;
int cnt;
{
	int i;

	for( i=0; i<cnt; i++ )
		*p1++ = *p2++;
}
/*
 *  This function is used for initialization,
 *  substituting the real values for the dummy ones.
 */
mapid (id)
int	id;
{
	switch (id)
	{
	  case  TUID:
		return (UID);
	  case	TGID:
		return (GID);
	  case	RUID:
		return (ROOT_UID);
	  case	RGID:
		return (ROOT_GID);
	  case	BUID:
		return (BIN_UID);
	  case	BGID:
		return (BIN_GID);
	  default:
		error ("mapid was passed %d", id);
	}
}


# else
main()
{
	void exit();
	
	unsupported ("Message subsystem is not supported.");
	exit(0);
}
# endif
