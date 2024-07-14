/*LINTLIBRARY*/
/*
 * z_jn.c
 */

#ifdef	UNDEF
#ident	"@(#)z_jn.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef jn
double jn();
#endif

static int success( val )
double *val;
{
	return( (*val) <= 1.0 && (*val) > -0.581876 );
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

static struct ftable Tjn = {
	"jn",
	"a return value of at most 1.0 and greater than -0.581876",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getd,
	1,
};

double  Zjn(n, x)
int n;
double x;
{
	double ret_val;

	errno = 0;
	ret_val = jn(n, x);
	evaluate( &Tjn, (pointer)&ret_val );
	return( ret_val );
}

