#include	<sv_base.h> /*replacement macros */
#include	<signal.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	REQ0	0

#ident	"@(#)ptrace1.c	1.2"


/*
** PTRACE1
**	This is the process being exec'd from the test for ptrace.
** In the following the CHILD process will
** perform a ptrace with request = 0 to
** allow the parent process to trace this
** child. The last 3 arguments passed to
** ptrace are ignored.  No return value is
** defined for this call.
*/
main()
{
#ifdef PTRACE
setup("ptrace");
start
	/* CHILD process */
	expecting (VOID);
	ptrace(REQ0, 0,0,0);
	expecting (SUCCESS);
	kill(getpid(), SIGUSR1);
finis
exit(0);
/*NOTREACHED*/
#endif
}
