#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)exec.c	1.3"

char *subs[] = {
	"exec0",	/* ordinary cases of exec			*/
#ifndef SYSVR_2		/* SVR3 addons	*/
	"exec5",
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("exec");
	funct("exec");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
