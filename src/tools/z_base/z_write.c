/*LINTLIBRARY*/
/*
 * z_write.c
 */

#ifdef	UNDEF
#ident	"@(#)z_write.c	1.2"
#endif

#include <sv_env.h>

#ifndef write
int write();
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

static struct ftable Twrite = {
	"write",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zwrite(fildes, buf, nbyte)
int fildes;
char *buf;
unsigned nbyte;
{
	int ret_val;

	errno = 0;
	ret_val = write(fildes, buf, nbyte);
	evaluate( &Twrite, (pointer)&ret_val );
	return( ret_val );
}

