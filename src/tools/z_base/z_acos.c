/*LINTLIBRARY*/
/*
 * z_acos.c
 */

#ifdef	UNDEF
#ident	"@(#)z_acos.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef acos
double acos();
#endif

static int success( val )
double *val;
{
	return( (*val) >= 0 && (*val) < 3.14159265358979323847 );
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

static struct ftable Tacos = {
	"acos",
	"a return value of at least 0 and at most pi",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	1,
};

double  Zacos(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = acos(x);
	evaluate( &Tacos, (pointer)&ret_val );
	return( ret_val );
}

