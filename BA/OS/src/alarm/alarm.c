#include	<sv_base.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)alarm.c	1.9"

# define	TIME	10
# define	SLACK	(TIME/5)	/* +/- 20% */
# define	WAIT	(TIME+SLACK+1)

# ifndef	SIG_ERR
# define	SIG_ERR	(int(*)())-1
# endif

/*
** alarm
**	Test the alarm function.
**
**	This test catches all SIGALRM signals.
**	This test may interfere with the driver.
**
**	This test should be run on a quiet  machine.
**	This test will have problems if your clock
**	isn't too accurate.
*/

int		blasted;
ttime	bs;
ttime	as;

main()
{
	ttime	ret_val;

	siggy	(*func)();

	unsigned	alarm();
	ttime	time();
	siggy	time_bomb();

	setup("alarm");
	funct("alarm");
	exit_on( SEVERR );

	abstract("Call signal to set signal catching for SIGALRM.");
	abstract("Call time to get the current time.  Call alarm( 0 ),");
	abstract("expecting a return value of 0.  Call time again to");
	abstract("verify that alarm did not sleep.  Resolution for this");
	abstract("check is %d seconds.  Verify that no SIGALRM was",SLACK);
	abstract("received.");
	start
		set_sig();
		get_time();
		testing("Call alarm( 0 ).");
		start			
			expecting ( 0 );
			alarm((unsigned) 0);
		finis
		chk_time( 0 );
		chk_sig( 0 );
	finis

	abstract("Call signal to set signal catching for SIGALRM.");
	abstract("Call time to get the current time.  Call alarm( %d ),",
	  TIME);
	abstract("expecting a return value of 0.  Call time again to");
	abstract("verify that alarm slept for approximately %d seconds.",
	  TIME );
	abstract("Resolution for this check is %d seconds.  Verify that",
	  SLACK);
	abstract("SIGALRM was received.");
	start
		set_sig();
		get_time();
		testing("Call alarm( %d ), and wait up to %d seconds for the alarm.",
		  TIME, WAIT);
		start
			expecting ( 0 );
			alarm( (unsigned) TIME);
			while( bs+WAIT > time( (ttime *)0 ) && !blasted );
			expecting( VOID );
			alarm((unsigned) 0 );
		finis
		chk_time( TIME );
		chk_sig( 1 );
	finis

	abstract("Call signal to set signal catching for SIGALRM.");
	abstract("Call time to get the current time.  Call alarm( %d ),",
	  TIME);
	abstract("expecting a return value of 0.  Call alarm( 0 ) to");
	abstract("reset the alarm clock, expecting the amount of time");
	abstract("remaining on the alarm to be returned.  Wait %d", WAIT );
	abstract("seconds to verify the alarm was reset, and verify that");
	abstract("no signal was sent.");
	start
		set_sig();
		get_time();
		testing("Call alarm( %d ), expecting a return value of 0.",
		  TIME);
		start
			expecting( 0 );
			alarm((unsigned) TIME);
		finis
		testing("Call alarm( 0 ), expecting a return value");
		testing("between %d and %d.  Wait for %d seconds to verify",
			TIME-SLACK, TIME, WAIT );
		testing("the alarm was reset.");
		start
			expecting( VOID );
			ret_val = (long) alarm((unsigned) 0);
			if ( ret_val > TIME || ret_val < TIME-SLACK )
				warning("Alarm returned a value outside of %d and %d: %d.",
				  TIME-SLACK, TIME, ret_val);
			while( bs+WAIT > time( (ttime *)0 ) && !blasted );
			expecting( VOID );
			alarm((unsigned) 0 );
		finis
		chk_sig( 0 );
	finis

	abstract("Call signal to set signal catching for SIGALRM.");
	abstract("Call time to get the current time.  Call alarm( %d ),",TIME);
	abstract("expecting a return value of 0.  Wait %d seconds.",TIME/2);
	abstract("Verify that no signal was sent by the first alarm.");
	abstract("Call alarm( %d ) to reset the alarm clock, expecting",TIME);
	abstract("the amount of time remaining on the alarm to be returned.");
	abstract("Resolution for this check is %d seconds.  Wait up",SLACK);
 	abstract("to %d seconds for the alarm to occur.  Verify ",TIME+SLACK+1 );
	abstract("alarm sent a signal.");
	start
		set_sig();
		get_time();
		testing("Call alarm( %d ), expecting a return value of 0.", TIME);
		start
			expecting ( 0 );
			alarm((unsigned)TIME);
		finis
		prep("Wait for %d seconds.",TIME/2);
		start
			long t;

			t = bs+TIME/2;
			while( t > time( (ttime *)0 ) && !blasted );
		finis
		chk_sig(0);
		testing("Call alarm( %d ), expecting the amount of time", TIME );
		testing("remaining on the alarm clock to be returned.  Verify");
		testing("the amount of time remaining is a value between %d",TIME/2-SLACK);
		testing("and %d.", TIME/2+SLACK);
		start
			expecting( VOID );
			ret_val = (long) alarm((unsigned) TIME);
			if ( ret_val > TIME/2+SLACK || ret_val < TIME/2-SLACK )
				warning("Alarm returned a value outside of %d and %d: %d.",
				  TIME/2-SLACK, TIME/2+SLACK, ret_val );
		finis
		get_time();
		testing("Wait for up to %d seconds for the alarm.",WAIT);
		start
			while( bs+WAIT > time( (ttime *)0 ) && !blasted );
			expecting( VOID );
			alarm((unsigned) 0 );
		finis
		chk_sig( 1 );
	finis
	done();
	/*NOTREACHED*/
}
siggy time_bomb( signal )
int signal;
{
	char *sig_lookup();

	if( signal != SIGALRM )
		severr("Caught stray signal: '%s' in signal handler.",
		  sig_lookup ( signal ) );
	blasted = 1;
}
set_sig() {
	prep("Set signal catching for SIGALRM.");
	start		
		blasted = 0;
		exit_on( ERROR );
		expecting( SUCCESS );
		signal(SIGALRM, time_bomb);
	finis
}
get_time() {
	prep("Call time to get the current time before the alarm.");
	start
		expecting ( SUCCESS );
		bs = time((ttime *)0);
	finis
}
chk_time( t )
int t;
{
	testing("Call time to get the current time after the alarm.");
	start
		expecting ( SUCCESS );
		as = time((ttime *)0);
		if ( (as - bs) > ( t + SLACK ) )
			warning("alarm slept longer than expected.");
		if ( (as - bs) < ( t - SLACK ) )
			warning("alarm did not sleep as long as expected.");
	finis
}
chk_sig( flg )
int flg;
{
	if( flg ) {
		testing("Verify that a signal was caught.");
		start
			if( !blasted )
				error("alarm did not send a signal.");
		finis
	}
	else {
		testing("Verify that no signal was caught.");
		start
			if( blasted )
				error("alarm sent a signal.");
		finis

	}
}
