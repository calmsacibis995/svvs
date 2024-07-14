
#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)creat2.c	1.4"

# define	PROT	0777

/*
** creat
**	Test the creat function.
**
**	This test is optional. This is for machines which
**	support pure text segments.
*/


main()
{
	char	path[PATH_MAX];

	int	creat();

	setup("creat");

	funct("creat");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	testing("Creat a pure text segment that is being executed.");
	start


		if (PURETEXT) {
			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/creat2");

			expecting ( ETXTBSY );
			creat(path, PROT);
		}
		else {
			message("Test case suppressed - pure text segments are not supported on this system");
		}
	finis

	done();
/*
 *	NOTREACHED
*/
}
