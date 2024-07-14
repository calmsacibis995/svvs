/*LINTLIBRARY*/
/*
 * z_read.c
 */

#ifdef	UNDEF
#ident	"@(#)z_read.c	1.2"
#endif

#include <sv_env.h>

#ifndef read
int read();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
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

static struct ftable Tread = {
	"read",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zread(fildes, buf, nbyte)
int fildes;
char *buf;
unsigned nbyte;
{
	int ret_val;

	errno = 0;
	ret_val = read(fildes, buf, nbyte);
	evaluate( &Tread, (pointer)&ret_val );
	return( ret_val );
}

