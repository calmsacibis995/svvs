/*LINTLIBRARY*/
/*
 * z_setbuf.c
 */

#ifdef	UNDEF
#ident	"@(#)z_setbuf.c	1.3"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef setbuf
void setbuf();
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

static struct ftable Tsetbuf = {
	"setbuf",
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

void  Zsetbuf(stream, buf)
FILE *stream;
char *buf;
{
	errno = 0;
	setbuf(stream, buf);
	evaluate( &Tsetbuf, (pointer)0 );
	return;
}

