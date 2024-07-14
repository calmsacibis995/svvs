/*LINTLIBRARY*/
/*
 * z_vprintf.c
 */

#ifdef	UNDEF
#ident	"@(#)z_vprintf.c	1.2"
#endif

#include <stdio.h>
#include <varargs.h>
#include <sv_env.h>

#ifndef vprintf
int vprintf();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
}

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
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

static struct ftable Tvprintf = {
	"vprintf",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zvprintf(fmt, ap)
char *fmt;
va_list ap;
{
	int ret_val;

	errno = 0;
	ret_val = vprintf(fmt, ap);
	evaluate( &Tvprintf, (pointer)&ret_val );
	return( ret_val );
}

