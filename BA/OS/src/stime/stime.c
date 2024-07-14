#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)stime.c	1.1"

char *subs[] = {
	"stime0",	/* ordinary cases of stime			*/
	"stime1",	/* stime on read only file sys			*/
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("stime");
	funct("stime");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
