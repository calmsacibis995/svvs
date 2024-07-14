/*LINTLIBRARY*/
/*
 * z_swab.c
 */

#ifdef	UNDEF
#ident	"@(#)z_swab.c	1.3"
#endif

#include <sv_env.h>

#ifndef swab
void swab();
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

static struct ftable Tswab = {
	"swab",
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

void  Zswab(from, to, nbytes)
char *from;
char *to;
int nbytes;
{
	errno = 0;
	swab(from, to, nbytes);
	evaluate( &Tswab, (pointer)0 );
	return;
}

