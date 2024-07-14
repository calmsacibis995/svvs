#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getuid.c	1.1"

char *subs[] = {
	"getuid0",	/* ordinary cases of getuid			*/
	"geteuid",
	"getgid",
	"getegid",
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("getuid");
	funct("getuid");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
