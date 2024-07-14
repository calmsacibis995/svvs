
#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)lockf.c	1.7"

char *subs[] = {
	"lockf0",
#ifndef	SYSVR_2
	"lockf1",	/*  tests mandatory (enforced) record locking  */
	"lockf2",	/*  deadlock tests using mandatory record locking  */
	"lockf3",	/*  deadlock tests using mandatory record locking  */
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("lockf");
	funct("lockf");
#ifdef	LOCKF
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 

#ifdef	SYSVR_2
	unsupported("Enforcement mode record/file locking not supported in System V Release 2");
#endif

#else
	unsupported ("lockf is not supported on this system.");
#endif
	done();
}
