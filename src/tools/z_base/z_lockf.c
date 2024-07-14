/*LINTLIBRARY*/
/*
 * z_lockf.c
 */

#ifdef	UNDEF
#ident	"@(#)z_lockf.c	1.3"
#endif

#ifdef	LOCKF
#include <unistd.h>
#endif

#include <sv_env.h>

#ifndef lockf
int lockf();
#endif

static int success( val )
int *val;
{
	return( (*val)? 0 : 1 );
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

static struct ftable Tlockf = {
	"lockf",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zlockf(fildes, function, size)
long size;
int fildes;
int function;
{
	int ret_val;

	errno = 0;
	ret_val = lockf(fildes, function, size);
	evaluate( &Tlockf, (pointer)&ret_val );
	return( ret_val );
}

