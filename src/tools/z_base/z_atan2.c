/*LINTLIBRARY*/
/*
 * z_atan2.c
 */

#ifdef	UNDEF
#ident	"@(#)z_atan2.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef atan2
double atan2();
#endif

static int success( val )
double *val;
{
	return( (*val) > -3.14159265358979323847 && (*val) < 3.14159265358979323847 );
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

static struct ftable Tatan2 = {
	"atan2",
	"a return value of at least -pi and at most pi",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	1,
};

double  Zatan2(y, x)
double y;
double x;
{
	double ret_val;

	errno = 0;
	ret_val = atan2(y, x);
	evaluate( &Tatan2, (pointer)&ret_val );
	return( ret_val );
}

