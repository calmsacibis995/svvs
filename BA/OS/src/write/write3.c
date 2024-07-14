#include	<fcntl.h>
#ifdef	LOCKF
#include	<unistd.h>
#endif
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/stat.h>	/*  needed for file modes  */

#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident		"@(#)write3.c	1.10"

#ifdef	SYSVR_2	/*  record locking does not exist for SVR2  */
main()	/*  dummy main routine  */
{
	setup ("write");
	funct ("write");
	unsupported("Manditory record locking is not supported in system V release 2.");
}

#else


/*
 *  wait GRACE seconds before assuming the children will never wake up
 */
#define	GRACE	(unsigned) 60
#define	FILENAME	"morelock"
#define	FILEMODE	S_IRUSR | S_IWUSR | S_ENFMT

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
 *  "Child A did not die on its own accord" with
 *  "Child B did not die on its own accord" with
 *  "Child C did not die on its own accord"
 *	this means that the call to write in child A never
 *	returned. The deadlock was not detected.
 *
 *  "Child A terminated prematurely"
 *	child A should have waited for a signal from the parent.
 *	The pause() returned (and should give the error
 *	"Pause returned in child A for unknown reasons"),
 *	and the write returned immediately,
 *	both of which are incorrect actions.
 *
 *  "Child B did not block on the second fcntl"
 *	The write() should have blocked because its byte
 *	is locked by child A, and the N_DELAY flag is NOT set.
 *
 *  "Child B did not die on its own accord"
 *	The call to write() never woke up.  It blocked because
 *	child A locked that byte, but didn't wake up when the lock was
 *	removed.  Perhaps the _exit routine did not release child A's
 *	locks (since they are not explicitly removed by child A).
 *  "Parent pause returned not due to SIGUSR1"
 *	This could be caused by the alarm clock timing out,
 *	meaning that child A did not get to send a signal to the parent
 *	(perhaps the fcntl call blocked or caused termination),
 *	or the parent did not receive the signal.
 *	Or it could be caused by a stray signal being caught.
 *
 *  If the 5 second wait is not sufficient for child B to make both locks
 *	then various problems can occur when child A resumes.
 *	If child A continues before child B does anything then A's
 *	write will succeed where it should get deadlock.
 *	If child A continues between child B's calls to fcntl and write,
 *	then A's locking byte 2 will block until child B terminates
 *	which will terminate because B's write call will fail
 *	with deadlock.
 *
 *  Child C's problems are similar to those of child B
 */

static	int	got1 = 0;
static	int	got2 = 0;
static	int	pidA = 1;
static	int	pidB = 1;
static	int	pidC = 1;

main ()
{
int	fd, retc;
long	t;
struct	flock	flock;
extern	void	catch1(), catch2 (), gotalarm (), dummy();
extern	long	time ();
char	path [PATH_MAX];


setup("write");
funct("write");
testing ("Test write for a three way deadlock condition");
start

signal (SIGUSR1, catch1);
signal (SIGUSR2, catch2);

expecting (VOID);
strcpy (path, sv_temp_path ());
strcat (path, FILENAME);
umask (0);
expecting (SUCCESS);
fd = open (path, O_RDWR | O_CREAT, FILEMODE);

flock.l_type = F_RDLCK;	/*  read locks desired  */
flock.l_whence = 0;	/*  offset from file beginning  */
flock.l_len = 1L;	/*  locking 1 byte at a time  */

testing ("Spawn child A");
start
if ((pidA = fork()) == 0)
	{
	/*  this is child 'A'  */
	expecting (SUCCESS);
	flock.l_start = 1L;
	fcntl (fd, F_SETLK, &flock);
	kill (getppid (), SIGUSR1);	/*  tell parent that lock is done  */
	expecting (EINTR);
	pause ();	/*  wait for parent to signal next lock  */

	if (got2 == 0)
		error ("Pause returned in child A for unknown reasons");

	expecting (2L);
	lseek (fd, 2L, 0);
	expecting (EDEADLK);
	write (fd, "a", 1);
	done ();
	/*NOTREACHED*/
	}
finis


/*  parent  */
/*  wait for child 'A' to create the lock  */
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
		error ("Parent: pause returned not due to SIGUSR1");
	}


testing ("Spawn child B");
start
if ((pidB = fork ()) == 0)
	{
	/*  this is child 'B'  */
	expecting (SUCCESS);

	flock.l_start = 3L;
	fcntl (fd, F_SETLK, &flock);

	lseek (fd, 1L, 0);
	expecting (SUCCESS);
	write (fd, "a", 1);
	done ();
	/*NOTREACHED*/
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
	error ("Child A terminated prematurely");

/*  make sure that child B still exists  */
expecting (VOID);
if (kill (pidB, 0) != 0)
	error ("Child B did not block on the second write");

testing ("Spawn child C");
start
if ((pidC = fork ()) == 0)
	{
	/*  this is child 'C'  */
	expecting (SUCCESS);
	flock.l_start = 2L;
	fcntl (fd, F_SETLK, &flock);

	lseek (fd, 3L, 0);
	expecting (SUCCESS);
	write (fd, "b", 1);
	done ();
	/*NOTREACHED*/
	}
finis

/*  wait for child C to do its thing  */
t = time ((long *) 0) + 5L;
while (t > time ((long *) 0))
	;

/*  make sure that child A still exists  */
expecting (VOID);
if (kill (pidA, 0) != 0)
	error ("Child A terminated prematurely");

/*  make sure that child B still exists  */
expecting (VOID);
if (kill (pidB, 0) != 0)
	error ("Child B did not block on the second write");

/*  make sure that child C still exists  */
expecting (VOID);
if (kill (pidC, 0) != 0)
	error ("Child C did not block on the second write");

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
			error ("Wait returned %d, which is not any child's pid",
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
			pidC = 1;
		else
			error ("Wait returned %d, which is not any child's pid",
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
			pidC = 1;
		else
			error ("Wait returned %d, which is not any child's pid",
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
message ("The watchdog timer has bitten");
}



void	gotalarm ()
{
continue_on (ERROR);
expecting (VOID);
if ((pidA != 1) && (kill (pidA, 0) == 0))
	{
	kill (pidA, SIGKILL);
#ifdef SV_RFS
	warning("%s\nChild C did not die on its own accord", ADVISORY );
#else
	error ("Child A did not die on its own accord");
#endif
	}

if ((pidB != 1) && (kill (pidB, 0) == 0))
	{
	kill(pidB, SIGKILL);
#ifdef SV_RFS
	warning("%s\nChild B did not die on its own accord", ADVISORY );
#else
	error ("Child B did not die on its own accord");
#endif
	}

if ((pidC != 1) && (kill (pidC, 0) == 0))
	{
	kill(pidC, SIGKILL);
#ifdef SV_RFS
	warning("%s\nChild C did not die on its own accord", ADVISORY );
#else
	error ("Child C did not die on its own accord");
#endif
	}
done ();
/* NOTREACHED */
}
#endif
