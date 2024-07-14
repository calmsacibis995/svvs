/*LINTLIBRARY*/
/*
 * z_lseek.c
 */

#ifdef	UNDEF
#ident	"@(#)z_lseek.c	1.2"
#endif

#include <sv_env.h>

#ifndef lseek
long lseek();
#endif

static int success( val )
long *val;
{
	return( (*val) >= 0 );
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

static struct ftable Tlseek = {
	"lseek",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getl,
	1,
};

long  Zlseek(fildes, offset, whence)
int fildes;
long offset;
int whence;
{
	long ret_val;

	errno = 0;
	ret_val = lseek(fildes, offset, whence);
	evaluate( &Tlseek, (pointer)&ret_val );
	return( ret_val );
}

