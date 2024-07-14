#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getpid.c	1.1"

char *subs[] = {
	"getpid0",	/* ordinary cases of getpid			*/
	"getpgrp",
	"getppid",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("getpid");
	funct("getpid");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
