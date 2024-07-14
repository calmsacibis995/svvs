/*LINTLIBRARY*/
/*
 * z_jrand48.c
 */

#ifdef	UNDEF
#ident	"@(#)z_jrand48.c	1.2"
#endif

#include <sv_env.h>

#ifndef jrand48
long jrand48();
#endif

static int success( val )
long *val;
{
	return( (*val) >= -2147483648 && (*val) <= 2147483647 );
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

static struct ftable Tjrand48 = {
	"jrand48",
	"a return value of at least -2147483648 and less than 2147483648",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getl,
	0,
};

long  Zjrand48(xsubi)
unsigned short xsubi[3];
{
	long ret_val;

	errno = 0;
	ret_val = jrand48(xsubi);
	evaluate( &Tjrand48, (pointer)&ret_val );
	return( ret_val );
}

