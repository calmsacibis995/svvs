#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)stime0.c	1.7"

# define	BOUND_SEC	60

/*
** stime
**	Test the stime function.
**
**	The strategy for the primary test is as follows.
**	(1) Record the current time.
**	(2) Set the time to epoch.
**	(3) Get the time.
**	(4) Make sure that time is close to epoch...within BOUND_SEC.
**	(5) Reset the time.
**	(6) Check that the time is close to when you started.
**
**
** Warning:
**	 Processes like cron should not be running when this test is executed.
**	This test may slightly alter your time of day clock.
**
*/
main()
{
	ttime	ret_val;

	ttime	current_time;
	ttime	epoch_start = 0;
	ttime	time();


	setup("stime");

	funct("stime");

	exit_on ( ERROR | SEVERR );

	abstract("A legal invocation of stime.");
	abstract("Expect a return value of 0.");
	abstract("If this test fails it may leave the system clock in a bad state.");
	start

		testing("Get current time.");
		start
			expecting ( SUCCESS );
			current_time = time((ttime *) 0);
		finis

		testing("Call stime to epoch.");
		start
			expecting ( SUCCESS );
			stime(&epoch_start);
		finis

		testing("Does time confirm that it's now epoch.");
		start
			expecting ( SUCCESS );
			ret_val = time((ttime *) 0);

			if (ret_val > BOUND_SEC)
			{
				error("Time is not close enough to epoch...stime/time failure.\nExpected to be within %ld seconds of epoch.\nWas actual value was %ld seconds from epoch\n", (long) BOUND_SEC, ret_val);	/* ERROR */
			}

		finis

		testing("Call stime to reset the clock.");
		start
			expecting ( SUCCESS );
			stime(&current_time);
		finis

		testing("Call time to make sure that clock has been reset.");
		start
			expecting ( SUCCESS );
			ret_val = time((ttime *) 0);

			if ((ret_val - current_time) > BOUND_SEC)
			{
				severr("Failed reset to original time!!\nThe current time is %ld.\nThe expected time was was %ld:\n", current_time, ret_val);	/* HALT */
			}
		finis

	finis

	done();
	/*NOTREACHED*/
}
