#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fcntl.c	1.4"

char *subs[] = {
	"fcntl0",	/* ordinary cases of fcntl			*/
#ifdef	LOCKF
	"fcntl1",	/* cases using lockf				*/
	"fcntl2",	/* cases using lockf				*/
/*
 *  fcntl3, fcntl4 no longer exist.
 *  Their test cases have been moved into fcntl1
 */

	"fcntl5",	/*  deadlock tests */
	"fcntl6",	/*  deadlock tests */
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("fcntl");
	funct("fcntl");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
