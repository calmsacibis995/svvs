#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

#ident	"@(#)malloc1.c	1.2"

/*
** malloc
**	Test the malloc function.
**
**	In this case test the case of malloc 0 bytes, which should
**	return a NULL pointer.
*/

char	*malloc();

main()
{
	setup("malloc");
	funct("malloc");

	abstract("Malloc is supposed to return a NULL pointer when");
	abstract("asked to allocate 0 bytes.  This tests that case.");
	start
		testing("Attempt to malloc() 0 bytes.");
		start
			expecting((char *)0);
			(void)malloc(0);
		finis		
	finis
	done();
	/*NOTREACHED*/
}
