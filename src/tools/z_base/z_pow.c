/*LINTLIBRARY*/
/*
 * z_pow.c
 */

#ifdef	UNDEF
#ident	"@(#)z_pow.c	1.4"
#endif

#include <math.h>
#include <sv_env.h>

#ifndef pow
double pow();
#endif

static int success0( val )
double *val;
{
	return( (*val) != HUGE && (*val) != -HUGE );
}

static int success1( val )
double *val;
{
	return( (*val) != HUGE && (*val) != -HUGE && (*val) != 0.0 );
}

static int failure0( val )
double *val;
{
	return( (*val) == HUGE || (*val) == -HUGE );
}

static int failure1( val )
double *val;
{
	return( (*val) == HUGE || (*val) == -HUGE || (*val) == 0.0 );
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

static struct ftable Tpow0 = {
	"pow",
	"a return value other than HUGE, -HUGE",
	"a return value of HUGE, -HUGE",
	format,
	compare,
	success0,
	failure0,
	(pfunctptr)sv_getd,
	1,
};
static struct ftable Tpow1 = {
	"pow",
	"a return value other than HUGE, -HUGE, or 0.0",
	"a return value of HUGE, -HUGE or 0.0",
	format,
	compare,
	success1,
	failure1,
	(pfunctptr)sv_getd,
	1,
};

double  Zpow(x, y)
double x;
double y;
{
	double ret_val;

	errno = 0;
	ret_val = pow(x, y);
	if( !x && y>0 )
		evaluate( &Tpow0, (pointer)&ret_val );
	else
		evaluate( &Tpow1, (pointer)&ret_val );
	return( ret_val );
}

