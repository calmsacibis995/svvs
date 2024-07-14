/*LINTLIBRARY*/
/*
 * z_cos.c
 */

#ifdef	UNDEF
#ident	"@(#)z_cos.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef cos
double cos();
#endif

static int success( val )
double *val;
{
	return( (*val) <= 1.0 && (*val) >= -1.0 );
}

static char *format( val )
double *val;
{
	static char buf[STRVAL];
	int i,l;

	(void) sprintf( buf, "%g (0x", *val );
	l = strlen( buf );
	for( i = 0; i < sizeof( double ); i++ )
		sprintf( buf+l+(2*i), "%02x", ((int)(*(((char *)val)+i)))&0xff );
	sprintf( buf+l+(2*i), ")" );
	return( buf );
}

static int compare( a, b )
double *a;
double *b;
{
	return( sv_dcmp( *a, *b ) );
}

double *sv_getd();

static struct ftable Tcos = {
	"cos",
	"a return value of at most 1.0 and at least -1.0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	1,
};

double  Zcos(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = cos(x);
	evaluate( &Tcos, (pointer)&ret_val );
	return( ret_val );
}

