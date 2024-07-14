/*LINTLIBRARY*/
/*
 * z__exit.c
 */

#ifdef UNDEF
#ident	"@(#)z__exit.c	1.4"
#endif

#include <sv_env.h>

#ifndef _exit
void _exit();
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

static struct ftable T_exit = {
	"_exit",
	"no return from the function",
	"return from the function",
	format,
	compare,
	GONE,
	RET,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

void  Z_exit(status)
int status;
{
	errno = 0;
	_exit(status);
	evaluate( &T_exit, (pointer)0 );
	return;
}

