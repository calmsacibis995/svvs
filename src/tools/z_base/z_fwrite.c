/*LINTLIBRARY*/
/*
 * z_fwrite.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fwrite.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fwrite
int fwrite();
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

static struct ftable Tfwrite = {
	"fwrite",
	"a return value greater than 0",
	"a return value of 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zfwrite(ptr, size, nitems, stream)
char *ptr;
int size;
int nitems;
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fwrite(ptr, size, nitems, stream);
	evaluate( &Tfwrite, (pointer)&ret_val );
	return( ret_val );
}

