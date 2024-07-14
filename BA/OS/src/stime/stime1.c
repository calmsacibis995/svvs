#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)stime1.c	1.4"

/*
** stime
**	Test the stime function.
**
*/

main()
{
	long	epoch_start = 0;

	setup("stime");

	funct("stime");

	testing("Stime invoked by a normal user fails.");
	testing("Expect a return value of -1, and an errno of EPERM.");
	testing("If this test fails it may leave the system clock in a bad state.");

	exit_on ( ERROR );
	start

		expecting( EPERM );
		stime(&epoch_start);
	finis

	done();
	/*NOTREACHED*/
}
