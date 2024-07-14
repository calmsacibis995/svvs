/*LINTLIBRARY*/
/*
 * z_strtod.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strtod.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef strtod
double strtod();
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

static struct ftable Tstrtod = {
	"strtod",
	"a return value other than HUGE or -HUGE",
	"a return value of HUGE or -HUGE",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getd,
	1,
};

double  Zstrtod(str, ptr)
char *str;
char **ptr;
{
	double ret_val;

	errno = 0;
	ret_val = strtod(str, ptr);
	evaluate( &Tstrtod, (pointer)&ret_val );
	return( ret_val );
}

