#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include <signal.h>
#include <svvs.h>
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)signal_me2.c	1.7"

int sigs[] = {
	SIGTERM,
	SIGUSR1,
	SIGUSR2,
	SIGHUP,
	SIGINT,
	SIGQUIT,
	SIGTRAP,
	SIGFPE,
	SIGSYS,
	SIGPIPE,
	SIGALRM,
#ifndef SYSVR_2
	SIGABRT,
#endif
};

# define	NSIGS	(sizeof(sigs)/sizeof(int))

static int ncaught = 0;

main()
{
	ttime t,time();
	siggy (*sts)();

#ifndef	SYSVR_2
	void (*signal())();
#else
	int (*signal())();
#endif

	int catch_sig();
	int i;

	for( i = 0; i < NSIGS; i++ )
	{
		expecting ( VOID );
		sts = (siggy (*)())signal(sigs[ i ], catch_sig);
		if ( sts == (siggy (*)())(-1) )
		{
severr("Unexpected return value from signal.\nExpected value was %ld.\nThe actual value was %ld.", (long)SIG_DFL, (long)sts);	/*  HALT */
		}
	}

	expecting ( VOID );
	t = time( (ttime *)0) + 50;

	expecting ( VOID );
	while( time( (ttime *)0 )  < t );
	if ( ncaught == 0 )
		error("child process did not catch a signal");

	done();
}
int catch_sig( s )
int s;
{
	siggy (*sts)();

#ifndef	SYSVR_2
	void (*signal())();
#else
	int (*signal())();
#endif
	if ( ncaught == 0 )
	{
		expecting ( VOID );
		sts = (siggy (*)())signal( s, catch_sig);
		if ( sts == (siggy (*)())(-1) )
		{
severr("unexpected return value or errno.\nThe unexpected value was %ld.", (long)sts);	/* HALT */
		}
	}
	ncaught++;
}
