#ident	"@(#)exec6.c	1.7"
#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

main(argc,argv)
int		argc;
char	*argv[];
{
# ifndef SYSVR_2

	sv_no_sig(SIGUSR1);
	sv_sigset();
	setup("exec");

	testing("Call sigset() to find out what the previous state of");
	testing("SIGUSR1 was.  It should be SIG_HOLD.");
	start
		expecting(SIG_HOLD);
		sigset(SIGUSR1,SIG_DFL);
	finis
	done();
# else
	unsupported("The functionality tested here is not supported by SVR2");
# endif
}
