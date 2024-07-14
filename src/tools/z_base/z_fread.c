/*LINTLIBRARY*/
/*
 * z_fread.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fread.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fread
int fread();
#endif

static int success( val )
int *val;
{
	return( (*val) > 0 );
}

static int failure( val )
int *val;
{
	return( (*val)? 0 : 1 );
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

static struct ftable Tfread = {
	"fread",
	"a return value greater than 0",
	"a return value of 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zfread(ptr, size, nitems, stream)
char *ptr;
int size;
int nitems;
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fread(ptr, size, nitems, stream);
	evaluate( &Tfread, (pointer)&ret_val );
	return( ret_val );
}

