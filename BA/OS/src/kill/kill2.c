#include	<sv_base.h>

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill2.c	1.6"

/*
 * kill2
 *	Test the kill function - pid = 0
 *
 */

# define	KILL_ME_0	"kill_me_0"
# define	NPROCS		7

siggy 		catch_sig();
static	int	got_sig = 0;

main() {
	int		sts;
	int		cpid[NPROCS];
	int		wpid;
	int		i,j;
	ttime		t;
	ttime		time();

	setup("kill");
	funct("kill");

	exit_on ( ERROR );

	abstract("Kill a group of processes with kill( 0, SIGTERM ).");
	abstract("Catch SIGTERM in the calling process, and verify");
	abstract("the rest of the process group terminated.");
	start
		prep("Reset process group and fork off NPROC children.");
		prep("Each one of those children arranges to kill itself.");
		start
			expecting ( VOID );
			(void) setpgrp();

			for(i = 0;i<NPROCS;i++) {
				expecting ( SUCCESS );
				if( !( cpid[i] = fork() )) 
					(void) execlp(  KILL_ME_0,  KILL_ME_0, (char *)0 );
			}
		finis
		prep("Set signal catching for SIGTERM.");
		start
			expecting ( VOID );
			signal( SIGTERM, catch_sig );
		finis
		prep("Wait fifteen seconds before calling kill.");
		start
			ttime	t;
			ttime	time();
			t = time( (ttime *)0) + 15;
			while( time( (ttime *)0 )  < t );
		finis
		testing("Call kill( 0, SIGTERM ).");
		start
			kill( 0, SIGTERM );
		finis
		testing("Call wait, and verify each childs exit status.");
		start
			int sts;

			for(i = 0; i < NPROCS; i++) {
				expecting ( SUCCESS );
				abort_on( ERROR );
				wpid = wait(&sts);
				for(j=0; j<NPROCS; j++) {
					if ( wpid == cpid[j] ) {
						cpid[j] = 0;
						break;
					}
				}
				if ( (sts & ~0200) != SIGTERM )
					error("%s\n%s%d\n%s%d",
					 "Child process returned unexpected exit status.",
					  "expected value:	",
					  SIGTERM,
					  "actual value:	",
					  sts & ~0200 );
			}
			if ( got_sig == 0 ) {
				error("Did not catch SIGTERM.");
			}
		finis
	finis
	done();
	/*NOTREACHED*/
}
siggy catch_sig(sig)
int sig;
{
	if (sig == SIGTERM ) got_sig = 1;
	else {
		severr("Caught unexpected signal.");
	}
}
