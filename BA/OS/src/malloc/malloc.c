#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)malloc.c	1.2"

char *subs[] = {
	"malloc0",	/* ordinary cases of malloc			*/
	"malloc1",
	"free",
	"realloc",
	"realloc1",
	"calloc",
	"calloc1",
	"mallinfo",
	"mallopt",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("malloc");
	funct("malloc");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
