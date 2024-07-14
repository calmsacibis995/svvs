#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)popen.c	1.2"

char *subs[] = {
	"popen0",	/* */
	"popen1",	/* */
	"popen2",	/* */
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("popen");
	funct("popen");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	funct("pclose");
	done();
}
