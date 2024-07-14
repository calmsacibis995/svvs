/*LINTLIBRARY*/
/*
 * z_srand.c
 */

#ifdef	UNDEF
#ident	"@(#)z_srand.c	1.3"
#endif

#include <sv_env.h>

#ifndef srand
void srand();
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

static struct ftable Tsrand = {
	"srand",
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

void  Zsrand(seed)
unsigned seed;
{
	errno = 0;
	srand(seed);
	evaluate( &Tsrand, (pointer)0 );
	return;
}

