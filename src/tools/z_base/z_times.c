/*LINTLIBRARY*/
/*
 * z_times.c
 */

#ifdef	UNDEF
#ident	"@(#)z_times.c	1.2"
#endif

#include <sys/types.h>
#include <sys/times.h>
#include <sv_env.h>

#ifndef times
long times();
#endif

static int success( val )
long *val;
{
	return( (*val) != (-1) );
}

static int failure( val )
long *val;
{
	return( (*val) == (-1) );
}

static char *format( val )
long *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%ld", *val );
	return( buf );
}

static int compare( a, b )
long *a;
long *b;
{
	return( *a == *b );
}

long *sv_getl();

static struct ftable Ttimes = {
	"times",
	"a return value other than (-1)",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getl,
	0,
};

long  Ztimes(buffer)
struct tms *buffer;
{
	long ret_val;

	errno = 0;
	ret_val = times(buffer);
	evaluate( &Ttimes, (pointer)&ret_val );
	return( ret_val );
}

