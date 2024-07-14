/*LINTLIBRARY*/
/*
 * z_atan.c
 */

#ifdef	UNDEF
#ident	"@(#)z_atan.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef atan
double atan();
#endif

static int success( val )
double *val;
{
	return( (*val) < 1.57079632679489661924 && (*val) > -1.57079632679489661924 );
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

static struct ftable Tatan = {
	"atan",
	"a return value of at most pi/2 and at least -pi/2",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	1,
};

double  Zatan(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = atan(x);
	evaluate( &Tatan, (pointer)&ret_val );
	return( ret_val );
}

