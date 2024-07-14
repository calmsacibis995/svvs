#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)chmod.c	1.3"

char *subs[] = {
	"chmod0",	/* ordinary cases of chmod			*/
	"chmod1",	/* chmod on read only file sys			*/
#ifndef	SYSVR_2
	"chmod2",	/*Enforcement mode record/file locking test	*/
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("chmod");
	funct("chmod");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
