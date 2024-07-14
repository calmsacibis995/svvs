/*LINTLIBRARY*/
/*
 * z_putw.c
 */

#ifdef	UNDEF
#ident	"@(#)z_putw.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef putw
int putw();
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

static struct ftable Tputw = {
	"putw",
	"a return value of 0",
	"a return value other than 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zputw(w, stream)
int w;
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = putw(w, stream);
	evaluate( &Tputw, (pointer)&ret_val );
	return( ret_val );
}

