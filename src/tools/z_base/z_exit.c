/*LINTLIBRARY*/
/*
 * z_exit.c
 */

#ifdef	UNDEF
#ident	"@(#)z_exit.c	1.3"
#endif

#include <sv_env.h>

#ifndef exit
void exit();
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

static struct ftable Texit = {
	"exit",
	"no return from the function",
	"not defined",
	format,
	compare,
	GONE,
	(ifunctptr)0,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

void  Zexit(status)
int status;
{
	errno = 0;
	exit(status);
	evaluate( &Texit, (pointer)0 );
	return;
}

