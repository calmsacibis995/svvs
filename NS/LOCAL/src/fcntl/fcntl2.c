#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	COE_FLAG_ON	0x1
# define	WAIT		5

char	*err_lookup ();

#ident	"@(#)fcntl2.c	1.7"

/*
** fcntl()
**	 This is the test program for the OS routine fcntl.
*/
main()
{

#ifdef LOCKF

	int		fildes1;
	int		pid;
	int		wait_val;
	int		i;
	int		retc;
	struct	flock	test_flock;
	struct	flock	*flock_ptr = &test_flock;

	setup("fcntl");


	abstract("Set a write lock on a file.");
	abstract("When the child process attempts to create the same lock,");
	abstract("the attempt should fail, proving that the parent retained");
	abstract("the lock and that the child did not inherit the lock.");
	abstract("The call to fcntl with F_GETLK should describe the");
	abstract("parent's lock which prevents it from locking.");

	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_RDWR|O_TRUNC, 0777);
		finis

		flock_ptr->l_type = F_WRLCK;
		flock_ptr->l_whence = 0;
		flock_ptr->l_start = 0L;
		flock_ptr->l_len = 10L;

		testing("Issue the call to fcntl to SETLCK of WRLCK.");
		start
			expecting ( SUCCESS );
			fcntl(fildes1, F_SETLK, flock_ptr);
		finis

		prep("Fork off a child.");
		start
			pid = fork();

		if ( pid == 0 )
		{
			/* CHILD process */
			testing("the child's attempt should fail with EACCES");
			start
				expecting ( FAILURE );
				fcntl(fildes1, F_SETLK, flock_ptr);
				/*
				 *  check the errno
				 */
				if (errno != EACCES)
					if (errno != EAGAIN)
						error (
"SETLK: errno = %s, EAGAIN or EACCES was expected", err_lookup (errno));
					else
						warning (
"%s\nerrno of EAGAIN was returned", FUTURDIR);
			finis

			testing ("fcntl (F_GETLK) should describe");
			testing ("the parent's lock");
			start
				expecting (SUCCESS);
				fcntl (fildes1, F_GETLK, flock_ptr);

				if (flock_ptr -> l_pid != getppid ())
					error (
"GETLK returned a pid of %d, %d was expected", flock_ptr -> l_pid, getppid ());
			finis
			exit(0);
		}
		else
		{
			testing("Parent waits for the child.");
			start
				expecting ( VOID );
				if ( (wait_val = wait_for(pid, WAIT)) )
				{
					severr("The child terminated abnormally.");
				}
			finis

			cleanup("Close the temp file.");
			start
				expecting ( SUCCESS );
				close(fildes1);
			finis

		}
		finis
	finis



	abstract("Attempting write locks on a file opened for read only");
	abstract("should fail with EBADF");

	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_RDONLY|O_TRUNC, 0777);
		finis

		flock_ptr->l_type = F_WRLCK;
		flock_ptr->l_whence = 0;
		flock_ptr->l_start = 0L;
		flock_ptr->l_len = 10L;

		testing("fcntl (fd, F_SETLK, flock_ptr) should fail with EBADF");
		start
			expecting ( EBADF );
			fcntl(fildes1, F_SETLK, flock_ptr);
		finis

		testing("fcntl (fd, F_SETLKW, flock_ptr) should fail with EBADF");
		start
			expecting ( EBADF );
			fcntl(fildes1, F_SETLKW, flock_ptr);
		finis

		cleanup("Close the temp file.");
		start
			expecting ( SUCCESS );
			close(fildes1);
		finis
	finis


	abstract("Attempting read locks on a file opened for write only");
	abstract("should fail with EBADF");

	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_WRONLY|O_TRUNC, 0777);
		finis

		flock_ptr->l_type = F_RDLCK;
		flock_ptr->l_whence = 0;
		flock_ptr->l_start = 0L;
		flock_ptr->l_len = 10L;

		testing("fcntl (fd, F_SETLK, flock_ptr) should fail with EBADF");
		start
			expecting ( EBADF );
			fcntl(fildes1, F_SETLK, flock_ptr);
		finis

		testing("fcntl (fd, F_SETLKW, flock_ptr) should fail with EBADF");
		start
			expecting ( EBADF );
			fcntl(fildes1, F_SETLKW, flock_ptr);
		finis

		cleanup("Close the temp file.");
		start
			expecting ( SUCCESS );
			close(fildes1);
		finis
	finis


	abstract("fcntl for a GETLK, SETLK, or SETLKW will fail with EINVAL");
	abstract("if the third argument points to an invalid structure.");
#ifndef	SYSVR_2
	abstract("fcntl will fail with ENOLCK when the lock table is full");
#endif

	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_WRONLY|O_TRUNC, 0777);
		finis


		testing("using a fcntl structure with whence > 2");
		testing("fcntl (fd, F_GETLK, bad_struct) should fail with EINVAL");
		start
			flock_ptr->l_type = F_RDLCK;
			flock_ptr->l_whence = 3;
			flock_ptr->l_start = 0L;
			flock_ptr->l_len = 10L;
			expecting ( EINVAL );
			fcntl(fildes1, F_GETLK, flock_ptr);
		finis

		testing("using a fcntl structure with len < 0");
		testing("fcntl (fd, F_GETLK, bad_struct) should fail with EINVAL");
		start
			flock_ptr->l_type = F_RDLCK;
			flock_ptr->l_whence = 0;
			flock_ptr->l_start = 0L;
			flock_ptr->l_len = -10L;
			expecting ( EINVAL );
			fcntl(fildes1, F_GETLK, flock_ptr);
		finis

		testing("using a fcntl structure with start < 0");
		testing("fcntl (fd, F_GETLK, bad_struct) should fail with EINVAL");
		start
			flock_ptr->l_type = F_RDLCK;
			flock_ptr->l_whence = 0;
			flock_ptr->l_start = -4L;
			flock_ptr->l_len = 10L;
			expecting ( EINVAL );
			fcntl(fildes1, F_GETLK, flock_ptr);
		finis

		testing("using a fcntl structure with an invalid type");
		testing("fcntl (fd, F_GETLK, bad_struct) should fail with EINVAL");
		start
			/*  this might be an illegal type	*/
			/*  but who are we to say?		*/
			flock_ptr->l_type = F_RDLCK + F_WRLCK + F_UNLCK;
			flock_ptr->l_whence = 0;
			flock_ptr->l_start = 0L;
			flock_ptr->l_len = 10L;
			expecting ( VOID );
			if ( (fcntl(fildes1, F_GETLK, flock_ptr) != (-1))
			  || (errno != EINVAL) )
				warning ("fcntl did not fail as expected");
		finis



#ifndef	SYSVR_2
/*
 *  This test was added for SVR3.
 *  Due to the grandfather clause, we cannot hold SVR2 systems to this test
 */
		testing ("exhaust the lock table");
		start
			flock_ptr->l_type = F_WRLCK;
			flock_ptr->l_whence = 0;
			flock_ptr->l_len = 1L;

			for (i=1 ; i>0 ; i++)
				{
				flock_ptr->l_start = (long) (2 * i);
				expecting (VOID);
				retc = fcntl (fildes1, F_SETLK, flock_ptr);
				if (retc == (-1))
					{
					if (errno != ENOLCK)
						error (
"fcntl failed with errno = %s, ENOLCK was expected", err_lookup (errno) );
					break;
					}
				}

		finis
#endif


		cleanup("Close the temp file.");
		start
			expecting ( SUCCESS );
			close(fildes1);
		finis


	finis
	done();
	/*NOTREACHED*/
#endif

}
