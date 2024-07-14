/*LINTLIBRARY*/
/*
 * z_clearerr.c
 */

#ifdef	UNDEF
#ident	"@(#)z_clearerr.c	1.3"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef clearerr
void clearerr();
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

static struct ftable Tclearerr = {
	"clearerr",
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

void  Zclearerr(stream)
FILE *stream;
{
	errno = 0;
	clearerr(stream);
	evaluate( &Tclearerr, (pointer)0 );
	return;
}

