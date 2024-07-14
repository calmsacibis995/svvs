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

#ident	"@(#)signal1.c	1.5"

/*
** signal1
**	Test the signal function - normal cases, child catches SIG_IGN.
**
*/
# define	SIGNAL_ME_1	"signal_me1"

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
	int		i,j;
	ttime		t;
	ttime		time();

	setup("signal");

	funct("signal");

	start

		for(i=0;i<NSIGS;i++ )
		{
			abstract("Signal with %s, SIG_IGN",sigs[i].signam);
			abstract("Expect 0 return value, unkilled child");
			start
				prep("Fork off a child.");
				start
					expecting ( SUCCESS );
					cpid = fork();
				finis
				if ( cpid == 0 )
				{
					testing("Child does an exec which should signal it.");
					start
						expecting ( VOID );
						(void) execlp(  SIGNAL_ME_1,  SIGNAL_ME_1, (char *)0 );
					finis
					severr( "signal1: cannot exec" );
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
					for( j = 0; j < 7; j++ )
					{
						expecting ( 0 );
						sts = kill(cpid, sigs[i].signo);
					}
					expecting ( cpid );
					wait(&sts);
					if ( sts != 0 )
					{
						error( "signal1: child did not terminate as expected");
					}
				finis
			finis
		}
	finis
	done();
	/*NOTREACHED*/
}
