/*LINTLIBRARY*/
/*
 * z_free.c
 */

#ifdef	UNDEF
#ident	"@(#)z_free.c	1.3"
#endif

#include <sv_env.h>

#ifndef free
void free();
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

static struct ftable Tfree = {
	"free",
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

void  Zfree(ptr)
char *ptr;
{
	errno = 0;
	free(ptr);
	evaluate( &Tfree, (pointer)0 );
	return;
}

