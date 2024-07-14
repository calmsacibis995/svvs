/*LINTLIBRARY*/
/*
 * z_fseek.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fseek.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fseek
int fseek();
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

static struct ftable Tfseek = {
	"fseek",
	"a return value of 0",
	"a return value other than 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zfseek(stream, offset, ptrname)
FILE *stream;
long offset;
int ptrname;
{
	int ret_val;

	errno = 0;
	ret_val = fseek(stream, offset, ptrname);
	evaluate( &Tfseek, (pointer)&ret_val );
	return( ret_val );
}

