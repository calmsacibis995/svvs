/*LINTLIBRARY*/
/*
 * z_fflush.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fflush.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fflush
int fflush();
#endif

static int success( val )
int *val;
{
	return( (*val)? 0 : 1 );
}

static int failure( val )
int *val;
{
	return( (*val) == EOF );
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

static struct ftable Tfflush = {
	"fflush",
	"a return value of 0",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zfflush(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fflush(stream);
	evaluate( &Tfflush, (pointer)&ret_val );
	return( ret_val );
}

