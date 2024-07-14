#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)umount.c	1.1"

char *subs[] = {
	"umount0",	/* ordinary cases of umount			*/
	"umount1",	/* umount on read only file sys			*/
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("umount");
	funct("umount");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
