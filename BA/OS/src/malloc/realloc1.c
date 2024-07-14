#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#define	SIZ	30

#ident	"@(#)realloc1.c	1.1"

/*
** realloc
**	Test the realloc function.
**
**	In this case test the case of realloc to 0 bytes, which should
**	return a NULL pointer.
*/

char	*realloc();
char	*malloc();

main()
{
	char	*p;
	int		i;
	setup("malloc");
	funct("realloc");

	abstract("realloc() is supposed to return a NULL pointer when");
	abstract("asked to allocate 0 bytes.  This tests that case.");
	start
		preparation("Allocate a small amount of memory and fill");
		preparation("it with a pattern.");
		start
			p = malloc(SIZ);
			for(i = 0; i < SIZ; ++i){
				p[i] = (char)i;
			}
		finis
		testing("realloc() 0 bytes, realloc() should return 0 and the");
		testing("previously allocated block should be unchanged.");
		start
			expecting((char *)0);
			(void)realloc(p,0);
			for(i=0; i < SIZ; ++i){
				if(p[i] != (char)i){
					error("Previously allocated block changed.");
					break;
				}
			}
		finis		
	finis
	done();
	/*NOTREACHED*/
}
