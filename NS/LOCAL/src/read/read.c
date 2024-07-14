#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)read.c	1.4"

char *subs[] = {
	"read0",	/*Issue 1 cases of read*/
	"read1",	/*Issue 2 cases of read*/
#ifndef	SYSVR_2
	"read2",	/*  deadlock tests using mandatory record locking  */
	"read3",	/*  deadlock tests using mandatory record locking  */
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("read");
	funct("read");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
