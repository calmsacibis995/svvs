#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)unlink.c	1.1"

char *subs[] = {
	"unlink0",	/* ordinary cases of unlink			*/
	"unlink1",
	"unlink2",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("unlink");
	funct("unlink");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
