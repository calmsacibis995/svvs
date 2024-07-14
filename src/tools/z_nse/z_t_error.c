/*LINTLIBRARY*/
/*
 * z_t_error.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_error.c	1.3"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_error
void t_error();
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
int *sv_getv();
#endif

static struct ftable Tt_error = {
	"t_error",
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

void  Zt_error( errmsg )
char *errmsg;
{
	errno = 0;
	t_error( errmsg );
	evaluate( &Tt_error, (pointer)0 );
	return;
}

