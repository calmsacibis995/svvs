#include	<sv_base.h> /*replacement macros */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <svvs.h>
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)shmctl.c	1.7"

# ifdef SHM

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>

/*
**	shmctl test
**
**	This test runs setuid root.  
**
**	The last test case, which checks for EINVAL, is hard coded. 
**	All of the other test cases are table driven.
**	
**	For each test case, a child process is forked to create
**	a memory segment.  Upon its return, a second child is
**	forked to actually do the test.  Upon its return, the
**	parent will check the results of the second process, and
**	if needed, remove the shared memory.
**
**	See the table "test[]" for the test cases.
**
**	The creat process does a shmget to creat the shared memory.
**	The shared memory should not exist before this, it is an
**	error if it does exist, and the process will terminate.
**	If the shared memory is successfully created, the creat process
**	will use shmctl( IPC_SET ) to configure the memory to the 
**	initial values specified in the table.  The initial
**	values were put into (struct shmid_ds *cur) by get_a_test().
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
**	buffer passed to shmctl, (struct shmid_ds *buf), and then 
**	check the returned buffer against the initial values, 
**	(struct shmid_ds *cur).  Mismatches are reported as errors.
**
**	If the operation is IPC_SET or IPC_RMID, the test process will 
**	set the buffer to the same values as (struct shmid_ds *nxt) before 
**	passing to shmctl.  Note that for IPC_RMID, the buffer contents
**	are really of no concern.
**
**	The parent process will IPC_STAT the shared memory if it is 
**	expected to exist after the test process terminates and the
**	test process was not testing IPC_STAT  (otherwise stat mismatches
**	would most likely be reported twice).  If the expected outcome of 
**	the test was success, the parent process will test that the stat
**	matches (struct shmid_ds *nxt).  If the expected outcome was
**	failure, the parent process will test that the stat matches
**	(struct shmid_ds *cur), to confirm that the memory has not been
**	corrupted by the failed attempt.  Any mismatches are reported as 
**	errors.  If the shared memory is expected to exist after the test
**	process terminates, the parent will attempt to remove it with 
**	shmctl(IPC_RMID).  The last operation for each test case is to
**	attempt a shmget without the IPC_CREAT flag.  This should fail.
**	If this does not fail, then the memory was not correctly removed
**	from the system.
**
**	Any time a child process reports an error, the parent
**	should report "child process terminated abnormally".  The parent
**	keeps track of the number of test cases with errors, and will 
**	halt after 10 failures.  
**
**	It should be noted again that chk_buf(), which reports problems
**	between the expected and actual stat buffers, is called from the
**	test process if the operation is IPC_STAT. Otherwise, it is 
**	called from the parent process.  All other error messages should
**	give some indication of which process generated the message.
**
**
**
*/

# define TESTKEY 23		/* a most random number	*/

int tuid;			/* owner of test proc	*/
int tgid;			/* group of test proc	*/

int oper;			/* operation for test	*/
int extant;			/* shm is not killed 	*/

struct shmid_ds mybuf;
struct shmid_ds *buf = &mybuf;	/* pass to IPC_STAT	*/
struct shmid_ds mycur;
struct shmid_ds *cur = &mycur;	/* current state, creat	*/
struct shmid_ds mynxt;
struct shmid_ds *nxt = &mynxt;	/* future state, test	*/

int errcnt = 10;		/* abort after 10 errs	*/
int smin;			/*Minimum shared memory segment size */
int expected;			/* expected return val	*/
int exerrno;			/* expected errno	*/

long	time();
/*
**  The following #defines allow the table which follows to fit on
**  an 80 column screen.
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
#define STAT	IPC_STAT
#define SET	IPC_SET
#define RMID	IPC_RMID
#define EAK	-61	/*  EACCES  */
#define EPR	-62	/*  EPERM  */

struct test_case {
	int tu;			/* test user id 	*/
	int tg;			/* test group id	*/
	int mu;			/* shm owner		*/
	int mg;			/* shm group		*/
	int cu;			/* shm creator owner	*/
	int cg;			/* shm creator group	*/
	int mo;			/* shm mode		*/
	int op;			/* operation		*/
	int nm;			/* new mode		*/
	int nu;			/* new owner		*/
	int ng;			/* new group		*/
	int xt;			/* exists after test	*/
	int ex;			/* expected ret val	*/
	int xr;			/* expected errno	*/
} test[] = {
/*-------- ----------------------------- ---- ------------------- -----
 test_proc    shm   attributes creat     attr after tst	retrn 
  uid gid   uid gid  cuid cgid mode oper nmde nuid ngid X R E
---------- ----------------------------- ---- ------------------- -----*/
{TUID,TGID,TUID,BGID,BUID,BGID,0400,STAT,0400,TUID,BGID,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,0400,STAT,0400,BUID,BGID,1,0,0},
{TUID,TGID,BUID,TGID,BUID,BGID,0040,STAT,0040,BUID,TGID,1,0,0},
{TUID,TGID,BUID,BGID,BUID,TGID,0040,STAT,0040,BUID,BGID,1,0,0},
{TUID,TGID,BUID,BGID,BUID,BGID,0004,STAT,0004,BUID,BGID,1,0,0},
{TUID,TGID,TUID,TGID,TUID,TGID,0000,STAT,0000,TUID,TGID,1,-1,EAK},
{RUID,RGID,TUID,TGID,TUID,TGID,0000,STAT,0000,TUID,TGID,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,0000,SET, 0000,TUID,TGID,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,0000,SET, 0000,BUID,BGID,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,0000,SET, 0000,TUID,TGID,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,0000,SET, 0000,BUID,BGID,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,0000,SET, 0000,BUID,TGID,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,0000,SET, 0000,BUID,BGID,1,0,0},
{TUID,TGID,TUID,BGID,BUID,BGID,0000,SET, 0600,TUID,BGID,1,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,0000,SET, 0600,BUID,BGID,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,0000,SET, 0600,BUID,TGID,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,0000,SET, 0600,TUID,TGID,1,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,0000,SET, 0600,TUID,TGID,1,-1,EPR},
{TUID,TGID,TUID,BGID,BUID,BGID,0000,RMID,0000,TUID,TGID,0,0,0},
{TUID,TGID,BUID,BGID,TUID,BGID,0000,RMID,0000,TUID,TGID,0,0,0},
{TUID,TGID,BUID,TGID,BUID,TGID,0000,RMID,0000,TUID,TGID,1,-1,EPR},
{RUID,RGID,TUID,TGID,TUID,TGID,0000,RMID,0000,TUID,TGID,0,0,0},
};

# define NTESTS	( sizeof( test ) / sizeof( struct test_case ) )

main()
{
	setup("shmctl");
	funct("shmctl");

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


	testing("Find the machine dependent minimum size of a shared memory segment");
	testing("so that this test can create shared memory segments.");
	/*  testing("A non-negative return value from shmget.");  */
	start

	init_smin();

	finis

	while( get_a_test() && (errcnt > 0) )
		{
		start
		if(!do_tst_expct_shm()) 
			if(creat_a_thing())	/* child to make segment */
				{
				errcnt--;
				}
			else
				{
				if(do_a_test())	/* child for test operation  */
					errcnt--;
				if(check_results()) /* parent chk/clean after child */
					errcnt--;
				}
		finis
		}

	if( errcnt <= 0 )
		error ("Too many errors: test aborted");


	testing ("shmctl(IPC_STAT) with an invalid shmid of (-1)");
	testing ("should fail to stat, with an errno of EINVAL");
	start
	
	expecting (-1);
	expecting (EINVAL);
	shmctl((-1), IPC_STAT, buf);
	finis

	done();
/*NOTREACHED*/

}

init_smin()
{
	int	shmid;

	expecting (VOID);  /*  This loops until the shmget succeeds  */
	for(smin=1;smin>0;smin++)
	{
		shmid = shmget((key_t) TESTKEY, smin,IPC_CREAT|0666);
		if ( shmid != -1 )
			break;
		/*  The only failure allowed is EINVAL (size too small)
		 *	all others (segment exists, table full) are errors
		 */
		if (errno != EINVAL)
			error ("shmget (0x%x, %d, IPC_CREAT | 0666) failed\n\
errno = %d", TESTKEY,smin,errno);
	}

	/*  give up if no segment was found  */
	exit_on (ERROR);

	if(!smin)
		error ("shmget could not create a segment");
	if(shmctl(shmid,IPC_RMID,cur))
		error ("shmctl could not remove a segment");
	if (shmget((key_t) TESTKEY, smin, 0) >= 0)
		error ("memory segment was not removed as expected");

}

get_a_test()
{
	static int i = 0;

	if( i >= NTESTS ) return(0);

	tuid = test[i].tu;	/* uid of test proc	*/
	tgid = test[i].tg;	/* group of test proc	*/

	oper = test[i].op;	/* operation for test	*/
	extant= test[i].xt;	/* shm is not killed 	*/
	expected = test[i].ex;	/* expected return val	*/
	exerrno = test[i].xr;	/* expected errno	*/


	cur->shm_perm.mode=test[i].mo;
	cur->shm_perm.uid=test[i].mu;
	cur->shm_perm.gid=test[i].mg;
	cur->shm_perm.cuid=test[i].cu;
	cur->shm_perm.cgid=test[i].cg;

	cur->shm_segsz = smin;		
	cur->shm_lpid = 0;
	cur->shm_nattch = 0;
	cur->shm_atime = 0;
	cur->shm_dtime = 0;
	cur->shm_ctime = 0;

	nxt->shm_perm.mode=test[i].nm;
	nxt->shm_perm.uid=test[i].nu;
	nxt->shm_perm.gid=test[i].ng;
	nxt->shm_perm.cuid=test[i].cu;
	nxt->shm_perm.cgid=test[i].cg;

	cur->shm_segsz = smin;		
	cur->shm_lpid = 0;
	cur->shm_nattch = 0;
	cur->shm_atime = 0;
	cur->shm_dtime = 0;
	cur->shm_ctime = 0;

	i++;

	return(1);
}	
creat_a_thing()
{
	int pid;
	int sts;
	int ret_val;
	int shmflg;
	int shmid;

	prep ("Fork a subtest.");
	start
	if(!(pid=fork()))
	{
		if(setgid( cur->shm_perm.cgid ))
			error ("creat process can not setgid to cgid");

		if(setuid( cur->shm_perm.cuid ))
			error ("creat process can not setuid to cuid");

		shmflg = IPC_CREAT | IPC_EXCL;
		if ( (shmid = shmget((key_t) TESTKEY, smin, shmflg)) < 0)
			error ("creat process can not create memory segment");

		cur->shm_ctime = nxt->shm_ctime = (time_t)time((long) 0);
		if( shmctl(shmid, IPC_SET, cur) )
			error ("creat process can not set owner/perm of memory segment");
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
	cur->shm_cpid = nxt->shm_cpid = pid;
	return(0);
}
do_a_test()
{
	int pid;
	int sts;
	int shmid;
	int ret_val;
	int shmflg;
	int errflag;

	extern	char *err_lookup ();

	expecting (SUCCESS);

	prep ("Fork a subtest.");
	start

	if(!(pid=fork()))
	{
		errflag = 0;

		setgid( tgid );

		setuid( tuid );

		shmflg = 0;
		/*  fails if test process can not get memory segment  */
		expecting (SUCCESS);
		shmid = shmget((key_t) TESTKEY, smin, shmflg);

		if( oper == IPC_STAT )
			fill(buf,sizeof(struct shmid_ds),'\0');
		else
			bcpy(buf,nxt,sizeof(struct shmid_ds));
		
		expecting (VOID);
		if( (ret_val = shmctl (shmid, oper, buf)) != expected )
		{
			error("shmctl returned unexpected value of %d, \
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
	int shmid;
	int ret_val;
	int errflag;

	errflag =0;
	if (extant) 
	{
		if ( (shmid = shmget((key_t) TESTKEY,smin, 0)) < 0)
			error ("shmget failed in parent process");

		if( oper != IPC_STAT )
		{
			fill(buf,sizeof(struct shmid_ds),'\0');

			if(ret_val=shmctl(shmid,IPC_STAT,buf) )
			{
			    error("shmctl(IPC_STAT) failed in parent process\
returning %d",
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
		if ( (ret_val = shmctl(shmid, IPC_RMID, buf)) != 0 )
			error("parent process can not remove shm segment\n\
returning %d", 
				ret_val);
	}		

	/*  Was the memory segment not removed as expected?  */
	expecting (-1);
	expecting (ENOENT);
	shmget((key_t) TESTKEY, smin, 0);

	return(errflag);
}

chk_buf( p1, p2 )
struct shmid_ds *p1, *p2;
{
	int errflag;

	errflag = 0;
	
	if ( p1->shm_cpid != p2->shm_cpid )
	{
		warning ("%s\nshm_cpid = %d, %d was expected",
			ADVISORY, p1->shm_cpid, p2->shm_cpid);
		errflag = 1;
	}
	if ( p1->shm_lpid != p2->shm_lpid )
	{
		warning ("%s\nshm_lpid = %d, %d was expected",
			ADVISORY, p1->shm_lpid, p2->shm_lpid);
		errflag = 1;
	}
	if ( p1->shm_nattch != p2->shm_nattch )
	{
		error ("shm_nattch = %d, %d was expected",
			p1->shm_nattch, p2->shm_nattch);
		errflag = 1;
	}
	if ( p1->shm_atime != p2->shm_atime )
	{
		error ("shm_atime = %d, %d was expected",
			p1->shm_atime, p2->shm_atime);
		errflag = 1;
	}
	if ( p1->shm_dtime != p2->shm_dtime )
	{
		error ("shm_dtime = %d, %d was expected",
			p1->shm_dtime, p2->shm_dtime);
		errflag = 1;
	}
	if ( p1->shm_ctime < p2->shm_ctime )
	{
		error ("shm_ctime = %d, %d was expected",
			p1->shm_ctime, p2->shm_ctime);
		errflag = 1;
	}
	if ( p1->shm_perm.uid != p2->shm_perm.uid )
	{
		error ("shm_perm.uid = %d, %d was expected",
			p1->shm_perm.uid, p2->shm_perm.uid);
		errflag = 1;
	}
	if ( p1->shm_perm.gid != p2->shm_perm.gid )
	{
		error ("shm_perm.gid = %d, %d was expected",
			p1->shm_perm.gid, p2->shm_perm.gid);
		errflag = 1;
	}
	if ( p1->shm_perm.cuid != p2->shm_perm.cuid )
	{
		error ("shm_perm.cuid = %d, %d was expected",
			p1->shm_perm.cuid, p2->shm_perm.cuid);
		errflag = 1;
	}
	if ( p1->shm_perm.cgid != p2->shm_perm.cgid )
	{
		error ("shm_perm.cgid = %d, %d was expected",
			p1->shm_perm.cgid, p2->shm_perm.cgid);
		errflag = 1;
	}
	return(errflag);
}
do_tst_expct_shm()
{
	char buf[512];

	testing ("While executing with uid: %s and gid: %s,",
	  (cur->shm_perm.cuid == UID)?("svvs"):("bin"),
	  (cur->shm_perm.cgid == GID)?("svvs"):("bin"));
	testing ("create a memory segment with mode: 0%o,",
	  cur->shm_perm.mode );
	testing ("uid: %s, gid: %s.",
	  (cur->shm_perm.uid == UID)?("svvs"):("bin"),
	  (cur->shm_perm.gid == GID)?("svvs"):("bin"));
	testing (
	  "Then while executing with uid:%s and gid:%s, attempt using",
	  (tuid == UID)?("svvs"):("root"),
	  (tgid == GID)?("svvs"):("root"));
	switch( oper )
	{

	case IPC_STAT:
		testing("shmctl(IPC_STAT).");
		if ( expected )
		{
			testing ("which should not to be able to stat the \
memory segment,");
			testing("returning an errno of EACCES.");
		}
		else
		{
			testing ("stat memory segment ");
			testing("should place the expected ");
			testing("results in buffer.");
		}
		break;

	case IPC_RMID:
		testing("shmctl(IPC_RMID).");
		if( expected ) 
		{
			testing ("Then attempt shmctl(IPC_STAT)");
			testing ("to check for unchanged segment.");
			testing ("This should fail to remove the");
			testing ("memory segment and return an errno of EPERM.");
		}
		else				
			testing ("should remove memory segment.");
		break;

	case IPC_SET:
		cur->shm_ctime = nxt->shm_ctime = (time_t)time((long) 0);

		testing("shmctl(IPC_SET) ");
		testing ("should set mode: 0%o, ",nxt->shm_perm.mode);
		testing ("uid: %s, gid: %s, ",
		  (nxt->shm_perm.uid==UID)?("svvs"):("bin"),
		  (nxt->shm_perm.gid==GID)?("svvs"):("bin"));
		testing("Then attempt shmctl(IPC_STAT) to check results.");
		if (expected)
		{
			testing ("should fail to set ");
			testing ("mode, uid, gid, and qbytes,");
			testing ("returning an errno of EPERM.");
		}
		else
		{
			testing ("should set mode, uid, and gid.");
		}
		break;
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
	
	unsupported("Shared Memory subsystem not supported.");
	exit(0);
}
# endif
