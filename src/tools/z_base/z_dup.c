/*LINTLIBRARY*/
/*
 * z_dup.c
 */

#ifdef	UNDEF
#ident	"@(#)z_dup.c	1.2"
#endif

#include <sv_env.h>

#ifndef dup
int dup();
#endif

static int success( val )
int *val;
{
	return( (*val) > 0 );
}

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
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

static struct ftable Tdup = {
	"dup",
	"a return value greater than 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zdup(fildes)
int fildes;
{
	int ret_val;

	errno = 0;
	ret_val = dup(fildes);
	evaluate( &Tdup, (pointer)&ret_val );
	return( ret_val );
}

