/*LINTLIBRARY*/
/*
 * z_atof.c
 */

#ifdef	UNDEF
#ident	"@(#)z_atof.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef atof
double atof();
#endif

static int success( val )
double *val;
{
	return( (*val) != HUGE && (*val) != -HUGE );
}

static int failure( val )
double *val;
{
	return( (*val) == HUGE || (*val) == -HUGE );
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

static struct ftable Tatof = {
	"atof",
	"a return value other than HUGE or -HUGE",
	"a return value of HUGE or -HUGE",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getd,
	1,
};

double  Zatof(str)
char *str;
{
	double ret_val;

	errno = 0;
	ret_val = atof(str);
	evaluate( &Tatof, (pointer)&ret_val );
	return( ret_val );
}

