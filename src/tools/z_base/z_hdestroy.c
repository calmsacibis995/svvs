/*LINTLIBRARY*/
/*
 * z_hdestroy.c
 */

#ifdef	UNDEF
#ident	"@(#)z_hdestroy.c	1.3"
#endif

#include <search.h>
#include <sv_env.h>

#ifndef hdestroy
void hdestroy();
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

static struct ftable Thdestroy = {
	"hdestroy",
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

void  Zhdestroy(){
	errno = 0;
	hdestroy();
	evaluate( &Thdestroy, (pointer)0 );
	return;
}

