#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mount1.c	1.2"

# define	READ_WRITE	0

/*
** mount
**	Test the mount function.
**
**
**	The binary must be non setuid root!
*/

main()
{
	int	mount();

	setup("mount");
	funct("mount");

	exit_on ( ERROR );

	testing("An illegal invocation of mount by a non-super-user.");
	testing("Expect a  return value of -1 and errno of EPERM.");
	start

		expecting ( VOID );
		(void) umount(SVVS_FS); /* insurance */
		errno = 0;

		expecting( EPERM );
		mount(SVVS_FS, MOUNT_POINT, READ_WRITE);

	finis

	done();
	/*NOTREACHED*/

}
