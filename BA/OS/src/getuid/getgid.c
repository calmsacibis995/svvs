#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getgid.c	1.2"

/*
** getgid
**	Test the getgid function.
**
**
**	This test should be duplicated and run as setgid root.
*/

main()
{
	unsigned short	getgid();

	setup("getuid");
	funct("getgid");

	exit_on ( ERROR );

	testing("Getgid returns a valid group ID.");
	testing("Expect a return value = GID, and errno = NOERR.");
	start

		expecting( GID );
		getgid();

	finis
done();
}
