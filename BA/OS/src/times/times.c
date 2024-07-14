#include	<sv_base.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/times.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)times.c	1.6"

/*
** times
**	Test the times function.
**
**	The times function is machine dependent.
**	Since the times value is highly dependent upon 
**	processor speeds, this test verifies that times 
**	works via relative time stamp measurements.
**
**	The heuristic for testing times is as follows.
**	(1) Take a time stamp.
**	(2) Do something to consume time (real, sys, and user).
**	(3) Take another time stamp.
**	(4) Hopefully the time stamps will differ.
**
*/

main()
{
	ttime	t;

	long	elapsed, start_point;
	struct	tms init_buffer, buffer;
	int	pid;
	ttime	time();
	long	times();
	void	exit();

	setup("times");

	funct("times");

	exit_on ( ERROR );

	testing("A legal invocation of times.");
	testing("Expecting a return value of >0.");
	testing("NOTE: This timestamp will be saved as a reference");
	start

		expecting ( SUCCESS );
		start_point = times(&init_buffer);

		check_times(&init_buffer);

	finis

	testing("Times after some processor usage returns a legal value.");
	testing("Expect that this loop will finish far enough in the future that");
	testing("a modest amount of real, system, and user time has passed.");
	start

		expecting ( VOID );
		t = time((ttime *)0) + 3l;

		expecting ( VOID );
		while( t > time((ttime *)0) )
		{
			expecting ( SUCCESS );
			elapsed = times(&buffer);

			check_times(&buffer);
		}
	finis

	testing("Comparing the last time stamp values against the first time stamp.");
	testing("Expect that values from current time stamp are greater than the first saved time stamp.");
	start

		if (buffer.tms_utime < init_buffer.tms_utime)
	{
error("No change in user time.\nThe initial time was %l.\nThe actual time is %l.\n", init_buffer.tms_utime, buffer.tms_utime);	/* ERROR */
	}
		if (buffer.tms_stime < init_buffer.tms_stime)
	{
error("No change in system time.\nThe initial time was %l.\nThe actual time is %l.\n", init_buffer.tms_stime, buffer.tms_stime);	/* ERROR */
	}

		if ( start_point >= elapsed )
	{
error("No change in elapsed real time?.\nThe initial time was %l.\nThe actual time is %l.\n", elapsed, start_point);	/* ERROR */
	}

	finis

	testing("Child process times are recorded by parent process.");
	testing("The child times to be initially zero then positive after");
	testing("the child has done some processing and died.");
	start

		if ( buffer.tms_cutime != 0 )
	{
error("Pre child user time not correct.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer.tms_cutime);	/* ERROR */
	}
		if ( buffer.tms_cstime != 0 )
	{
error("Pre child user time not correct.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer.tms_cutime);	/* ERROR */
	}

		expecting ( SUCCESS );
		elapsed = times(&buffer);

		expecting ( SUCCESS );
		pid = fork();

		if (pid > 0) 	/* parent */
			(void) wait_for(pid, 2);

		else	/* child */
		{

			expecting ( VOID );
			t = time((ttime *)0) + 3l;

			expecting ( VOID );
			while( t > time((ttime *)0) )
			{
				expecting ( SUCCESS );
				elapsed = times(&buffer);

				check_times(&buffer);
			}
			exit(0);
		}


		expecting ( SUCCESS );
		elapsed = times(&buffer);

		check_times(&buffer);

		if ( buffer.tms_cutime == 0 )
	{
error("Post child user time not correct.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer.tms_cutime);	/* ERROR */
	}
		if ( buffer.tms_cstime == 0 )
	{
error("Post child user time not correct.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer.tms_cutime);	/* ERROR */
	}

	finis

	done();
	/*NOTREACHED*/
}

check_times(buffer)
struct tms *buffer;
{
	if ( buffer->tms_utime < 0)
	{
severr("tms_utime is a negative value.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer->tms_utime);	/* HALT */
	}

	if ( buffer->tms_stime < 0)
	{
severr("tms_stime is a negative value.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer->tms_stime);	/* HALT */
	}

	if ( buffer->tms_cutime < 0)
	{
severr("tms_cutime is a negative value.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer->tms_cutime);	/* HALT */
	}

	if ( buffer->tms_cstime < 0)
	{
severr("tms_cstime is a negative value.\nThe initial time was %l.\nThe actual time is %l.\n", 0, buffer->tms_cstime);	/* HALT */
	}
}
