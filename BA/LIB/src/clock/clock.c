#include	<sv_base.h> /*replacement macros */


#ifndef	SYSVR_2
#include	<sys/types.h>
#include	<time.h>
#endif

#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	WAITTIME	5
# define	MAXNUM		10000


#ident	"@(#)clock.c	1.4"



/*
** clock
**	Test the clock function.
*/
main()
{
	ttime	ret_val;
	ttime	prev;
	int	process_id;
	int	wait_val;
	ttime	t;
	
	extern	ttime	clock();
	extern	ttime	time();

setup("clock");
funct("clock");
abstract ("The clock subroutine is tested by looping for 5 minutes");
abstract ("(as judged by time()) and breaking out of the loop");
abstract ("when the clock ticks");

start

	testing("Execute a loop, call clock at the end of the loop.");
	testing("Verify the return value from clock is greater than 0.");
	start
		ret_val = 0L;
		t = time((ttime *)0) + 5l;
		while( t > time((ttime *)0) && !ret_val)
			if ((ret_val = clock()) < 0l )
				error ("Negative return value from clock");
		if(!(prev = ret_val))
			error ("Return value from clock did not increase during 5 secs. of calls");
	finis


	testing("Execute the loop again, call clock at the end of the loop.");
	testing("Verfiy the cpu time increases.");
	start
		t = time((ttime *)0) + 5l;
		while( t > time((ttime *)0) && ret_val == prev)
			if ((ret_val = clock()) < prev )
				error ("Return value from clock less than previous return value");

		if(!(prev = ret_val))
			error ("Return value from clock did not increase during 5 secs. of calls");
	finis


	testing("Run a child process and execute the loop again, call");
	testing("clock at the end of the loop. ");
	testing("Verify an increase in indicated CPU usage.");
	start

		process_id = fork();
		if ( process_id == -1 )
			error ("Could not fork from clock()");

		if ( process_id != 0 )
			wait_val = wait_for(process_id, WAITTIME);
		else
		{
			/*
			** CHILD process
			*/
			t = time((ttime *)0) + 5l;
			while( t > time((ttime *)0) && !ret_val)
				clock();
			exit(0);
			/*NOTREACHED*/
		}
		if ( wait_val )
			error ("child process exited abnormally");

		ret_val = clock();
		if ( ret_val < prev )
			error ("CPU time did not increase.");
	finis

finis

done();
/*NOTREACHED*/
}
