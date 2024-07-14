#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)utime.c	1.1"

char *subs[] = {
	"utime0",	/* ordinary cases of utime			*/
	"utime1",	/* utime on read only file sys			*/
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("utime");
	funct("utime");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
