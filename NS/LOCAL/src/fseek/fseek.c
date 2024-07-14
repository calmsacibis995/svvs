#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fseek.c	1.2"

char *subs[] = {
	"rewind0",	/* */
	"rewind1",	/* */
	"rewind2",	/* */
	"rewind3",	/* */
	"rewind4",	/* */
	"fseek0",	/* */
	"fseek1",	/* */
	"fseek2",	/* */
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("fseek");
	funct("fseek");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
