/*LINTLIBRARY*/
/*
 * z_erand48.c
 */

#ifdef	UNDEF
#ident	"@(#)z_erand48.c	1.4"
#endif

#include <sv_env.h>

#ifndef erand48
double erand48();
#endif

static int success( val )
double *val;
{
	return( (*val) >= 0.0 && (*val) < 1.0 );
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

static struct ftable Terand48 = {
	"erand48",
	"a return value of at least 0.0 and less than 1.0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	0,
};

double  Zerand48(xsubi)
unsigned short xsubi[3];
{
	double ret_val;

	errno = 0;
	ret_val = erand48(xsubi);
	evaluate( &Terand48, (pointer)&ret_val );
	return( ret_val );
}

