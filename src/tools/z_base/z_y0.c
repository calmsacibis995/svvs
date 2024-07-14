/*LINTLIBRARY*/
/*
 * z_y0.c
 */

#ifdef	UNDEF
#ident	"@(#)z_y0.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef y0
double y0();
#endif

static int success( val )
double *val;
{
	return( (*val) > -HUGE && (*val) < 0.520787 );
}

static int failure( val )
double *val;
{
	return( (*val) == -HUGE );
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

static struct ftable Ty0 = {
	"y0",
	"a return value greater than -HUGE and less than 0.520787",
	"a return value of -HUGE",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getd,
	1,
};

double  Zy0(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = y0(x);
	evaluate( &Ty0, (pointer)&ret_val );
	return( ret_val );
}

