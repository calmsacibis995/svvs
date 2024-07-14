#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)signal3.c	1.7"

/*
** signal3
**	Test the signal function - error cases, child catches with func.
**
*/

main()
{
	siggy	(*sts)();

#ifndef	SYSVR_2
	void (*signal())();
#else
	int (*signal())();
#endif

	setup("signal");

	funct("signal");

	exit_on ( SEVERR );

	testing("Signal with SIGKILL and SIG_IGN.");
	testing("Expect a return value of SIGERR and errno of EINVAL.");
	start

		expecting ( EINVAL );
		sts = (siggy (*)())signal(SIGKILL, SIG_IGN);
		if ( sts != (siggy (*)())(-1) )
	{
severr("Unexpected return value from signal.\nExpected value was %ld.\nThe actual value was %ld.\n", (long)SIG_DFL, (long)sts);	/* HALT */
	}

	finis

	testing("Signal with invalid signal and SIG_IGN.");
	testing("Expect a return value of SIGERR and errno of EINVAL.");
	start

		expecting ( EINVAL );
		sts = (siggy (*)())signal((-1), SIG_IGN);
		if ( sts != (siggy (*)())(-1) )
	{
severr("Unexpected return value from signal.\nExpected value was %ld.\nThe actual value was %ld.\n", (long)SIG_DFL, (long)sts);	/* HALT */
	}

	finis

	done();
	/*NOTREACHED*/
}
