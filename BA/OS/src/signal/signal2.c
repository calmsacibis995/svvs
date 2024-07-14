#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)signal2.c	1.7"

/*
** signal2
**	Test the signal function - normal cases, child catches with func.
**
*/
# define	SIGNAL_ME_2	"signal_me2"

struct	acttab{
	int	signo;
	char	*signam;
	int	exp;
};

struct	acttab	sigs[] = {
	{SIGHUP,"SIGHUP",1},
	{SIGINT,"SIGINT",1},
	{SIGQUIT,"SIGQUIT",1},
	{SIGTRAP,"SIGTRAP",1},
	{SIGFPE,"SIGFPE",1},
	{SIGSYS,"SIGSYS",1},
	{SIGPIPE,"SIGPIPE",1},
	{SIGALRM,"SIGALRM",1},
	{SIGTERM,"SIGTERM",1},
	{SIGUSR1,"SIGUSR1",1},
	{SIGUSR2,"SIGUSR2",1},
#ifndef SYSVR_2
	{SIGABRT,"SIGABRT",1},
#endif
};

# define	NSIGS	(sizeof(sigs)/sizeof(struct acttab))

main()
{
	int		sts;
	int		cpid;
	int		i;
	ttime		t;
	ttime		time();

	setup("signal");

	funct("signal");

	start

		for(i=0;i<NSIGS;i++ )
		{
			abstract("Signal with %s, signal catching",sigs[i].signam);
			abstract("function. Expect 0 return value, child terminates");
			abstract("on its own.");
			start
				prep("Fork off a child.");
				start
					expecting ( SUCCESS );
					cpid = fork();
				finis
				if ( cpid == 0 )
				{
					testing("Child execs which should signal it.");
					start
						(void) execlp(  SIGNAL_ME_2,  SIGNAL_ME_2, (char *)0 );
					finis
					severr( "signal2: cannot exec" );
				}
				prep("Parent waits 5 seconds before sending signal.");
				start
					expecting ( VOID );
					t = time( (ttime *)0) + 10;
					expecting ( VOID );
					while( time( (ttime *)0 )  < t );
				finis
				testing("Signal the child process and determine that it terminated");
				testing("from the correct signal.");
				start
					expecting ( 0 );
					sts = kill(cpid, sigs[i].signo);

					expecting ( VOID );
					t = time( (ttime *)0) + 10;

					expecting ( VOID );
					while( time( (ttime *)0 )  < t );

					expecting ( 0 );
					sts = kill(cpid, sigs[i].signo);

					expecting ( VOID );
					t = time( (ttime *)0) + 10;

					expecting ( VOID );
					while( time( (ttime *)0 )  < t );

					expecting ( 0 );
					sts = kill(cpid, sigs[i].signo);

					expecting ( cpid );
					wait(&sts);
					if (i == 3 )
					{
						if ( sts != 0 )
						{
							error( "Child did not terminate as expected.\nThe expected wait value was %d.\nThe actual wait value was %d\n", 0, sts);
						}
					}
					else
					{
						if ( (sts & ~0200) != sigs[i].signo )
						{
							error( "Child did not terminate as expected.\nThe expected wait value was %d.\nThe actual wait value was %d.\n", sigs[i].signo, (sts & ~0200));
						}
					}
				finis
			finis
		}
	finis

	done();
	/*NOTREACHED*/
}
