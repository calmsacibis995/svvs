#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)geteuid.c	1.2"

/*
** geteuid
**	Test the geteuid function.
**
**	This test should be duplicated and run as setuid root.
**
**
*/

main()
{
	unsigned short	geteuid();

	setup("getuid");
	funct("geteuid");

	exit_on ( ERROR );

	testing("Geteuid returns a valid user ID.");
	testing("Expect a  return value = EUID, and errno = NOERR.");
	start

		expecting( EUID );
		(unsigned short) geteuid();


	finis
	done();
}
