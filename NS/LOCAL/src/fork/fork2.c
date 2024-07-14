#ident	"@(#)fork2.c	1.3"
#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

/*
** fork - Create a new process
**
** The SIG_HOLD setting for a signal is supposed to be preserved
** through a fork.  This tests whether this is the case.
**
*/
#ifndef	SYSVR_2

struct tab {
	char	*name;
	int		num;
};

struct	tab	sigs[] = {
	{	"SIGHUP",	SIGHUP	},
	{	"SIGINT",	SIGINT	},
	{	"SIGQUIT",	SIGQUIT	},
	{	"SIGILL",	SIGILL	},
	{	"SIGTRAP",	SIGTRAP	},
	{	"SIGABRT",	SIGABRT	},
	{	"SIGFPE",	SIGFPE	},
	{	"SIGSYS",	SIGSYS	},
	{	"SIGPIPE",	SIGPIPE	},
	{	"SIGALRM",	SIGALRM	},
	{	"SIGTERM",	SIGTERM	},
	{	"SIGUSR1",	SIGUSR1	},
	{	"SIGUSR2",	SIGUSR2	},
};

#define	NSGS	(sizeof(sigs) / sizeof(struct tab))

main(argc,argv)
int		argc;
char	*argv;
{
	int		i;

	sv_sigset();
	setup("fork");
	funct("fork");

		abstract("The setting SIG_HOLD for a signal is supposed to be");
		abstract("inherited by the child after a fork() if it was set");
		abstract("in the parent.  For each of the signals which can be");
		abstract("set to this setting, set SIG_HOLD and fork a child.");
		abstract("In the child, set the signal back to SIG_DFL and see");
		abstract("whether the old setting was SIG_HOLD.");
		start
		for(i = 0; i < NSGS; ++i){
			testing("Set %s to SIG_HOLD and fork a child.",sigs[i].name);
			start
				expecting(SUCCESS);
				sigset(sigs[i].num,SIG_HOLD);
				if(!fork()){
					testing("Examine %s in the child.",sigs[i].name);
					start
						expecting(SIG_HOLD);
						(void)sigset(sigs[i].num,SIG_DFL);
					finis
					exit(0);
				}
				wait((int *)0);
			finis
		}
	finis
	done();
}
#else
main(argc,argv)
int		argc;
char	*argv[];
{
	unsupported("SIG_HOLD not supported in System V Release 2");
}
#endif
