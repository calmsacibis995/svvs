/*LINTLIBRARY*/
/*
 * z_tzset.c
 */

#ifdef	UNDEF
#ident	"@(#)z_tzset.c	1.3"
#endif

#include <time.h>
#include <sv_env.h>

#ifndef tzset
void tzset();
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

static struct ftable Ttzset = {
	"tzset",
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

void  Ztzset(){
	errno = 0;
	tzset();
	evaluate( &Ttzset, (pointer)0 );
	return;
}

