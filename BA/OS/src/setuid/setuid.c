#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)setuid.c	1.1"

char *subs[] = {
	"setuid0",	/* ordinary cases of setuid			*/
	"setuid1",
	"setgid",
	"setgid1",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("setuid");
	funct("setuid");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
