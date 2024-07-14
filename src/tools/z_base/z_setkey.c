/*LINTLIBRARY*/
/*
 * z_setkey.c
 */

#ifdef	UNDEF
#ident	"@(#)z_setkey.c	1.3"
#endif

#include <sv_env.h>

#ifndef setkey
void setkey();
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
void	sv_getv();
#endif

static struct ftable Tsetkey = {
	"setkey",
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

void  Zsetkey(key)
char *key;
{
	errno = 0;
	setkey(key);
	evaluate( &Tsetkey, (pointer)0 );
	return;
}

