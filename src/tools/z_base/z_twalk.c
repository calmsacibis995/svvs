/*LINTLIBRARY*/
/*
 * z_twalk.c
 */

#ifdef	UNDEF
#ident	"@(#)z_twalk.c	1.3"
#endif

#include <search.h>
#include <sv_env.h>

#ifndef twalk
void twalk();
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

static struct ftable Ttwalk = {
	"twalk",
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

void  Ztwalk(root, action)
char *root;
void (*action)();
{
	errno = 0;
	twalk(root, action);
	evaluate( &Ttwalk, (pointer)0 );
	return;
}

