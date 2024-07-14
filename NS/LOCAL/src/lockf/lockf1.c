#ident	"@(#)lockf1.c	1.8"
#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
#ifndef	SYSVR_2
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>

#define	SLEEP	0
#define	AXIS	1
#define	REL		15	/*15 second release time*/
#define	HOLD	5	/*5 second delay*/
#define	MODE	(S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP | S_ENFMT)
#define	GRACE	60

time_t	time();
void	deadlock();

char	path[512];

static	int	pid;

char	*err_lookup ();

main(argc,argv)
int		argc;
char	*argv;
{
	setup("lockf");
	funct("lockf");

	tstlock();
	tstblock();
	tstddlk();
	done();
/*NOTREACHED*/
}

int		tstlock()
{
	int		fd;

	abstract("This part tests whether enforcement record locking");
	abstract("causes F_TLOCK to fail with errno set to EACCES (or");
	abstract("EAGAIN) when called for a locked file.  It creates");
	abstract("a file with enforcement record locking turned on, and");
	abstract("locks it.  It then waits a while and unlocks the file.");
	abstract("meanwhile a child tries to lock the same file.  The");
	abstract("child should get a failure with EACCES (or EAGAIN)");
	abstract("as an errno.");
	start
		prep("Create a file with enforcement record locking turned on.");
		start
			strcpy(path,temp_path());
			strcat(path,"eaccess");
			fd = creat(path,MODE);
		finis
		prep("Lock the file.");
		start
			lockf(fd,F_LOCK,(long)0);
		finis
		prep("fork a child to test whether lockf() will fail.");
		start
			if(!fork()){
				checklock(fd);
				exit(0);
			}
		finis
		cleanup("Wait for the child to terminate.");
		start
			wait((int *)0);
			close(fd);
		finis
	finis
}

int	checklock(fd)
int		fd;
{
	testing("Attempt to lock the file that the parent already locked.");
	testing("lockf() should fail with EACCES or EAGAIN.");
	start
		expecting(FAILURE);
		lockf(fd,F_TLOCK,(long)0);
		if(errno != EACCES){
#ifdef	EAGAIN
			if(errno == EAGAIN){
				warning(FUTURDIR);
			}
			else
#endif
			    error("Incorrect errno: got %s, expected EACCES.",
				err_lookup (errno));
		}
	finis
}

int		tstblock()
{
	int		fd;
	time_t	rel;

	abstract("This part tests whether enforcement record locking");
	abstract("causes F_LOCK to sleep on a locked file.  It creates");
	abstract("a file with enforcement record locking turned on, and");
	abstract("locks it.  It then waits a while and unlocks the file.");
	abstract("meanwhile a child is trying to lock the same file.  The");
	abstract("child should block until the parent is finished.");
	start
		prep("Create a file with enforcement record locking turned on.");
		start
			strcpy(path,temp_path());
			strcat(path,"block");
			fd = creat(path,MODE);
		finis
		prep("Lock the file.");
		start
			lockf(fd,F_LOCK,(long)0);
		finis
		prep("fork a child to test whether lockf() will sleep.");
		start
			rel = time((time_t *)0) + REL;
			if(!fork()){
				checkblock(fd,rel);
				exit(0);
			}
		finis
		while(time((time_t *)0) < rel);
		prep("Now that the agreed upon time has arrived, release the");
		prep("lock on the file.");
		start
			lockf(fd,F_ULOCK,(long)0);
		finis
		cleanup("Wait for the child to terminate.");
		start
			wait((int *)0);
		finis
	finis
}

int	checkblock(fd,rel)
int		fd;
time_t	rel;
{
	void	(*stuck)();
	prep("Set up an alarm to wake up process if it hangs.");
	start
		signal(SIGALRM,stuck);
		expecting (VOID);
		alarm (GRACE);
	finis
	testing("Attempt to lock the file that the parent already locked.");
	testing("The process should sleep until the parent unlocks the file.");
	start
		lockf(fd,F_LOCK,(long)0);
		if(time((time_t)0) < rel){
			abort_on(ERROR);
			error("Child did not sleep on locked file, cannot continue.");
		}
	finis
}

static	int		fd1,fd2;

int		tstddlk()
{
	void	(*stuck)();
	time_t	rel;

	abstract("This part tests the EDEADLK condition of lockf().");
	abstract("It locks a file with enforcement mode locking turned on");
	abstract("and forks a child which locks a different file.");
	abstract("The child then locks the first file, causing it to block.");
	abstract("The parent then attempts to lock the child's file");
	abstract("which should cause a deadlock condition.");
	abstract("lockf() should set errno to EDEADLK in this situation.");
	start
		prep("Create two files with enforcement mode record locking");
		prep("turned on in their modes.");
		start
			strcpy(path,temp_path());
			strcat(path,"deadlock1");
			fd1 = creat(path,MODE);
			chmod(path,MODE);
			strcpy(path,temp_path());
			strcat(path,"deadlock2");
			fd2 = creat(path,MODE);
			chmod(path,MODE);
		finis
		prep("Lock one file.");
		start
			lockf(fd1,F_LOCK,(long)0);
		finis
		prep("Fork a child to lock the second file and block on the");
		prep("first file.");
		start
			if(!(pid = fork())){
				rel  = time((time_t *)0) + REL;
				prep("Lock the second file.");
				start
					lockf(fd2,F_LOCK,(long)0);
				finis
				prep("Set up an alarm to wake up process if it hangs.");
				start
					signal(SIGALRM,stuck);
					expecting (VOID);
					alarm (GRACE);
				finis
				prep("Attempt lock first file, process should block.");
				start
					lockf(fd1,F_LOCK,(long)0);
				finis
				cleanup("Unlock the files.");
				start
					lockf(fd2,F_ULOCK,(long)0);
					lockf(fd1,F_ULOCK,(long)0);
				finis
				exit(0);
			}
			prep("Wait until the child has the second file locked");
			start
				expecting(VOID);
				while(!lockf(fd2,F_TEST,(long)0));
			finis
			prep("Give child time to become blocked.");
			start
				rel = time((time_t *)0) + REL;
				while(time((time_t *)0) < rel);
			finis
			prep("Set up an alarm to wake the test up from a deadlock");
			prep("if necessary.");
			start
				signal(SIGALRM,deadlock);
				expecting(VOID);
				alarm (GRACE);
			finis
			testing("Attempt to lock the second file which should be");
			testing("locked by the child at this point.  This should");
			testing("create a deadlock situation, and cause errno to be");
			testing("set to EDEADLK.  If this fails the alarm will stop");
			testing("the deadlock.");
			start
				expecting(EDEADLK);
				lockf(fd2,F_LOCK,(long)0);
			finis
			cleanup("Unlock first file and wait for child to exit.");
			start
				lockf(fd1,F_ULOCK,(long)0);
				close(fd1);
				wait((int *)0);
			finis			
		finis
	finis
}

void	stuck(sig)
{
	error("Test case hung.  Timeout reached.");
	done();
}

void	deadlock(sig)
int		sig;
{
	error("EDEADLK not returned, deadlock created.");
	cleanup("Unlock first file and wait for child to exit.");
	start
		lockf(fd1,F_ULOCK,(long)0);
		close(fd1);
		wait((int *)0);
	finis
	done();
}
#else
main(argc,argv)
int		argc;
char	*argv[];
{
	unsupported("Enforcement mode record/file locking not supported in System V Release 2");
	done();
}
#endif
