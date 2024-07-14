#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)creat.c	1.2"

char *subs[] = {
	"creat0",
	"creat1",
	"creat2",
#ifndef	SYSVR_2
	"creat3",
#endif
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("creat");
	funct("creat");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
