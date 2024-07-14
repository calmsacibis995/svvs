#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)time.c	1.5"

/*
** time
**	Test the time function.
*/

main()
{
	ttime	ret_val;

	ttime	local_time;
	ttime	time();

	void	exit();


	setup("time");

	funct("time");

	exit_on ( ERROR );

	testing("A legal invocation of time.");
	testing("Expect a return value of >0.");
	start

		expecting ( SUCCESS );
		time((ttime *) 0);

	finis

	testing("Time with a legal tloc argument.");
	testing("Expect a return value >0.");
	start

		expecting ( SUCCESS );
		ret_val = time(&local_time);

		if ( local_time != ret_val)
	{
error("Value returned by time does not equal that left in tloc.\nThe return value was %l.\nThe value for tloc was %l.\n", ret_val, local_time);	/* ERROR */
	}

	finis

	done();
	/*NOTREACHED*/
}
