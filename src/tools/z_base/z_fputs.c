/*LINTLIBRARY*/
/*
 * z_fputs.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fputs.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fputs
int fputs();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
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

static struct ftable Tfputs = {
	"fputs",
	"a return value of at least 0",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zfputs(s, stream)
char *s;
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fputs(s, stream);
	evaluate( &Tfputs, (pointer)&ret_val );
	return( ret_val );
}

