/*LINTLIBRARY*/
/*
 * z_setvbuf.c
 */

#ifdef	UNDEF
#ident	"@(#)z_setvbuf.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef setvbuf
int setvbuf();
#endif

static int success( val )
int *val;
{
	return( (*val)? 0 : 1 );
}

static int failure( val )
int *val;
{
	return( (*val)? 1 : 0 );
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

static struct ftable Tsetvbuf = {
	"setvbuf",
	"a return value of 0",
	"a return value other than 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zsetvbuf(stream, buf, type, size)
FILE *stream;
char *buf;
int type;
int size;
{
	int ret_val;

	errno = 0;
	ret_val = setvbuf(stream, buf, type, size);
	evaluate( &Tsetvbuf, (pointer)&ret_val );
	return( ret_val );
}

