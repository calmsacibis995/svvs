/*LINTLIBRARY*/
/*
 * z_erfc.c
 */

#ifdef	UNDEF
#ident	"@(#)z_erfc.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef erfc
double erfc();
#endif

static int success( val )
double *val;
{
	return( (*val) >= 0.0 );
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

static struct ftable Terfc = {
	"erfc",
	"a return value of at least 0.0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	0,
};

double  Zerfc(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = erfc(x);
	evaluate( &Terfc, (pointer)&ret_val );
	return( ret_val );
}

