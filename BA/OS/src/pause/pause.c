#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)pause.c	1.8"

# define	CHILD_WAIT	5

# ifndef	SIG_ERR
# define	SIG_ERR	(int(*)())-1
# endif

/*
** pause
**	Test the pause function.
**
**	This test catches all SIGUSR1 signals.
**	This test may interfere with the driver.
**
*/

main()
{
	int	pause();
	ttime	time();
	int	pid, first_time, current_time;
	siggy	(*func)();
	int	catch_sig();


	setup("pause");

	funct("pause");

	exit_on ( ERROR );

	abstract("A typical usage of pause.");
	abstract("Expect a return value of -1 and errno of EINTR.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid > 0 )	/* parent */
		{
			prep("Parent waits 5 seconds before sending signal.");
			start
				expecting ( SUCCESS );
				first_time = time((ttime *)0);

				do
					current_time = time((ttime *)0);
				while (( current_time - first_time ) < CHILD_WAIT );
			finis

			testing("Parent now kills the child who should be in a pause.");
			start
				expecting ( SUCCESS );
				kill(pid, SIGUSR1);
			finis

			testing("Wait for the child to die.");
			start
				(void) wait_for(pid, 2);
			finis
		}
		else	/* child */
		{

			prep("Child prepares to accept parents signal.");
			start
				expecting ( VOID );
				func = (siggy (*)())signal(SIGUSR1, catch_sig);

				if ( func == (siggy (*)())SIG_ERR )
				{
					severr("Signal could not set SIGUSR1.");	/* HALT */
				}
			finis

			testing("Child now pauses.");
			start
				expecting( EINTR );
				pause();

			finis
		}

	finis

	done();
	/*NOTREACHED*/
}


int catch_sig() {
	return(-1);
}
