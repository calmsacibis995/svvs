/*LINTLIBRARY*/
/*
 * z_sqrt.c
 */

#ifdef	UNDEF
#ident	"@(#)z_sqrt.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef sqrt
double sqrt();
#endif

static double x_arg = 0;

static int success( val )
double *val;
{
	return( (*val) > 0.0 || ( x_arg == 0.0 && (*val) == 0.0 ) );
}

static int failure( val )
double *val;
{
	return( ( x_arg < 0.0 && (*val) == 0.0 ) );
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

static struct ftable Tsqrt = {
	"sqrt",
	"a return value greater than 0.0 or if x is 0.0 a return value of 0.0",
	"a return value of zero if x is less than 0.0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getd,
	0,
};

double  Zsqrt(x)
double x;
{
	double ret_val;

	errno = 0;
	x_arg = x;
	ret_val = sqrt(x);
	evaluate( &Tsqrt, (pointer)&ret_val );
	return( ret_val );
}

