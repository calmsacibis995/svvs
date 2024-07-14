#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getuid0.c	1.3"

/*
** getuid
**	Test the getuid function.
**
**	This test should be duplicated and run as setuid root.
*/

main()
{
	unsigned short	getuid();

	setup("getuid");
	funct("getuid");

	exit_on ( ERROR );

	testing("Getuid returns a valid user ID.");
	testing("Expect a  return value == UID, and errno = NOERR.");
	start

		expecting( UID );
		(unsigned short) getuid();

	finis
	done();
/*NOTREACHED*/
}
