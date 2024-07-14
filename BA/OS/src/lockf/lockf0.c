#include <sv_base.h> /*replacement macros */

#ifndef	LOCKF
main()
{
	exit (0);
}
#else
#include	<unistd.h>

#include	<errno.h>
#include	<signal.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#include	<sys/types.h>
#ifdef	LOCKF
#include	<unistd.h>
#endif

#ident	"@(#)lockf0.c	1.12"

# define	LIMIT	15
# define	YES	1
# define	NO	0

char	*err_lookup ();

/*
** lockf
**	Test the lockf function.
**
*/

main()
{
	int	(*sts)();
	int	fildes;
	int	i, size, overlap, pid;
	time_t	rel;
	time_t	time();
	int	lockf();
	int	fork();
	void	check_lockf();
	long	lseek();
	void	exit();
	siggy	catch_sig();

	setup("lockf");

	funct("lockf");

	exit_on ( SEVERR );

	abstract("From every position in a file incrementally lock");
	abstract("and unlock all portions of the file.");
	abstract("Expect return values of zero  (Advisory only).");
	start

		prep("Open a data file.");
		start
			expecting ( SUCCESS );
			fildes = data_open("lockf_data210", O_RDWR, 0L);
		finis

		testing("Issue the locks.");
		start
			for(i = 0; i <= LIMIT; i++)
			{
				expecting( i );
				lseek(fildes, (long)i, SEEK_SET);

				for(size = 0; size <= LIMIT; size++)
				{

					expecting ( SUCCESS );
					lockf(fildes, F_LOCK, (long)size);

					overlap = NO;
					check_lockf(i, size, overlap);

					expecting ( SUCCESS );
					lockf(fildes, F_ULOCK, (long)size);

				}
			}
		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis

	finis


	abstract("From every position in a file place incremental");
	abstract("overlapping locks to all other forward positions in");
	abstract("the file.");
	abstract("Expect return values of zero  (Advisory only).");
	start

		prep("Open the data file.");
		start
			expecting ( SUCCESS );
			fildes = data_open("lockf_data210", O_RDWR, 0);
		finis

		testing("Issue the locks.");
		start
			for(i = 0; i <= LIMIT; i++)
			{
				expecting( i );
				lseek(fildes, (long)i, SEEK_SET);

				for(size = 0; size <= LIMIT; size++)
				{

					expecting ( SUCCESS );
					lockf(fildes, F_LOCK, (long)size);

					overlap = YES;
					check_lockf(i, size, overlap);

				}
			}
		finis

		cleanup("Close the data file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis

	finis


	abstract("Lock the whole file then unlock one byte in the");
	abstract("center of the file.  Two locks should remain.");
	start

		prep("Open the data file.");
		start
			expecting ( SUCCESS );
			fildes = data_open("lockf_data210", O_RDWR, 0);
		finis

		testing("Seek to the start of the file and lock it all.");
		start
			expecting ( 0L );
			lseek(fildes, 0L, SEEK_SET);

			expecting ( SUCCESS );
			lockf(fildes, F_LOCK, 0L);
		finis

		testing("Seek to the middle of the file and unlock one byte.");
		start
			expecting( LIMIT/2 );
			lseek(fildes, (long)LIMIT/2, SEEK_SET);

			expecting ( SUCCESS );
			lockf(fildes, F_ULOCK, 1L);
		finis

		testing("Check that there are two locks remaining.");
		start
			overlap = YES;
			check_lockf(0, LIMIT/2, overlap);
			check_lockf(LIMIT/2, LIMIT/2, overlap);
		finis

		cleanup("Close the data file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis

	finis


	abstract("Test the lockf function F_TEST:");
	abstract("Lock a whole file. Fork off a child and have it issue an");
	abstract("F_TEST and hope it returns -1 and an errno of EACCES.");
	start

		prep("Open the data file.");
		start
			expecting ( SUCCESS );
			fildes = data_open("lockf_data210", O_RDWR, 0);
		finis

		testing("Seek to the beginning of the file and issue the lock.");
		start
			expecting ( 0L );
			lseek(fildes, 0L, SEEK_SET);

			expecting ( SUCCESS );
			lockf(fildes, F_LOCK, 0L);
		finis

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if (pid > 0) 	/* parent */
			(void) wait_for(pid, 2);

		else	/* child */
		{
			prep("Child opens data file.");
			start
				expecting ( SUCCESS );
				fildes = data_open("lockf_data210", O_RDWR, 0);
			finis

			testing("Child seeks to beginning and");
			testing("tries to lock the file.");
			start
				expecting ( 0L );
				lseek(fildes, 0L, SEEK_SET);

				expecting( FAILURE );
				lockf(fildes, F_TEST, 0L);

				if ( errno != EACCES )
					if ( errno == EAGAIN )
						warning(FUTURDIR);
					else
						error("Incorrect errno value from lockf on F_TEST.\nExpecting EACCES or EAGAIN, got %s.", err_lookup (errno));

			finis

			exit(0);
		}
		cleanup("Close the data file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis

	finis


	abstract("The lockf function F_TLOCK.");
	abstract("Lock a whole file. Fork off a child and have it issue an");
	abstract("F_TLOCK and hope it returns -1 and an errno of EACCES.");
	start
		prep("Open the data file.");
		start
			expecting ( SUCCESS );
			fildes = data_open("lockf_data210", O_RDWR, 0);
		finis

		testing("Seek to the start of the file and issue a lock.");
		start
			expecting ( 0L );
			lseek(fildes, 0L, SEEK_SET);

			expecting ( SUCCESS );
			lockf(fildes, F_LOCK, 0L);
		finis

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if (pid > 0) 	/* parent */
			(void) wait_for(pid, 2);

		else	/* child */
		{
			prep("The child opens the data file.");
			start
				expecting ( SUCCESS );
				fildes = data_open("lockf_data210", O_RDWR, 0);
			finis

			testing("The child seeks to the beginning and");
			testing("tries to issue the lock.");
			start
				expecting ( 0L );
				lseek(fildes, 0L, SEEK_SET);

				expecting( FAILURE );
				lockf(fildes, F_TLOCK, 0L);

				if ( errno != EACCES )
					if ( errno == EAGAIN )
						warning(FUTURDIR);
					else
						error("Incorrect errno value from lockf on F_TEST.\nExpecting EACCES or EAGAIN, got %s.", err_lookup (errno));

			finis
			exit(0);
		}

		cleanup("Close the data file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis

	finis


	testing("Lockf given an invalid file descriptor.");
	testing("This test is constructed using the F_LOCK cmd.");
	testing("A return value of -1 and errno of EBADF.");
	start
		expecting( EBADF );
		lockf(BAD_FILDES, F_LOCK, 0L);

	finis

	testing("Lockf given an invalid file descriptor.");
	testing("This test is constructed using the F_TLOCK cmd.");
	testing("A return value of -1 and errno of EBADF.");
	start
		expecting( EBADF );
		lockf(BAD_FILDES, F_TLOCK, 0L);
	finis


	testing("Lockf given an invalid file descriptor.");
	testing("This test is constructed using the F_TEST cmd.");
	testing("A return value of -1 and errno of EBADF.");
	start
		expecting( -1 );
		lockf(BAD_FILDES, F_TEST, 0L);

		if ( errno != EBADF )
			if ( errno != EINVAL )
						error("Incorrect errno value from lockf on F_TEST.\nExpecting EBADF or EINVAL, got %s.", err_lookup (errno));
			else
				warning("%s\nlockf on F_TEST set errno to EINVAL", ADVISORY);
	finis


	abstract("Determine whether deadlock avoidance is operating.");
	abstract("Parent and child each lock a record.  Child");
	abstract("attempts to lock parents record and blocks.  Parent ");
	abstract("attempts to lock childs record.  Parent should sense");
	abstract("deadlock condition.");
	start


/*
** The heuristics for this deadlock test are as follows:
**
**	Parent				Child
**	  - set alarm time bomb
**	  - lock segment A
**	  - fork child			  - lock segment B
**	  - sleep(15)			  - try locking A (block|lock)
**	  - try locking B (block|lock)	   
**	should return EDEADLK
**	parent should exit		 child should lock A
**
**		time bomb: encountered deadly embrace
**
**	If child doesn't get to lock(A) before parent gets to lock(B), 
**	the time for sleep is not long enough.  This really should have 
**	been done with semaphores, but there is no way to v_sema after
**	the process is blocked, and a v_sema before the process is blocked
**	causes a race.  The call to sleep is a one-shot.
**
*/

		prep("Reset SIGALRM and set the alarm to go off.");
		start
			expecting ( SUCCESS );
			abort_on(ERROR);

			signal(SIGALRM, catch_sig);

			expecting ( VOID );
			(void) alarm(60);
		finis

		prep("Open the data file.");
		prep("Seek to the beginning and set a lock.");
		start
			expecting ( SUCCESS );
			fildes = data_open("lockf_data210", O_RDWR, 0);

			expecting ( 0L );
			lseek(fildes, 0L, SEEK_SET);


			expecting ( SUCCESS );	/* lock segment A */
			lockf(fildes, F_LOCK, 1L);
		finis

		prep("Fork a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis


		if (pid > 0) 	/* parent */
		{
			prep("Parent waits 15 seconds before checking for deadlock.");
			start
				expecting ( VOID );
				rel = time((time_t *)0) + 15;
				while(time((time_t *)0) < rel);
			finis

			testing("Parent now seeks and tries to lock segment B.");
			testing("The parent hopes to get deadlock.");
			start
				expecting( 1L );
				lseek(fildes, 1L, SEEK_SET);

				expecting( EDEADLK );
				lockf(fildes, F_LOCK, 1L);   /* try locking seg. B */
			finis
			cleanup("Unlock segment A and wait for child.");
			start
				expecting ( 0L );
				lseek(fildes, 0L, SEEK_SET);
				expecting ( SUCCESS );	/* lock segment A */
				lockf(fildes, F_ULOCK, 1L);
				wait((int *)0);
			finis
		}
		else	/* child */
		{

			prep("Child opens data file.");
			start
				expecting ( SUCCESS );
				fildes = data_open("lockf_data210", O_RDWR, 0);
			finis

			testing("Child now seeks to segment B and locks it.");
			start
				expecting( 1L );
				lseek(fildes, 1L, SEEK_SET);

				expecting ( SUCCESS );	/* lock segment B */
				lockf(fildes, F_LOCK, 1L);
			finis

			testing("The child now seeks to segment A and tries to lock it.");
			start
				expecting ( 0L );
				lseek(fildes, 0L, SEEK_SET);

				expecting ( SUCCESS );
				lockf(fildes, F_LOCK, 1L);
			finis
			exit(0);
		}

		cleanup("Close the data file.");
		start
			expecting ( SUCCESS );
			close(fildes);
		finis
	finis
	done();
	/*NOTREACHED*/
}

void check_lockf(i, size, overlap)  int	i, size, overlap;

{
	struct flock frec;
	int	pid;
	int	fildes;

	expecting ( SUCCESS );
	pid = fork();

	if (pid > 0) 	/* parent */
		(void) wait_for(pid, 2);

	else	/* child */
	{
		expecting ( SUCCESS );
		fildes = data_open("lockf_data210", O_RDWR, 0);

		frec.l_type = F_WRLCK;
		frec.l_start = i;
		frec.l_whence = 0;
		frec.l_len = size;

		expecting( SUCCESS );
		fcntl(fildes, F_GETLK, &frec);

		if ( frec.l_type != F_WRLCK )
		{
			severr("fcntl indicates that lockf failed to set a write lock.");
		}

		if ( overlap == NO )
		{
			if ( frec.l_start != i )
			{
				error("Fcntl reports that l_start is different than expected.\nExpected l_start of %d.\nFound l_start of %d.", i, (int) frec.l_start);	/* ERROR */
			}

			if ( frec.l_whence != 0 )
			{
				error("Fcntl reports that l_whence is different than expected.\nExpected l_whence of %d.\nFound l_whence of %d.", i, (int) frec.l_whence);	/* ERROR */
			}

			if ( frec.l_len != size )
			{
				error("Fcntl reports that l_len is different than expected.\nExpected l_len of %d.\nFound l_len of %d.", i, (int) frec.l_len);	/* ERROR */
			}
		}
		exit(0);
	}
}

siggy	catch_sig(sig)
int sig;
{
	if (sig != SIGALRM)
	{
		severr("lockf: unexpected signal");
	}

	{
		severr("lockf: deadlock occurred and was undetected");
	}
}
#endif
