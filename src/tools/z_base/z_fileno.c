/*LINTLIBRARY*/
/*
 * z_fileno.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fileno.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fileno
int fileno();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
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

static struct ftable Tfileno = {
	"fileno",
	"a return value of at least 0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zfileno(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = fileno(stream);
	evaluate( &Tfileno, (pointer)&ret_val );
	return( ret_val );
}

