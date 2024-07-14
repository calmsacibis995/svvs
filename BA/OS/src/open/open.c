#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident    "@(#)open.c	1.2"

char *subs[] = {
	"open0",	/* ordinary cases of open			*/
	"open1",
	"open2",
	"open3",
#ifndef	SYSVR_2
	"open4"		/*  test O_SYNC, record locking  */
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("open");
	funct("open");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
