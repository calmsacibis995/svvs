/*LINTLIBRARY*/
/*
 * z_ftell.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ftell.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef ftell
long ftell();
#endif

static int success( val )
long *val;
{
	return( (*val) >= 0 );
}

static char *format( val )
long *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%ld", *val );
	return( buf );
}

static int compare( a, b )
long *a;
long *b;
{
	return( *a == *b );
}

long *sv_getl();

static struct ftable Tftell = {
	"ftell",
	"a return value of at least 0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getl,
	0,
};

long  Zftell(stream)
FILE *stream;
{
	long ret_val;

	errno = 0;
	ret_val = ftell(stream);
	evaluate( &Tftell, (pointer)&ret_val );
	return( ret_val );
}

