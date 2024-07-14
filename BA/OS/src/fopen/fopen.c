#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fopen.c	1.4"

char *subs[] = {
	"fopen0",	/* ordinary cases of fopen			*/
	"fopen1",	/* suid root - fopen of read only filsys for w	*/
	"fopen3",	/* fopen of pure text segment in execution	*/
	"freopen0",	/* ordinary cases of freopen			*/
	"freopen1",	/* suid root - freopen of read only filsys for w*/
	"freopen3",	/* freopen of pure text segment in execution	*/
	"fdopen0",	/* ordinary case of fdopen			*/
/*  fopen2 and freopen2 were deleted  */
};

# define	NSUBTESTS	(sizeof(subs)/sizeof(pointer))

main()
{
	int i;
	setup("fopen");
	funct("fopen");
	for( i=0 ; i < NSUBTESTS ; i++ )
		subtest(subs[i]); 
	done();
}
