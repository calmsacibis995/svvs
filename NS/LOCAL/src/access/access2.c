#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)access2.c	1.6"

/*
 * access
 *	Test the access function.
 *
 *	This test is optional. This is for machines which
 *	support pure text segments.
 */

# define	W_ACC	02

main()
{
	char	path[PATH_MAX];

	setup("access");

	funct("access");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	abstract("Access a pure text segment that is being executed.");
	start

		if(PURETEXT) {
			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/access2");

			expecting( ETXTBSY );
			access(path, W_ACC);
		}
		else {
			unsupported ("Test case suppressed - pure text segments are not supported on this system");
		}
	finis

	done();

/*
 *	NOTREACHED
 */
}
