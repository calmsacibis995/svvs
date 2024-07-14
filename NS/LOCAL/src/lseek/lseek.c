#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)lseek.c	1.2"

char *subs[] = {
	"lseek0",	/* */
	"lseek1",	/* */
	"lseek2",	/* */
	"lseek3",	/* */
	"lseek4",	/* */
	"lseek5",	/* */
	"lseek6",	/* */
	"lseek7",	/* */
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("lseek");
	funct("lseek");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
