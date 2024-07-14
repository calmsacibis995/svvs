#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)sleep.c	1.11"

# ifndef	SIG_ERR
# define	SIG_ERR	(int(*)())-1
# endif

# define	TIME_PERIOD	(unsigned)10
# define	ALRM_LIMIT	(unsigned)3

# ifndef	SIG_ERR
# define	SIG_ERR	(int(*)())-1
# endif

/*
** sleep
**	Test the sleep function.
**
*/

main()
{
	unsigned	expected;
	unsigned	ret_val;

	unsigned	sleep();
	unsigned	alarm();
	ttime	time();
	ttime	bs, as;
	void	(*func)();
	int	catch_sig();


	setup("sleep");
	funct("sleep");

	testing("A legal invocation of sleep.");
	testing("Expect a  return value of 0.");

	start
		expecting ( SUCCESS );
		bs = (unsigned) time((ttime *)0);

		expecting ( 0 );
		sleep(TIME_PERIOD);


		expecting ( SUCCESS );
		as = (unsigned) time((ttime *)0);

		if ((as - bs) > ( TIME_PERIOD +2 ))
		{
			warning("Sleep slept for too long a period of time.\n\
Expected to sleep %ld.\nActually slept %ld", (long)TIME_PERIOD+2, (long)as-bs);	/* WARNING */
		}

	finis

	testing("Sleep with an argument of zero.");
	testing("Expect sleep to return immediately.");
	start

		expecting ( SUCCESS );
		bs = (unsigned) time((ttime *)0);

		expecting ( 0 );
		sleep((unsigned)0);

		expecting ( SUCCESS );
		as = time((ttime *)0);

		if ((as - bs) > ( (unsigned)1 ))
		{
			warning("Sleep slept for too long a period of time.\n\
Expected to sleep no more than 1.\n\
Actually slept %ld", (long)as-bs);	/* WARNING */
		}

	finis

	testing("Interrupting sleep by having an alarm go off.");
	testing("Expect sleep to return the number of seconds not slept.");
	start

		/*
**	Re-map SIGALRM so that default svvs environment does
**	not catch it.
**
*/

		expecting ( VOID );
		func = (void (*)())signal(SIGALRM, catch_sig);

		if ( func == (void (*)())SIG_ERR )
		{
			severr("Could not set sigalrm.");	/* HALT */
		}

		expecting ( 0 );
		alarm(ALRM_LIMIT);

		expecting ( VOID );
		expected = TIME_PERIOD - ALRM_LIMIT;
		ret_val = sleep(TIME_PERIOD);

		if ( ret_val != expected )
			error("Sleep when interrupted failed to return the time remaining on the clock.\nThe amount of time remaining on the clock should have been%d.\nThe actual time remaining was %d.",  (int) expected, (int) ret_val);
	finis


	done();
	/*NOTREACHED*/
}

int
catch_sig()
{
	return (-1);
}
