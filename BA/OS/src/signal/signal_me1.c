#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include <signal.h>
#include <svvs.h>
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)signal_me1.c	1.6"

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
#ifndef	SYSVR_2
	SIGABRT,
#endif
};

# define	NSIGS	(sizeof(sigs)/sizeof(int))

main()
{
	ttime t,time();
	siggy (*sts)();

#ifndef	SYSVR_2
	void (*signal())();
#else
	int (*signal())();
#endif

	int i;

	for( i = 0; i < NSIGS; i++ )
	{
		expecting ( VOID );
		sts = (siggy (*)())signal(sigs[ i ], SIG_IGN);
		if ( sts == (siggy(*)())(-1) )
	{
severr("Unexpected return value from signal.\nExpected value was %ld.\nThe actual value was %ls.", (long)SIG_DFL, (long)sts);	/*  HALT */
	}
	}
	expecting ( VOID );
	t = time( (ttime *)0) + 20;
	expecting ( VOID );
	while( time( (ttime *)0 )  < t );
	done();
}
