#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getegid.c	1.2"

/*
** getegid
**	Test the getegid function.
**
*/
main()
{
	unsigned short	getegid();

	setup("getuid");
	funct("getegid");

	exit_on ( ERROR );

	testing("Getegid returns a valid group ID.");
	testing("Expect a  return value = EGID, and errno = NOERR.");
	start

		expecting( EGID );
		(unsigned short) getegid();

	finis
	done();
}
