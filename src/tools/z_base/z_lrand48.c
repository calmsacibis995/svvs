/*LINTLIBRARY*/
/*
 * z_lrand48.c
 */

#ifdef	UNDEF
#ident	"@(#)z_lrand48.c	1.2"
#endif

#include <sv_env.h>

#ifndef lrand48
long lrand48();
#endif

static int success( val )
long *val;
{
	return( (*val) >= 0 && (*val) < 2147483648 );
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

static struct ftable Tlrand48 = {
	"lrand48",
	"a return value of at least 0 and less than 2147483648",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getl,
	0,
};

long  Zlrand48(){
	long ret_val;

	errno = 0;
	ret_val = lrand48();
	evaluate( &Tlrand48, (pointer)&ret_val );
	return( ret_val );
}

