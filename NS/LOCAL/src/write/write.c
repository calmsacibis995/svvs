#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)write.c	1.3"

char *subs[] = {
	"write0",	/* write new files				*/
	"write1",	/* write existing files				*/
	"readwrite0",	/* read/write new files			*/
	"readwrite1",	/* read/write existing files 			*/
#ifndef	SYSVR_2
	"write2",	/*  deadlock tests using mandatory record locking  */
	"write3",	/*  deadlock tests using mandatory record locking  */
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("write");
	funct("write");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
