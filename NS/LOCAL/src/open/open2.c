#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)open2.c	1.3"

# define	BLOCK_SPEC	060777

/*
** open
**	Test the open function.
**
**
** This tests is dependent upon:
**	mknod()
**	<stdio.h>
**
*/

main()
{

	char		path[PATH_MAX];

	int		mknod();
	char		*temp_path();

	setup("open");
	funct("open");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Open a device special which does not exist.");
	abstract("Expect a  return value of (-1) and errno of ENXIO");
	start

		prep("Call mknod to create a special file..");
		start
			strcpy(path, temp_path());
			strcat(path, "open_spec");

			expecting ( SUCCESS );
			mknod(path, BLOCK_SPEC, OFF_LINE_DEV);
		finis

		testing("Issue the call to open.");
		start
			expecting ( ENXIO );
			open(path, O_RDONLY);
		finis

	finis

	done();
	/*NOTREACHED*/
}
