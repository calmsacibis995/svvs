#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)fcntl1.c	1.10"

#ifndef	LOCKF

main()	/*  dummy main routine  */
{
	setup ("fcntl");
	funct ("fcntl");
	unsupported("Record locking is not supported in this system.");
}

#else

#define	SKIP		0
#define	NOBLOCK		2
#define	WILLBLOCK	3

#define	GRACE	60

typedef	struct
	{
	short	a_type;
	short	a_whence;
	long	a_start;
	long	a_len;
	short	b_type;		 /*  0 means suppress fcntl call  */
	short	b_whence;
	long	b_start;
	long	b_len;
	short	c_type;		 /*  0 means no child needed */
	short	c_whence;
	long	c_start;
	long	c_len;
	short	c_flag;

		/*  NOBLOCK - immediate success
		 *  WILLBLOCK - blocks, then succeeds (parent must unlock records)
		 */
	}	testcase;

static	testcase	testcases[]
={
/*
 *  All the test cases from fcntl1.c are covered by the cases in fcntl2.c.
 *  This is because all locks that are expected to block are tested to
 *  see if they fail immediately using SETLK, so the two tests are done
 *  in one shot.
 *
 *  Possible failure modes:
 *	If some of the children never unblock,
 *	perhaps the parent's attempt to unlock all its locks
 *	is not working (stopping with the first lock found).
 *
 *	If none of the children unblock,
 *	perhaps there is a problem with unlock not waking up
 *	waiting processes.
 *
 *  test cases from fcntl2.c
 */
{F_WRLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 0L, 10L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 15L, 10L, NOBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 10L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 3L, 5L, WILLBLOCK},
{F_WRLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 0L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 5L, 10L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 3L, 5L, WILLBLOCK},
{F_RDLCK, 0, 0L, 10L,
	F_UNLCK, 0, 0L, 10L,
		F_WRLCK, 0, 0L, 10L, NOBLOCK},
{F_RDLCK, 0, 0L, 0L,
	F_UNLCK, 0, 10L, 10L,
		F_WRLCK, 0, 10L, 10L, NOBLOCK},
{F_WRLCK, 0, 0L, 0L,
	F_UNLCK, 0, 10L, 10L,
		F_RDLCK, 0, 10L, 10L, NOBLOCK},
{F_WRLCK, 0, 0L, 0L,
	F_UNLCK, 0, 10L, 10L,
		F_RDLCK, 0, 20L, 10L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	F_WRLCK, 0, 0L, 0L,
		SKIP, 0, 0L, 0L, NOBLOCK},
{F_WRLCK, 0, 0L, 0L,
	F_RDLCK, 0, 0L, 0L,
		SKIP, 0, 0L, 0L, NOBLOCK},

/*  test cases from fcntl3.c  */
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 0L, 5L, WILLBLOCK},
{F_RDLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 5L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 0L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 10L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 0L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	F_RDLCK, 0, 0L, 10L,
		F_WRLCK, 0, 0L, 0L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 10L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	F_RDLCK, 0, 0L, 10L,
		F_WRLCK, 0, 0L, 10L, WILLBLOCK},
{F_RDLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 15L, 10L, NOBLOCK},
{F_RDLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 5L, 10L, WILLBLOCK},
{F_RDLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 3L, 5L, WILLBLOCK},
{F_WRLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 0L, 0L, WILLBLOCK},

/*  test cases from fcntl4.c  */
{F_WRLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 0L, 10L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 15L, 10L, NOBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 10L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 3L, 5L, WILLBLOCK},
{F_WRLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 0L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 5L, 10L, WILLBLOCK},
{F_WRLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 3L, 5L, WILLBLOCK},
{F_RDLCK, 0, 0L, 10L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 0, 0L, 10L, WILLBLOCK},
{F_RDLCK, 0, 0L, 0L,
	F_UNLCK, 0, 10L, 10L,
		F_WRLCK, 0, 10L, 10L, NOBLOCK},
{F_WRLCK, 0, 0L, 0L,
	F_UNLCK, 0, 10L, 10L,
		F_RDLCK, 0, 10L, 10L, NOBLOCK},
{F_RDLCK, 0, 0L, 0L,
	F_WRLCK, 0, 0L, 0L,
		SKIP, 0, 0L, 0L, NOBLOCK},
{F_WRLCK, 0, 0L, 0L,
	F_RDLCK, 0, 0L, 0L,
		SKIP, 0, 0L, 0L, NOBLOCK},

#ifndef	SYSVR_2

{F_RDLCK, 0, 0L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 0L, 0L, NOBLOCK},

{F_RDLCK, 0, 3L, 10L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 8L, 0L, NOBLOCK},

/*
 *  Test for whence = 1 (current position) and 2 (end of file).
 *  Remember, the temp file is opened, then 10 bytes
 *  are written, so the file pointer to at the 11th byte
 *  which is also end of file.
 */

{F_WRLCK, 0, 0L, 12L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 5L, WILLBLOCK},

{F_WRLCK, 0, 0L, 12L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 2L, 1L, NOBLOCK},

{F_WRLCK, 0, 0L, 12L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 1L, 1L, WILLBLOCK},

{F_WRLCK, 2, 1L, 0L,
	SKIP, 0, 0L, 0L,
		F_WRLCK, 2, 0L, 1L, NOBLOCK},

{F_WRLCK, 2, 1L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 1L, NOBLOCK},

{F_WRLCK, 2, 1L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 1L, 1L, WILLBLOCK},

{F_WRLCK, 2, 1L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 2L, WILLBLOCK},

{F_WRLCK, 2, 1L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 1L, NOBLOCK},

{F_WRLCK, 2, 1L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 20L, WILLBLOCK},

{F_WRLCK, 2, 2L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 1L, 1L, NOBLOCK},

{F_WRLCK, 2, 2L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 5L, NOBLOCK},

{F_WRLCK, 2, 2L, 0L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 0L, WILLBLOCK},

{F_WRLCK, 1, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 0L, WILLBLOCK},

{F_WRLCK, 1, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 14L, 1L, NOBLOCK},

{F_WRLCK, 1, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 15L, 1L, WILLBLOCK},

{F_WRLCK, 1, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 5L, NOBLOCK},

{F_WRLCK, 1, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 20L, WILLBLOCK},

{F_WRLCK, 2, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 5L, 0L, WILLBLOCK},

{F_WRLCK, 2, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 14L, 1L, NOBLOCK},

{F_WRLCK, 2, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 0, 15L, 1L, WILLBLOCK},

{F_WRLCK, 2, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 5L, NOBLOCK},

{F_WRLCK, 2, 5L, 2L,
	SKIP, 0, 0L, 0L,
		F_RDLCK, 1, 0L, 20L, WILLBLOCK},

#endif
};


#define	NTESTS	(sizeof (testcases) / sizeof (testcase))

static	testcase	*thiscase;
static	struct	flock	flock;
static	int	parent, child;
static	int	got1 = 0;
static	int	fd;

char	*err_lookup ();

#define	FILEDATA	"ten bytes!"

main() {
	extern	void	catch1 ();	/*  signal catching subroutine  */

	setup ("fcntl");
	funct ("fcntl");

	signal (SIGUSR1, catch1);
	
	testing ("try various file locks on an ordinary file");
	start
		do_test (O_CREAT | O_RDWR | O_TRUNC, 0777);
	finis

#ifndef	SYSVR_2	/*  mandatory record locking does not exist in SVR2  */

	testing ("a file with mandatory record locking");
	testing ("should behave the same as an ordinary file");
	start
		do_test (O_CREAT | O_RDWR | O_TRUNC,
			S_ENFMT | S_IRUSR | S_IWUSR);
	finis


	testing ("a file with mandatory record locking and no delay");
	testing ("should behave the same as an ordinary file");
	start
		do_test (O_CREAT | O_RDWR | O_TRUNC | O_NDELAY,
			S_ENFMT | S_IRUSR | S_IWUSR);
	finis

#endif   /*  end of SVR3 test cases  */


	done ();
/*  NOTREACHED  */
}  /*  end of main  */

do_test (file_flag, file_mode)
int	file_flag, file_mode;
{
	int	i, retc, status;
	long	t;
	extern	long	time ();
	extern	void	gotalarm ();

	prep("set the umask and get the pid.");
	start
		expecting (VOID);
		umask (0);
		parent = getpid ();
	finis
/*  loop through all test cases  */
	for (i=0 ; i < NTESTS ; i++)	{
		prep ("open a temporary file");
		start
			fd = temp_open ("fcntl1", file_flag, file_mode);
			expecting (10);
			write (fd, FILEDATA, 10);
		finis
		prep("Initialize the structures.");
		start
			thiscase = &testcases [i];
			flock.l_type = thiscase -> a_type;
			flock.l_whence = thiscase -> a_whence;
			flock.l_start = thiscase -> a_start;
			flock.l_len = thiscase -> a_len;
		finis
		maketestingline ();
		testing ("fcntl (fd, F_SETLK, &flock) should succeed");
		start
			fcntl (fd, F_SETLK, &flock);
		finis
		if ((thiscase -> b_type) != SKIP){
			prep("Initialize another set of structures.");
			start
				flock.l_type = thiscase -> b_type;
				flock.l_whence = thiscase -> b_whence;
				flock.l_start = thiscase -> b_start;
				flock.l_len = thiscase -> b_len;
			finis
			maketestingline ();
			testing ("fcntl (fd, F_SETLK, &flock) should succeed");
			start
				fcntl (fd, F_SETLK, &flock);
			finis
		}
		if ((thiscase -> c_type) == SKIP){
			cleanup ("close the temp file");
			start
				expecting (SUCCESS);
				close (fd);
			finis
			continue;	/*  continue to the next case  */
		}
	/*  spawn a child process  */
		abstract ("spawning a child process");
		start

			if ((child = fork ()) == 0)	{
				exit_on (ERROR | SEVERR);
				child_proc ();
			}
		finis
		/*  parent process  */
		if ((thiscase -> c_flag) == WILLBLOCK) {

			testing("Make sure the child still exists.");
			start
				expecting (VOID);
				if (kill (child, 0) != 0)
					error ("child terminated prematurely, it should have blocked");

			finis
			testing("Wait for a signal from the child.");
			start
				if (got1 == 0) {
				/*
				 *  The child did not signal yet.
				 *  Set a 60 second alarm to break the pause
				 *  just in case the child doesn't terminate
				 *  on its own accord.
				 */
					signal (SIGALRM, gotalarm);
					expecting (VOID);
					alarm (GRACE);
					expecting (EINTR);  /*  pause for the SIGUSR1  */
					pause();
					expecting (VOID);
					alarm ((unsigned)0);	/*  turn off the alarm clock  */
					if (got1 != 1)
						error ("parent pause returned not due to SIGUSR1");
				}
				got1 = 0;  /*  reset the flag  */
			finis
			testing("Make sure that the child still exists.");
			start
				expecting (VOID);
				if (kill (child, 0) != 0)
					error ("child terminated prematurely, it should have blocked");
			finis
			testing ("the parent deletes all locks for the child to unblock");
			start
				flock.l_type = F_UNLCK;
				flock.l_whence = 0;
				flock.l_start = 0L;
				flock.l_len = 0L;
				expecting (SUCCESS);
				fcntl (fd, F_SETLK, &flock);
			finis
		}
		cleanup ("wait for the child to terminate and close the temp file");
		start
		/*
		 *  set a 60 second alarm to break the wait
		 *  just in case the child doesn't terminate on its own accord
		 */
			signal (SIGALRM, gotalarm);
			expecting (VOID);
			alarm (GRACE);
			expecting( child );
			wait (&status);
			expecting (VOID);
			alarm ((unsigned)0);	/*  turn off the alarm clock  */
			if (status != 0)
				error ("child returned status 0x%x", status);
			expecting (SUCCESS);
			close (fd);
		finis
	}  /*  end of loop  */
}  /*  end of parent process  */

child_proc (){  /*  child process  */


	flock.l_type = thiscase -> c_type;
	flock.l_whence = thiscase -> c_whence;
	flock.l_start = thiscase -> c_start;
	flock.l_len = thiscase -> c_len;
	flock.l_pid = 0;
	maketestingline ();

	if ((thiscase -> c_flag) == NOBLOCK)
		testing (
		"fcntl (fd, F_GETLK, &flock) should show that the lock will succeed");
	else
		testing ("%s\n%s",
		"fcntl (fd, F_GETLK, &flock) should show the parent's lock that will",
		"cause this lock to fail or block");

	start
		expecting (SUCCESS);
		fcntl (fd, F_GETLK, &flock);

		if ((thiscase -> c_flag) == NOBLOCK){
		/*
		 *  the lock should succeed, 
		 *  so the type is changed to F_UNLCK
		 *  but everything else remains unchanged
		 */
			if (flock.l_type != F_UNLCK)
				error ( "GETLK: type = %d, %d was expected", 
				  flock.l_type, F_UNLCK);
			if (flock.l_whence != thiscase -> c_whence)
				error ( "GETLK: whence = %d, should have remained %d",
				  flock.l_whence, thiscase -> c_whence);

			if (flock.l_start != thiscase -> c_start)
				error ("GETLK: start = %d, should have remained %d",
				  flock.l_start, thiscase -> c_start);

			if (flock.l_len != thiscase -> c_len)
				error ("GETLK: len = %d, should have remained %d",
				  flock.l_len, thiscase -> c_len);

			if (flock.l_pid != 0)
				error ("GETLK: pid = %d, should have remained 0",
				  flock.l_pid);
		}
		else {
			if (flock.l_pid != parent)
				error ("GETLK: pid = %d, should be parent's id of %d",
				  flock.l_pid,parent);

			if (flock.l_type != thiscase -> a_type)
				error ("GETLK: type = %d, should be parent's first lock type of %d",
				  flock.l_type, thiscase -> a_type);

			if (flock.l_whence != thiscase -> c_whence)
				error ("GETLK: whence = %d, should be %d",
				  flock.l_whence, thiscase -> c_whence);
		}

	finis

/*
 *  now try to set the lock, nonblocking
 *  This will succeed for NOBLOCK,
 *  fail for WILLBLOCK
 */
	flock.l_type = thiscase -> c_type;
	flock.l_whence = thiscase -> c_whence;
	flock.l_start = thiscase -> c_start;
	flock.l_len = thiscase -> c_len;
	flock.l_pid = 0;

	if ((thiscase -> c_flag) == NOBLOCK){
		expecting (SUCCESS);
		testing ("fcntl (fd, F_SETLK, &flock) should succeed");
	}
	else
	{
		expecting (VOID);
		testing ("fcntl (fd, F_SETLK, &flock) should fail");
	}
	start
		fcntl (fd, F_SETLK, &flock);

		if ((thiscase -> c_flag) == WILLBLOCK)
		{
		/*
		 *  check the errno
		 */
		if (errno != EACCES)
			if (errno != EAGAIN)
				error ("SETLK: errno = %s, EAGAIN or EACCES was expected", err_lookup (errno));
			else
				warning ("%s\nerrno of EAGAIN was returned",
				  FUTURDIR);

		testing (
		 "fcntl (fd, F_SETLKW, &flock) should succeed after blocking");
		start
			expecting (SUCCESS);
			/*  tell the parent to release the locks  */
			kill (parent, SIGUSR1);
			fcntl (fd, F_SETLKW, &flock);
		finis
		}
	finis
	exit( 0 );
/*  NOTREACHED  */
}  /*  end of child process  */

void	gotalarm ()
{
	/*
	 *  save the expecting value since a z_funct is being interrupted
	 */
	int	old_expect = sv_xenv->sv_expecting;

	continue_on (ERROR);
	expecting (VOID);
	if (kill (child, 0) == 0){
		kill (child, SIGKILL);
		error ("The child did not terminate on its own accord");
	sv_xenv->sv_expecting = old_expect;
	}
}



void	catch1 ()	/*  invoked on catching SIGUSR1  */
{
	/*
	 *  save the expecting value since a z_funct is being interrupted
	 */
	int	old_expect = sv_xenv->sv_expecting;

	expecting (SUCCESS);  /*  arrange for signal handling  */
	signal (SIGUSR1, catch1);
	sv_xenv->sv_expecting = old_expect;
	got1++;
}

maketestingline ()
{
char	*ptr, *ptr2;

switch (flock.l_type)
  {
  case F_RDLCK:
	ptr = "Making a read lock";
	break;

  case F_WRLCK:
	ptr = "Making a write lock";
	break;

  case F_UNLCK:
	ptr = "Unlocking";
	break;

  default:
	error ("maketestingline: unknown type = %d", flock.l_type);
	return;
  }

switch (flock.l_whence)
  {
  case 0:
	ptr2 = "from the beginning of the file";
	break;

  case 1:
	ptr2 = "from the current position in the file";
	break;

  case 2:
	ptr2 = "from the end of the file";
	break;

  default:
	error ("maketestingline: unknown type = %d", flock.l_type);
	return;
  }


testing ("%s %s starting at %ld",
		ptr, ptr2, flock.l_start);

if (flock.l_len == 0L)
	testing ("for the entire file");
else
	testing ("for %ld bytes", flock.l_len);

return;
}

#endif
