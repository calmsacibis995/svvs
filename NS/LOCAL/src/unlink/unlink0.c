#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)unlink0.c	1.2"

# define	BLOCK_SPEC	060777
# define	PROT		0777
# define	READ_ONLY	1

/*
** unlink
**	Test the unlink function.
**
**
*/

main()
{
	char	path[PATH_MAX];
	char	PathPointer[PATH_MAX];

	setup("unlink");

	funct("unlink");

	exit_on ( ERROR );

	abstract("A legal invocation of unlink.");
	abstract("Expect a return value of zero.");
	start

		prep("Create a temporary file.");
		start
			temp_creat("unlink_1", PROT);
		finis

		strcpy(path, temp_path());
		strcat(path, "unlink_1");

		testing("Issue the call to unlink.");
		start
			expecting ( SUCCESS );
			unlink(path);
		finis

	finis

	testing("Unlinking a file which does not exist.");
	testing("Expect a return value of -1 and errno of ENOENT.");
	start

		strcpy(path, temp_path());
		strcat(path, "_unlink");

		expecting( ENOENT );
		unlink(path);

	finis

	testing("Unlinking a file -- search permission along a path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm.dir");
		strcat(path, "/unlink_tryme");

		expecting( EACCES );
		unlink(path);

	finis

	testing("Unlinking a file -- write permission along a path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm1.dir");
		strcat(path, "/unlink_tryme");

		expecting( EACCES );
		unlink(path);

	finis

	abstract("Unlink -- argument has a component that is not a directory.");
	abstract("Expect a return value of -1 and errno of ENOTDIR.");
	start

		prep("Create a temporary file which will be part of the component path.");
		start
			temp_creat("unlinkbad_dir", PROT);

			strcpy(path, temp_path());
			strcat(path, "unlink");
			strcat(path, "bad_dir");
			strcat(path, "/dummy_point");
		finis

		testing("Issue the call to unlink.");
		start
			expecting( ENOTDIR );
			unlink(path);
		finis

		cleanup("Unlink the temporary file.");
		start
			strcpy(path, temp_path());
			strcat(path, "unlink");
			strcat(path, "bad_dir");

			expecting ( SUCCESS );
			unlink(path);
		finis

	finis

	testing("To fail remove a directory while not superuser.");
	testing("Expect a return value of -1 and errno of EPERM.");
	start

		strcpy(PathPointer,temp_path());
		PathPointer[strlen(PathPointer)-1] = 0;

		expecting( EPERM);
		unlink(PathPointer);

	finis

	done();
	/*NOTREACHED*/
}
