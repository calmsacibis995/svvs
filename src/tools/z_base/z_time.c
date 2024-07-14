/*LINTLIBRARY*/
/*
 * z_time.c
 */

#ifdef	UNDEF
#ident	"@(#)z_time.c	1.2"
#endif

#include <sv_env.h>

#ifndef time
long time();
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

static struct ftable Ttime = {
	"time",
	"a return value other than (-1)",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getl,
	0,
};

long  Ztime(tloc)
long *tloc;
{
	long ret_val;

	errno = 0;
	ret_val = time(tloc);
	evaluate( &Ttime, (pointer)&ret_val );
	return( ret_val );
}

