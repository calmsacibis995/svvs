/*LINTLIBRARY*/
/*
 * z_fgetc.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fgetc.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fgetc
int fgetc();
#endif

static int success( val )
int *val;
{
	return( (*val) != EOF );
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

static struct ftable Tfgetc = {
	"fgetc",
	"a return value other than EOF",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zfgetc(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fgetc(stream);
	evaluate( &Tfgetc, (pointer)&ret_val );
	return( ret_val );
}

