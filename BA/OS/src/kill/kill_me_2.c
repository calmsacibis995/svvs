#include	<sv_base.h>
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill_me_2.c	1.6"

/*
 * kill_me_2
 *	Test the kill function - pid = -1
 *
 */

# define	KILL_ME_0	"kill_me_0"
# define	NPROCS		7
# define	SIG		SIGTERM

siggy 		catch_sig();
static	int	got_sig = 0;

main() {
	int		sts;
	int		cpid[NPROCS];
	int		wpid;
	int		i, j, wflg;

	setup("kill");

	testing("Call setpgrp, set signal catching for SIGTERM,");
	testing("fork and exec %d children, and wait", NPROCS );
	testing("for the children to terminate.");
	start
		expecting ( VOID );
		setpgrp();
	
		wflg = 0;

		expecting( VOID );
		signal( SIGTERM, catch_sig );
		expecting( SUCCESS );	
		exit_on ( ERROR );
		for(i = 0;i<NPROCS;i++)
			if( !(cpid[i] = fork()) )
				execlp( KILL_ME_0,  KILL_ME_0, (char *)0 );
		for(i = 0; i < NPROCS; i++) {
			expecting( VOID );
			wpid = wait(&sts);
			if (wpid == (-1) && errno == EINTR && wflg == 0) {
				wflg = 1;
				wpid = wait(&sts);
			}
			for(j=0; j<NPROCS; j++) {
				if ( wpid == cpid[j] ) {
					cpid[j] = 0;
					break;
				}
			}
			if (j == NPROCS) {
				error("wait() terminated abnormally" );
			}
			if ( (sts & ~0200) != SIGTERM )
				error("%s\n%s%d\n%s%d",
				 "Child process returned unexpected exit status.",
				  "expected value:	",
				  SIGTERM,
				  "actual value:	",
				  sts );
		}
		if ( got_sig == 0 )
			error("Did not catch signal.");
	finis
	done();
}
siggy catch_sig(sig)
int sig;
{
	if (sig == SIGTERM) got_sig = 1;
	else {
		error("Caught unexpected signal");
	}
}
