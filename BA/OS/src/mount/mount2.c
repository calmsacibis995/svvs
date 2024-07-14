#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)mount2.c	1.4"



# define	READ_WRITE	0

/*
** mount
**	Test the mount function.
**
**
**	This test should be run manually.
**	This test tries to determine whether mount fails due to
**	a lack of mount table entries.
**	This is an extrememly difficult test to construct.
**	The simplest alternative is to consume all the mount table
**	entries and then execute this test.
*/

main()
{
	char	PathPointer[PATH_MAX];
	int	mount();

	setup("mount");
	funct("mount");

	exit_on ( ERROR );
	testing("Mount will fail when there are no more mount table entries.");
	testing("This is probably best done manually before this test is run.");
	testing("Expect a  return value of -1 and errno of EBUSY.");
	start
		exit_on (ERROR | SEVERR);

		expecting ( VOID );
		(void) umount(SVVS_FS); /* insurance */

		strcpy(PathPointer,data_path());
		PathPointer[strlen(PathPointer)-1] = 0;

		expecting (-1);
		mount(SVVS_FS, PathPointer, READ_WRITE);
		if (errno != EBUSY)
			{
			if (errno == EAGAIN)
				warning("%s\nerrno was set to EAGAIN",
					FUTURDIR);
			else
				error ("errno was set to %s.",
					err_lookup (errno));
			}

	finis

	done();
	/*NOTREACHED*/

}
