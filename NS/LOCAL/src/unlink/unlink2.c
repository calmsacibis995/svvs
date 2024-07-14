#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)unlink2.c	1.2"

/*
** unlink
**	Test the unlink function.
**
**	This test is optional. This is for machines which
**	support pure text segments.
*/

main()
{
	char	path[PATH_MAX];

	int	unlink();

	setup("unlink");

	funct("unlink");

	exit_on ( ERROR );

	abstract("Unlink a pure text segment that is being executed.");
	abstract("Expect a return value of -1 and errno of ETXTBSY.");
	start

		if ( PURETEXT ) {
			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/unlink2");

			testing("Issue the call to unlink.");
			start
				expecting( ETXTBSY );
				unlink(path);
			finis
		}
		else
		{
			message("Test case suppressed - pure text segments are not supported.");
		}

	finis
	done();
	/*NOTREACHED*/
}
