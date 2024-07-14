/*LINTLIBRARY*/
/*
 * z_perror.c
 */

#ifdef	UNDEF
#ident	"@(#)z_perror.c	1.3"
#endif

#include <sv_env.h>

#ifndef perror
void perror();
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

static struct ftable Tperror = {
	"perror",
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

void  Zperror(s)
char *s;
{
	perror(s);
	evaluate( &Tperror, (pointer)0 );
	return;
}

