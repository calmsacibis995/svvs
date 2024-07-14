#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)ulimit.c	1.1"

char *subs[] = {
	"ulimit0",	/* ordinary cases of ulimit			*/
	"ulimit1",	/* ulimit on read only file sys			*/
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("ulimit");
	funct("ulimit");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
