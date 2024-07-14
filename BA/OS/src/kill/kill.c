#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)kill.c	1.3"

char *subs[] = {
	"kill0",	/* */
	"kill1",	/* */
	"kill2",	/* */
	"kill3",	/* */
	"kill4",	/* */
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("kill");
	funct("kill");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
