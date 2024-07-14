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

#ident		"@(#)lockf3.c	1.9"

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
 *  sleeping on writes due to write locks.
 *
 *  Most of this test is the same for:
 *	fcntl
 *	lockf
 *	read
 *	write
 *  so if you change one, check with the others.
 *
 *  This is a test for three-way deadlock detection
 *
 *  This involves three processes, named A, B and C.
 *  They collide locking three bytes in a file as follows:
 *
 *  A locks byte 1 and succeeds
 *  B locks byte 3 and succeeds
 *  B locks byte 1 and blocks (waiting for 'A')
 *  C locks byte 2 and succeeds
 *  C locks byte 3 and blocks (waiting for 'B')
 *  A locks byte 2 and gets EDEADLK
 *
 *
 *  Here are some possible ways for this test to fail:
 *
 *  "child A did not die on its own accord" with
 *  "child B did not die on its own accord" with
 *  "child C did not die on its own accord"
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
 *
 *  Child C's problems are similar to those of child B
 */


#ifndef	LOCKF

main()	/*  dummy main routine  */
{
	setup ("lockf");
	funct ("lockf");
	unsupported("Record locking is not supported in this system.");
}

#else


static	int	got1 = 0;
static	int	got2 = 0;
static	int	pidA = 1;
static	int	pidB = 1;
static	int	pidC = 1;

main ()
{
int	fd, retc;
long	t;
extern	void	catch1(), catch2 (), gotalarm (), dummy();
extern	long	time ();
char	path [PATH_MAX];


setup("lockf");
funct("lockf");
testing ("test lockf for a three way deadlock");
start

signal (SIGUSR1, catch1);
signal (SIGUSR2, catch2);

expecting (VOID);
strcpy (path, sv_temp_path ());
strcat (path, FILENAME);
umask (0);
expecting (SUCCESS);
fd = creat (path, FILEMODE);

testing ("spawn child A");
start
if ((pidA = fork()) == 0)
	{
	/*  this is child 'A'  */
	expecting (SUCCESS);
	lseek (fd, 1L, 0);
	lockf (fd, F_LOCK, 1L);
	kill (getppid (), SIGUSR1);	/*  tell parent that lock is done  */
	expecting (EINTR);
	pause ();	/*  wait for parent to signal next lock  */

	if (got2 == 0)
		error ("pause returned in child A for unknown reasons");

	expecting (SUCCESS);
	lseek (fd, 2L, 0);
	expecting (EDEADLK);
	lockf (fd, F_LOCK, 1L);
	done ();
	/*NOTREACHED*/
	}
finis

/*  parent waits for child 'A' to create the lock  */
if (got1 == 0)	/*  did child A signal yet?  */
	{
	/*
	 *  set a 60 second alarm to break the pause
	 *  just in case child A never sends the signal.
	 */
	signal (SIGALRM, dummy);

	expecting (VOID);
	alarm (GRACE);

	expecting (EINTR);  /*  no - pause for the SIGUSR1  */
	pause ();

	expecting (VOID);
	alarm ((unsigned)0);	/*  turn off the alarm clock  */

	if (got1 != 1)
		error ("parent: pause returned not due to SIGUSR1");
	}

testing ("spawn child B");
start
if ((pidB = fork ()) == 0)
	{
	/*  this is child 'B'  */
	expecting (SUCCESS);
	lseek (fd, 3L, 0);
	lockf (fd, F_LOCK, 1L);

	lseek (fd, 1L, 0);
	lockf (fd, F_LOCK, 1L);
	done ();
	}
finis


/*
 *  wait for child 'B' to get up to blocking on the lock.
 *  I must do a busy wait because
 *	a. there is no way for child 'B' to inform the parent that it is sleeping
 *	b. using sleep () uses SIGALRM which have side-effects
 */
t = time ((long *) 0) + 5L;
while (t > time ((long *) 0))
	;

/*  make sure that child A still exists  */
expecting (VOID);
if (kill (pidA, 0) != 0)
	error ("child A terminated prematurely");

/*  make sure that child B still exists  */
expecting (VOID);
if (kill (pidB, 0) != 0)
	error ("child B did not block on the second lockf");

testing ("spawn child C");
start
if ((pidC = fork ()) == 0)
	{
	/*  this is child 'C'  */
	expecting (SUCCESS);
	lseek (fd, 2L, 0);
	lockf (fd, F_LOCK, 1L);
	lseek (fd, 3L, 0);
	lockf (fd, F_LOCK, 1L);

	done ();
	/*  NOTREACHED  */
	}
finis


/*  wait for child C to block  */
t = time ((long *) 0) + 5L;
while (t > time ((long *) 0))
	;

/*  make sure that child A still exists  */
expecting (VOID);
if (kill (pidA, 0) != 0)
	error ("child A terminated prematurely");

/*  make sure that child B still exists  */
expecting (VOID);
if (kill (pidB, 0) != 0)
	error ("child B did not block on the second lockf");

/*  make sure that child C still exists  */
expecting (VOID);
if (kill (pidC, 0) != 0)
	error ("child C did not block on the second lockf");

/*  tell child 'A' to make the last lock  */
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
	pidA = 1;	/*  check off child A  */
else
	{
	if (retc == pidB)
		pidB = 1;	/*  check off child B   */
	else
		if (retc == pidC)
			pidC = 1;	/*  check off child C  */
		else
			error ("wait returned %d, which is not any child's pid",
			retc);
	}

retc = wait ((int *) 0);
if (retc == pidA)
	pidA = 1;	/*  check off child A  */
else
	{
	if (retc == pidB)
		pidB = 1;	/*  check off child B   */
	else
		if (retc == pidC)
			pidC = 1;	/*  check off child C  */
		else
			error ("wait returned %d, which is not any child's pid",
			retc);
	}

retc = wait ((int *) 0);
expecting (VOID);
alarm ((unsigned)0);	/*  turn off the alarm clock  */
if (retc == pidA)
	pidA = 1;	/*  check off child A  */
else
	{
	if (retc == pidB)
		pidB = 1;	/*  check off child B   */
	else
		if (retc == pidC)
			pidC = 1;	/*  check off child C  */
		else
			error ("wait returned %d, which is not any child's pid",
			retc);
	}

finis
done ();
}

void	catch1 ()
{got1 = 1;}



void	catch2 ()
{got2 = 1;}



void dummy()
{
message ("Timer expired.");
}



void	gotalarm ()
{
continue_on (ERROR);
expecting (VOID);
if ((pidA != 1) && (kill (pidA, 0) == 0))
	{
	kill (pidA, SIGKILL);
	error ("child A did not die on its own accord");
	}

if ((pidB != 1) && (kill (pidB, 0) == 0))
	{
	kill(pidB, SIGKILL);
	error ("child B did not die on its own accord");
	}

if ((pidC != 1) && (kill (pidC, 0) == 0))
	{
	kill(pidC, SIGKILL);
	error ("child C did not die on its own accord");
	}
done ();
/* NOTREACHED */
}

#endif
