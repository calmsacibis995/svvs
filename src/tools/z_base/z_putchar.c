/*LINTLIBRARY*/
/*
 * z_putchar.c
 */

#ifdef	UNDEF
#ident	"@(#)z_putchar.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef putchar
int putchar();
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

static struct ftable Tputchar = {
	"putchar",
	"a return value of at least 0 and at most 255",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zputchar(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = putchar(c);
	evaluate( &Tputchar, (pointer)&ret_val );
	return( ret_val );
}

