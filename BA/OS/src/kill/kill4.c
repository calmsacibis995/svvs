#include	<sv_base.h>

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill4.c	1.5"

/*
 * kill4
 *	Test the kill function - kill as root with mismatched uid.
 *
 */

# define	KILLME1	"kill_me_1"

main() {
	int		cpid;

	setup("kill");
	funct("kill");

	exit_on ( ERROR );

	abstract("Kill a process which does not have matching real or");
	abstract("effective uid while running as root.  Verify the");
	abstract("child process is terminated.");
	start
		prep("Fork and exec a child process.");
		start
			if( !( cpid = fork() ) )
				execlp(  KILLME1,  KILLME1, (char *)0 );
		finis
		prep("Wait fifteen seconds before calling kill.");
		start
			ttime	t;
			ttime	time();
			t = time( (ttime *)0) + 15;
			while( time( (ttime *)0 )  < t );
		finis
		testing("Call kill( %d, SIGKILL ).", cpid );
		start
			kill( cpid, SIGKILL );
		finis
		testing("Call wait, and verify the childs exit status.");
		start
			int sts;

			expecting ( cpid );
			wait( &sts );
				if ( (sts & ~0200) != SIGKILL )
				error("%s\n%s%d\n%s%d",
				 "Child process returned unexpected exit status.",
				  "expected value:	",
				  SIGKILL,
				  "actual value:	",
				  sts );
		finis
	finis
	done();
	/*NOTREACHED*/
}
