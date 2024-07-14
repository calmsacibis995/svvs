/*LINTLIBRARY*/
/*
 * z_puts.c
 */

#ifdef	UNDEF
#ident	"@(#)z_puts.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef puts
int puts();
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

static struct ftable Tputs = {
	"puts",
	"a return value of at least 0",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zputs(s)
char *s;
{
	int ret_val;

	errno = 0;
	ret_val = puts(s);
	evaluate( &Tputs, (pointer)&ret_val );
	return( ret_val );
}

