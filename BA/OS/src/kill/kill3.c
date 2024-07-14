#include <sv_base.h>

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill3.c	1.7"

/*
 * kill3
 *	Test the kill function - pid = -pgid
 *
 */
# define	KILL_ME_2	"kill_me_2"

main() {
	int		cpid;

	setup("kill");
	funct("kill");

	exit_on ( ERROR );

	abstract("Kill a group of processes with SIGTERM and pid of -pgid.");
	abstract("Fork a child process, which calls setpgrp, sets signal");
	abstract("catching, and forks some grandchild processes.  Verify");
	abstract("terminated grandchild processes, and an unterminated");
	abstract("child process.  The child process will wait for the");
	abstract("grandchild processes.");
	start
		prep("Fork and exec a child process.");
		start
			if( !( cpid = fork() )) 
				execlp(  KILL_ME_2,  KILL_ME_2, (char *)0 );
		finis

		prep("Wait thirty seconds before calling kill.");
		start
			ttime	t;
			ttime	time();
			t = time( (ttime *)0) + 30;
			while( time( (ttime *)0 )  < t );
		finis

		testing("Call kill( %d, SIGTERM ).", -cpid );
		start
			kill( (-cpid), SIGTERM );
		finis
		testing("Call wait, and verify the child's exit status.");
		start
			int sts;
			expecting ( cpid );
			wait( &sts );
			if ( sts )
				error("%s\n%s%d\n%s%d",
				 "Child process returned unexpected exit status.",
				  "expected value:	",
				  0,
				  "actual value:	",
				  sts );
		finis
	finis

	done();
	/*NOTREACHED*/
}
