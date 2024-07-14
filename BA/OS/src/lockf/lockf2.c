#include	<fcntl.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/stat.h>	/*  needed for file modes  */

#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifdef	LOCKF
#include	<unistd.h>
#endif

#ident		"@(#)lockf2.c	1.9"

/*
 *  wait GRACE seconds before assuming the children will never wake up
 */
#define	GRACE	(unsigned) 60
#define	FILENAME	"locks"
#ifndef	SYSVR_2

#define	FILEMODE	S_IWUSR

#else

#define	FILEMODE	0200

#endif

/*
 *  This tests for the deadlock situation.
 *  The only way to cause a deadlock is to have processes
 *  sleeping on read or write locks.
 *
 *  Most of this test is the same for:
 *	fcntl
 *	lockf
 *	read
 *	write
 *  so if you change one, check with the others.
 *
 *  This is a test for two-way deadlock detection
 *
 *  This involves two processes, named A and B.
 *  They collide locking two bytes in a file as follows:
 *
 *  A locks byte 1 and succeeds
 *  B locks byte 2 and succeeds
 *  B locks byte 1 and blocks
 *  A locks byte 2 and gets EDEADLK
 *
 *
 *  Here are some possible ways for this test to fail:
 *
 *  "child A did not die on its own accord" with
 *  "child B did not die on its own accord"
 *	this means that the second call to lockf in child A never
 *	returned. The deadlock was not detected.
 *
 *  "child A terminated prematurely"
 *	child A should have waited for a signal from the parent.
 *	The pause() returned (and should give the error
 *	"pause returned in child A for unknown reasons"),
 *	and the second lockf returned immediately,
 *	both of which are incorrect actions.
 *
 *  "child B did not block on the second lockf"
 *	The second lockf() should have blocked because its byte
 *	is locked by child A, and the N_DELAY flag is NOT set.
 *
 *  "child B did not die on its own accord"
 *	The second call to lockf() never woke up.  It blocked because
 *	child A locked that byte, but didn't wake up when the lock was
 *	removed.  Perhaps the _exit routine did not release child A's
 *	locks (since they are not explicitly removed by child A).
 *  "parent pause returned not due to SIGUSR1"
 *	This could be caused by the alarm clock timing out,
 *	meaning that child A did not get to send a signal to the parent
 *	(perhaps the first lockf call blocked or caused termination),
 *	or the parent did not receive the signal.
 *	Or it could be caused by a stray signal being caught.
 *
 *  If the 5 second wait is not sufficient for child B to make both locks
 *	then various problems can occur when child A resumes.
 *	If child A continues before child B does anything then A's second
 *	lockf will succeed where it should get deadlock.
 *	If child A continues between child B's calls to lockf,
 *	then A's locking byte 2 will block until child B terminates
 *	which will terminate because B's second lockf call will fail
 *	with deadlock.
 */
static	int	got1 = 0;
static	int	got2 = 0;
static	int	pidA = 1;
static	int	pidB = 1;

#ifndef	LOCKF

main()	/*  dummy main routine  */
{
	setup ("lockf");
	funct ("lockf");
	unsupported("Record locking is not supported in this system.");
}

#else


main()
{
int	fd, retc;
long	t;
char	path [PATH_MAX];
extern	long	time ();
extern	void	catch1(), catch2(), gotalarm(), dummy();

setup("lockf");
funct("lockf");
testing ("test lockf for a 2-way deadlock condition");
start
exit_on (ERROR | SEVERR);

expecting (SUCCESS);  /*  arrange for signal handling  */
signal (SIGUSR1, catch1);
signal (SIGUSR2, catch2);

expecting (VOID);
strcpy (path, sv_temp_path ());
strcat (path, FILENAME);
expecting (SUCCESS);
fd = creat (path, FILEMODE);

expecting (6);
write (fd, "hello",6);

expecting (0L);
lseek (fd, 0L, 0);

testing ("spawn child A");
start
if ((pidA = fork()) == 0)	/*  child 'A' */
	{
	/*  lock byte 1  */
	expecting (SUCCESS);
	lockf (fd, F_LOCK, 1L);
	/*  inform the parent that byte 1 has been locked  */
	kill (getppid(),SIGUSR1);

	/* wait for child B to block (sleep on lock)  */
	expecting (EINTR);
	pause();
	if (got2 == 0)
		error ("pause returned in child A for unknown reasons");
	/*
	 *  we have the write lock on the first byte,
	 *  so we should be able to write it and not block.
	 *  This has the desired effect of advancing the file pointer
	 *  to the second byte
	 *  (not needed in fcntl test)
	 *  If this blocks, the parent will give an error that
	 *  the child did not terminate
	 */
	expecting (1);
	write (fd, "a", 1);

	expecting (EDEADLK);
	lockf (fd, F_LOCK, 1L);
	done ();
	/*NOTREACHED*/
	}
finis


/* parent  */
if (got1 == 0)	/*  did child A signal yet?  */
	{
	/*
	 *  set a 60 second alarm to break the pause
	 *  just in case child A never sends the signal.
	 */
	expecting (VOID);
	signal (SIGALRM, dummy);
	alarm (GRACE);

	expecting (EINTR);  /*  no - pause for the SIGUSR1  */
	pause();

	expecting (VOID);
	alarm ((unsigned)0);	/*  turn off the alarm clock  */

	if (got1 != 1)
		error ("parent pause returned not due to SIGUSR1");
	}


testing ("spawn child B");
start
if ((pidB = fork()) == 0)	/*  child 'B' */
	{
	/*  lock byte 2  */
	expecting (SUCCESS);
	lseek (fd, 1L, 0);
	lockf (fd, F_LOCK, 1);

	/*  lock byte 1.  This should block, until child A terminates,
	 thus unlocking byte 1 */
	lseek (fd, 0L, 0);
	lockf (fd, F_LOCK,1);
	done ();
	/*  NOTREACHED  */
	}
finis

/*  wait for B to get to blocking  */
t = time ((long *)0) + 5;
while (t > time ((long *)0))
	;

/*  make sure that child A still exists  */
expecting (VOID);
if (kill (pidA, 0) != 0)
	error ("child A terminated prematurely");

/*  make sure that child B still exists  */
expecting (VOID);
if (kill (pidB, 0) != 0)
	error ("child B did not block on the second lockf");

/*  tell child A to resume  */
expecting (SUCCESS);
kill (pidA, SIGUSR2);

/*
 *  set a 60 second alarm to break the wait
 *  just in case the children don't terminate on their own accord
 */
signal (SIGALRM, gotalarm);

expecting (VOID);
alarm (GRACE);

retc = wait ((int *) 0);
if (retc == pidA)
	{
	pidA = 1;	/*  check off child A  */
	}
else
	{
	if (retc == pidB)
		{
		pidB = 1;	/*  check off child B   */
		}
	else
		error ("wait returned %d, which is not either child's pid",
			retc);
	}

retc = wait ((int *) 0);

expecting (VOID);
alarm ((unsigned)0);	/*  turn off the alarm clock  */
if (retc == pidA)
	{
	pidA = 1;	/*  check off child A  */
	}
else
	{
	if (retc == pidB)
		{
		pidB = 1;	/*  check off child B   */
		}
	else
		error ("wait returned %d, which is not either child's pid",
			retc);
	}

finis
done ();
}



void catch1()
{got1 = 1;}



void catch2()
{got2 = 1;}



void dummy()
{
message ("Timer expired.");
}



void	gotalarm()
{
continue_on (ERROR);
expecting (VOID);
/*
 *  If child A was spawned, and it still exists
 *  then kill it and report that it didn't terminate on its own.
 */
if ((pidA != 1) && (kill (pidA, 0) == 0))
	{
	kill (pidA, SIGKILL);
	error ("child A did not die on its own accord");

	/*
	 *  If this happens, look at the temp file ../tmp/dead1.dir/locks.
	 *  If it contains 'hello', then the write in child A blocked.
	 *  If it contains 'aello', then the lockf in child A blocked.
	 */
	}
/*
 *  If child B was spawned, and it still exists
 *  then kill it and report that it didn't terminate in its own.
 */
if ((pidB != 1) && (kill (pidB, 0) == 0))
	{
	kill (pidB, SIGKILL);
	error ("child B did not die on its own accord");
	}
done ();
/*NOTREACHED*/
}

#endif
