#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)signal.c	1.2"

char *subs[] = {
	"signal0",	/* */
	"signal1",	/* */
	"signal2",	/* */
	"signal3",	/* */
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("signal");
	funct("signal");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
