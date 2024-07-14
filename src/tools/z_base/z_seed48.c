/*LINTLIBRARY*/
/*
 * z_seed48.c
 */

#ifdef	UNDEF
#ident	"@(#)z_seed48.c	1.2"
#endif

#include <sv_env.h>

#ifndef seed48
unsigned short *seed48();
#endif

static int success( val )
unsigned short **val;
{
	return( (*val)? 1 : 0 );
}

static char *format( val )
unsigned short **val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%u", *val);
	return( buf );
}

static int compare( a, b )
unsigned short **a;
unsigned short **b;
{
	return( *a == *b );
}

unsigned *sv_getu();

static struct ftable Tseed48 = {
	"seed48",
	"a return value other than NULL",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getu,
	0,
};

unsigned short * Zseed48(seed16v)
unsigned short seed16v[3];
{
	unsigned short * ret_val;

	errno = 0;
	ret_val = seed48(seed16v);
	evaluate( &Tseed48, (pointer)&ret_val );
	return( ret_val );
}

