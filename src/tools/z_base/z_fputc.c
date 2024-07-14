/*LINTLIBRARY*/
/*
 * z_fputc.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fputc.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fputc
int fputc();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 && (*val) <= 255 );
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

static struct ftable Tfputc = {
	"fputc",
	"a return value of at least 0 and at most 255",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zfputc(c, stream)
int c;
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fputc(c, stream);
	evaluate( &Tfputc, (pointer)&ret_val );
	return( ret_val );
}

