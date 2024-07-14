/*LINTLIBRARY*/
/*
 * z_exp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_exp.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef exp
double exp();
#endif

static int success( val )
double *val;
{
	return( (*val) >= 0.0 && (*val) != HUGE );
}

static int failure( val )
double *val;
{
	return( (*val) == HUGE );
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

static struct ftable Texp = {
	"exp",
	"a return value of at least 0.0 and not HUGE",
	"a return value of HUGE",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getd,
	0,
};

double  Zexp(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = exp(x);
	evaluate( &Texp, (pointer)&ret_val );
	return( ret_val );
}

