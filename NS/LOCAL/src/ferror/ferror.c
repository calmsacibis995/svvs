#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)ferror.c	1.1"

char *subs[] = {
		"clearerr0",
		"clearerr1",
		"feof0",
		"ferror0",
		"fileno0",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("ferror");
	funct("ferror");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
