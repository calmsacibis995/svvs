/*LINTLIBRARY*/
/*
 * z_qsort.c
 */

#ifdef	UNDEF
#ident	"@(#)z_qsort.c	1.3"
#endif

#include <sv_env.h>

#ifndef qsort
void qsort();
#endif

static char *format() {
	return( "void/undefined" );
}

static int compare() {
#ifdef	DEVELOPMENT
	warning( "sv_test for void function called" );
#endif
	return( 0 );
}

#ifdef	DEVELOPMENT
void sv_getv();
#endif

static struct ftable Tqsort = {
	"qsort",
	"return from the function",
	"not defined",
	format,
	compare,
	RET,
	(ifunctptr)0,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

void  Zqsort(base, nel, width, compar)
char *base;
unsigned nel;
unsigned width;
int (*compar)();
{
	errno = 0;
	qsort(base, nel, width, compar);
	evaluate( &Tqsort, (pointer)0 );
	return;
}

