#include	<stdio.h>
#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getcwd.c	1.2"

/*
** getcwd
**	Test the getcwd function.
*/

main()
{

	char *getcwd(), *path;
	char buf[PATH_MAX+2];


	setup("getcwd");

	funct("getcwd");

	exit_on ( ERROR );

	testing("Normal invocation of getcwd, buf=array and size=PATH_MAX+2.");
	testing("Expecting a return of non-NULL.");
	start

		expecting ( NON_NULL );
		getcwd(buf, PATH_MAX+2);

	finis


	testing("Getcwd with buf=array and size=0.");
	testing("Expecting a return value of NULL, and an errno of EINVAL.");
	start

		expecting ( NULL );
		expecting ( EINVAL );
		getcwd(buf, 0);

	finis


	testing("Getcwd with buf=array and size=-1.");
	testing("Expecting a return value of NULL, and an errno of ERANGE.");
	start

		expecting ( NULL );
		expecting ( ERANGE );
		getcwd(buf, -1);

	finis


	testing("Getcwd with buf=array and size=1.");
	testing("Expecting a return value of NULL, and an errno of ERANGE.");
	start

		expecting ( NULL );
		expecting ( ERANGE );
		getcwd(buf, 1);

	finis

	done();
	/*NOTREACHED*/
}
