#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill0.c	1.5"

/*
** kill0
**	Test the kill function - normal case.
**
*/
# define	KILL_ME	"kill_me_0"

struct sigs {
	int	signum;
	char *signame;
} sigs[] = {
	SIGTERM,	"SIGTERM",
	SIGUSR1,	"SIGUSR1",
	SIGUSR2,	"SIGUSR2",
	0,			"0",
};

# define	NSIGS	(sizeof(sigs)/sizeof(struct sigs))

main() {
	int		cpid;
	int		i;

	setup("kill");
	funct("kill");

	for(i=0;i<NSIGS;i++ ) {
		abstract("Fork a child process.  Kill the child process");
		abstract("with %s.  Verify the child process",sigs[i].signame);
		abstract("%s.",
		  sigs[i].signum ? "terminated" : "did not terminate");
		start
			prep("Fork and exec a child process.");
			start
				exit_on( ERROR );
				if( !(cpid = fork()) )
				(void) execlp(  KILL_ME,  KILL_ME, (char *)0 );
			finis
			prep("Wait fifteen seconds before calling kill.");
			start
				ttime	t;
				ttime	time();
				t = time( (ttime *)0) + 15;
				while( time( (ttime *)0 )  < t );
			finis
			testing("Call kill( %d, %s ).", cpid, sigs[ i ].signame );
			start
				kill( cpid, sigs[ i ].signum );
			finis
			testing("Call wait, and verify the childs exit status.");
			start
				int sts;

				expecting ( cpid );
				wait( &sts );
				if ( (sts & ~0200) != sigs[ i ].signum )
					error("%s\n%s%d\n%s%d",
					 "Child process returned unexpected exit status.",
					  "expected value:	",
					  sigs[ i ].signum,
					  "actual value:	",
					  sts & ~0200 );
			finis
		finis
	}
	done();
	/*NOTREACHED*/
}
