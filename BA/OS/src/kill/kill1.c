#include <sv_base.h>

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill1.c	1.6"

/*
 * kill1
 *	Test the kill function - error cases.
 *
 */

# define	KILLME0	"kill_me_0"
# define	KILLME1	"kill_me_1"

main() {
	int		cpid;

	setup("kill");
	funct("kill");

	exit_on ( ERROR );

	abstract("Call kill with invalid signal (-1).");
	abstract("Verify failure with errno set to EINVAL, and an");
	abstract("unterminated child process.");
	start
		prep("Fork and exec a child process.");
		start
			if( !( cpid = fork() ) )
				execlp(  KILLME0,  KILLME0, (char *)0 );
		finis
		prep("Wait fifteen seconds before calling kill.");
		start
			ttime	t;
			ttime	time();
			t = time( (ttime *)0) + 15;
			while( time( (ttime *)0 )  < t );
		finis
		testing("Call kill( %d, -1 ).", cpid );
		start
			expecting ( EINVAL );
			kill( cpid, -1 );
		finis
		testing("Call wait, and verify the childs exit status.");
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

	testing("Kill with an invalid pid");
	testing("Verify failure with errno set to ESRCH.");
	start
		expecting ( ESRCH );
		kill(cpid, SIGKILL);
	finis

	abstract("Kill a process which does not have matching real or");
	abstract("effective uid.  Verify failure with errno set to EPERM,");
	abstract("and an unterminated child process.");
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
			expecting ( EPERM );
			kill( cpid, SIGKILL );
		finis
		testing("Call wait, and verify the childs exit status.");
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
