/*LINTLIBRARY*/
/*
 * z_clock.c
 */

#ifdef	UNDEF
#ident	"@(#)z_clock.c	1.2"
#endif

#include <sv_env.h>

#ifndef clock
long clock();
#endif

static int success( val )
long *val;
{
	return( (*val) >= 0 );
}

static char *format( val )
long *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%ld", *val );
	return( buf );
}

static int compare( a, b )
long *a;
long *b;
{
	return( *a == *b );
}

long *sv_getl();

static struct ftable Tclock = {
	"clock",
	"a return value of at least 0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getl,
	0,
};

long  Zclock(){
	long ret_val;

	errno = 0;
	ret_val = clock();
	evaluate( &Tclock, (pointer)&ret_val );
	return( ret_val );
}

