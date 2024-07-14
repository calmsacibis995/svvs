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

#ident		"@(#)read2.c	1.10"



#ifdef	SYSVR_2	/*  record locking does not exist for SVR2  */

main()	/*  dummy main routine  */
{
	setup ("read");
	funct ("read");
	unsupported("Manditory record locking is not supported in System V release 2.");
}

#else


/*
 *  wait GRACE seconds before assuming the children will never wake up
 */
#define	GRACE	(unsigned) 60
#define	FILENAME	"morelocks"
#define	FILEMODE	S_IRUSR | S_IWUSR | S_ENFMT

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
 *  "Child A did not die on its own accord" with
 *  "Child B did not die on its own accord"
 *	this means that the call to read in child A never
 *	returned. The deadlock was not detected.
 *
 *  "Child A terminated prematurely"
 *	child A should have waited for a signal from the parent.
 *	The pause() returned (and should give the error
 *	"pause returned in child A for unknown reasons"),
 *	and the read returned immediately,
 *	both of which are incorrect actions.
 *
 *  "Child B did not block on the second fcntl"
 *	The read() should have blocked because its byte
 *	is locked by child A, and the N_DELAY flag is NOT set.
 *
 *  "Child B did not die on its own accord"
 *	The call to read() never woke up.  It blocked because
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
 *	read will succeed where it should get deadlock.
 *	If child A continues between child B's calls to fcntl and read,
 *	then A's locking byte 2 will block until child B terminates
 *	which will terminate because B's read call will fail
 *	with deadlock.
 */
static	int	got1 = 0;
static	int	got2 = 0;
static	int	pidA = 1;
static	int	pidB = 1;


main()
{


int	fd, retc;
long	t;
char	path [PATH_MAX];
char	tinybuf [1];
struct	flock	flock;
extern	long	time ();
extern	void	catch1(), catch2(), gotalarm(), dummy();

setup("read");
testing ("Test read for a 2-way deadlock condition");
start

exit_on (ERROR | SEVERR);

expecting (SUCCESS);  /*  arrange for signal handling  */
signal (SIGUSR1, catch1);
signal (SIGUSR2, catch2);

expecting (VOID);
strcpy (path, sv_temp_path ());
strcat (path, FILENAME);
umask (0);
expecting (SUCCESS);
fd = open (path, O_RDWR | O_CREAT, FILEMODE);

expecting (6);
write (fd, "hello",6);

flock.l_type = F_WRLCK;	/*  write locks desired  */
flock.l_whence = 0;	/*  offset from file beginning  */
flock.l_len = 1L;	/*  locking 1 byte at a time  */

testing ("Spawn child A");
start
if ((pidA = fork()) == 0)	/*  child 'A' */
	{
	/*  lock byte 1  */
	expecting (SUCCESS);
	flock.l_start = 1L;
	fcntl (fd, F_SETLK, &flock);
	/*  inform the parent that byte 1 has been locked  */
	kill (getppid(),SIGUSR1);

	/* wait for child B to block (sleep on lock)  */
	expecting (EINTR);
	pause();
	if (got2 == 0)
		error ("Pause returned in child A for unknown reasons");

	expecting (2L);
	lseek (fd, 2L, 0);
	expecting (EDEADLK);
	read (fd, tinybuf, 1);
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
		error ("Parent pause returned not due to SIGUSR1");
	}


testing ("Spawn child B");
start
if ((pidB = fork()) == 0)	/*  child 'B' */
	{
	/*  lock byte 2  */
	expecting (SUCCESS);
	flock.l_start = 2L;
	fcntl (fd, F_SETLK, &flock);

	/*  lock byte 1.  This should block, until child A terminates,
	 thus unlocking byte 1 */
	expecting (1L);
	lseek (fd, 1L, 0);
	expecting (SUCCESS);
	read (fd, tinybuf, 1);
	done ();
	/*NOTREACHED*/
	}
finis


/*  wait for B to get to blocking  */
t = time ((long *)0) + 5;
while (t > time ((long *)0))
	;

/*  make sure that child A still exists  */
expecting (VOID);
if (kill (pidA, 0) != 0)
	error ("Child A terminated prematurely");

/*  make sure that child B still exists  */
expecting (VOID);
if (kill (pidB, 0) != 0)
	error ("Child B did not block on the second read");

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
		error ("Wait returned %d, which is not either child's pid",
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
		error ("Wait returned %d, which is not either child's pid",
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
message ("The watchdog timer has bitten");
}



void	gotalarm()
{
continue_on (ERROR);
expecting (VOID);
/*
 *  If child A was spawned, and it still exists
 *  then kill it and report that it didn't terminate in its own.
 */
if ((pidA != 1) && (kill (pidA, 0) == 0))
	{
	kill (pidA, SIGKILL);
#ifdef SV_RFS
	warning("%s\nChild A did not die on its own accord", ADVISORY );
#else
	error ("Child A did not die on its own accord");
#endif
	}
/*
 *  If child B was spawned, and it still exists
 *  then kill it and report that it didn't terminate in its own.
 */
if ((pidB != 1) && (kill (pidB, 0) == 0))
	{
	kill (pidB, SIGKILL);
#ifdef SV_RFS
	warning("%s\nChild B did not die on its own accord", ADVISORY );
#else
	error ("Child B did not die on its own accord");
#endif
	}
done ();
/*NOTREACHED*/
}



#endif
