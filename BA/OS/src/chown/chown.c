#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)chown.c	1.2"

char *subs[] = {
	"chown0",	/* chown while executing as svvs	*/
	"chown1",	/* chown while executing as bin		*/
	"chown2",	/* chown while executing as root	*/
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("chown");
	funct("chown");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
