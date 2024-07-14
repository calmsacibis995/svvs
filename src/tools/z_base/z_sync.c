/*LINTLIBRARY*/
/*
 * z_sync.c
 */

#ifdef	UNDEF
#ident	"@(#)z_sync.c	1.3"
#endif

#include <sv_env.h>

#ifndef sync
void sync();
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

static struct ftable Tsync = {
	"sync",
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

void  Zsync(){
	errno = 0;
	sync();
	evaluate( &Tsync, (pointer)0 );
	return;
}

