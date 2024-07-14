#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mknod1.c	1.5"

# define	PIPE_PROT	0010777
# define	NODEV	0

/*
** mknod
**	Test the mknod function.
**
*/

main()
{
	char	path[PATH_MAX];

	int	mode;
	int	mknod();
	int	fstat();


	setup("mknod");
	funct("mknod");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Test all possible values of mode for mknod while executing as svvs.");
	abstract("Expect a return value of -1 with errno = EPERM.");
	start

		/*
**	Set umask explicitly to zero since driver may not do so....
**	Call unlink on the fly in order not to leave around lots and lots
**	of empty files....
*/

		prep("Reset umask.");
		start
			expecting ( VOID );
			umask(0);

			expecting ( 0 );
			umask(0);
		finis

		testing("Issue the calls to mknod.");
		start
			for ( mode = 00; mode <= 07777; mode++){

				sprintf(path, "%s%d", temp_path(), mode);

				expecting( EPERM );
				mknod(path, mode, NODEV);

			}
		finis

	finis

	testing("Mknod a file that already exists.");
	testing("Expect a return value of -1 and errno of EEXIST.");
	start

		strcpy(path, temp_path());
		strcat(path, "mknod1_eexist");

		expecting ( SUCCESS );
		mknod(path, PIPE_PROT, NODEV);

		expecting( EEXIST );
		mknod(path, PIPE_PROT, NODEV);

	finis

	testing("Mknod for a file along a path which does not exist.");
	testing("Expect a return value of -1 and errno of ENOENT.");
	start

		strcpy(path, temp_path());
		strcat(path, "_mknod/_mknod");

		expecting( ENOENT );
		mknod(path, PIPE_PROT, NODEV);

	finis

	testing("Mknod -- argument has a component that is not a directory.");
	testing("Expect a return value of -1 and errno of ENOTDIR");
	start

		temp_creat("mknodbad_dir", PIPE_PROT);

		strcpy(path, temp_path());
		strcat(path, "mknod");
		strcat(path, "bad_dir");
		strcat(path, "/dummy_point");

		expecting( ENOTDIR );
		mknod(path, PIPE_PROT, NODEV);

	finis

	testing("Mknod for a file -- search permission along a path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm.dir");
		strcat(path, "/mknod_tryme");

		expecting( EACCES );
		mknod(path, PIPE_PROT, NODEV);

	finis

	done();
	/*NOTREACHED*/
}
