/*LINTLIBRARY*/
/*
 * z_floor.c
 */

#ifdef	UNDEF
#ident	"@(#)z_floor.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef floor
double floor();
#endif

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

static struct ftable Tfloor = {
	"floor",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	0,
};

double  Zfloor(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = floor(x);
	evaluate( &Tfloor, (pointer)&ret_val );
	return( ret_val );
}

