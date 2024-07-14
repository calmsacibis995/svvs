#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mknod.c	1.3"

char *subs[] = {
	"mknod0",	/* ordinary cases of mknod			*/
	"mknod1",
#ifndef SYSVR_2
	"mknod2",
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("mknod");
	funct("mknod");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
