#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)open3.c	1.2"

/*
** open
**	Test the open function.
**
**	This test is optional. This is for machines which
**	support pure text segments.
*/

main()
{
	int		ret_val;
	char		path[PATH_MAX];

	int		unlink();

	setup("open");
	funct("open");

	exit_on ( ERROR );

	abstract("Open for O_WRONLY a pure text segment that is being executed.");
	abstract("Expect a  return value of (-1) and errno of ETXTBSY.");
	start

		if ( PURETEXT ){
			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/open3");

			testing("Issue the call to open.");
			start
				expecting ( ETXTBSY );
				open(path, O_WRONLY);
			finis

		finis

		testing("Open for O_RDWR a pure text segment that is being executed.");
		testing("Expect a  return value of (-1) and errno of ETXTBSY.");
		start

			expecting ( ETXTBSY );
			open(path, O_RDWR);

		}
else {
	message("Test case suppressed - pure text segments are not supported on this system");
}
	finis

	done();
	/*NOTREACHED*/
}
