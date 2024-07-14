#include	<sv_base.h>
#include	<errno.h>
#include	<signal.h>
#include	<stdio.h>
#include	<fcntl.h>
#include	<sv_env.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ident	"@(#)abort.c	1.6"

#ifndef SYSVR_2
#define		SIGNAME		"SIGABRT"
#define		SIGNUM		SIGABRT
#define		BAD_STS		error("Wait did not return a value indicating %s.", SIGNAME);
#else
#ifdef SIGABRT
#define		SIGNAME		"SIGABRT"
#define		SIGNUM		SIGABRT
#define		BAD_STS		warning("%s %s\n",FUTURDIR,"SIGABRT has been implemented")
#else
#define		SIGNAME		"SIGIOT"
#define		SIGNUM		SIGIOT
#define		BAD_STS		error("Wait did not return a value indicating %s.", SIGNAME);
#endif
#endif

FILE	*fstr;
FILE	*temp_fopen();

/*
** ABORT
**	Test the abort function
**
**	This test also accounts for an implementation that
**	issues a SIGABRT.
*/

main()
{
	int	pid;
	int	wpid;
	int	wait_val;
	int	status;

	setup( "abort" );
	funct( "abort" );

	abstract("Testing a normal instance of abort.");
	abstract("Have a child process call abort." );
	abstract("The child's open temp file should be flushed,");
	abstract("and wait should indicate the child");
	abstract("terminated from %s.", SIGNAME );
	start
		abort_on(ERROR);
		prep("Fork a child process.");
		start
			expecting ( SUCCESS );
			if(!( pid = fork())) {
				prep("Open a temp file, and call fwrite to write");
				prep("a character to it.");
				start
					fstr = temp_fopen( "abort","w" );
					expecting( 1 );
					fwrite("Z", 1, 1, fstr);
				finis
				prep("Reset signal handling for %s to SIG_DFL.",SIGNAME);
				start
					expecting(SUCCESS);
					signal( SIGNUM, SIG_DFL);
				finis
				testing("Call abort.");
				start
					abort();
				finis
				cleanup("Call exit, because previous abort failed.");
				start
					exit(0);
				finis
			}
		finis
		expecting( pid );
		wait(&status);
		testing("Verify the child was terminated by %s.",SIGNAME);
		start
			if ( (status & ~0200 ) != SIGNUM )
				BAD_STS;
		finis
		testing("Verify that the temp file was flushed.");
		start
			char ch;
			break_on( ERROR );
			fstr = temp_fopen( "abort","r" );
			expecting( 1 );
			fread( &ch, 1, 1, fstr);
			if( ch != 'Z' )
				error("Temp file was not flushed and closed.");
		finis
	finis
	done();
/*
 * NOTREACHED
 */
}
