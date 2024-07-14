#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#ident	"@(#)calloc1.c	1.2"

/*
** calloc
**	Test the calloc function.
**
**	In this case test the case of calloc 0 bytes, which should
**	return a NULL pointer.
*/

char	*calloc();

main()
{
	setup("malloc");
	funct("calloc");

	abstract("calloc is supposed to return a NULL pointer when");
	abstract("asked to allocate 0 bytes.  This tests that case.");
	start
		testing("Attempt to calloc() 0, 1 byte objects.");
		start
			expecting((char *)0);
			(void)calloc(0,sizeof(char));
		finis		
		testing("Attempt to calloc() 1, 0 byte object.");
		start
			expecting((char *)0);
			(void)calloc(1,0);
		finis		
	finis
	done();
	/*NOTREACHED*/
}
