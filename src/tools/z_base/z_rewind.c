/*LINTLIBRARY*/
/*
 * z_rewind.c
 */

#ifdef	UNDEF
#ident	"@(#)z_rewind.c	1.3"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef rewind
void rewind();
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

static struct ftable Trewind = {
	"rewind",
	"not defined",
	"not defined",
	format,
	compare,
	(ifunctptr)0,
	(ifunctptr)0,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

void  Zrewind(stream)
FILE *stream;
{
	errno = 0;
	rewind(stream);
	evaluate( &Trewind, (pointer)0 );
	return;
}

