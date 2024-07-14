/*LINTLIBRARY*/
/*
 * z_ceil.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ceil.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef ceil
double ceil();
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

static struct ftable Tceil = {
	"ceil",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	0,
};

double  Zceil(x)
double x;
{
	double ret_val;

	errno = 0;
	ret_val = ceil(x);
	evaluate( &Tceil, (pointer)&ret_val );
	return( ret_val );
}

