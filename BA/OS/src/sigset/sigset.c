/*
**EMACS_MODES: tabstop=4
*/

#ident	"@(#)sigset.c	1.3"

#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2

char	*tests[] = {
	"sigset0",
	"sighold",
	"sigrelse",
	"sigignore",
};
#define	NTESTS	(sizeof(tests)/sizeof(char *))

main()
{
	int	i;

	setup("sigset");
	for(i = 0; i < NTESTS; ++i){
		subtest(tests[i]);
	}
	done();
}

#else

main(argc,argv)
int		argc;
char	*argv[];
{
	setup("sigset");
	unsupported("SIGSET(BA_OS) page is not supported in System V release 2");
	done();
}
#endif
