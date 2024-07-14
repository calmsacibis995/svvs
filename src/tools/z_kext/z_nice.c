/*LINTLIBRARY*/
/*
 * z_nice.c
 */

#ifdef	UNDEF
#ident	"@(#)z_nice.c	1.2"
#endif

#include <sv_env.h>

#ifndef nice
int nice();
#endif

static int success( val )
int *val;
{
	return( (-NZERO) <= (*val) && (*val) < NZERO );
}

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%d", *val );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}

int *sv_geti();

static struct ftable Tnice = {
	"nice",
	"a return value of at least (-NZERO) and less than NZERO",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	1,
};

int  Znice(incr)
int incr;
{
	int ret_val;

	errno = 0;
	ret_val = nice(incr);
	evaluate( &Tnice, (pointer)&ret_val );
	return( ret_val );
}

