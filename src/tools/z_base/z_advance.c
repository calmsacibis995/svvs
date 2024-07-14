/*LINTLIBRARY*/
/*
 * z_advance.c
 */

#ifdef	UNDEF
#ident	"@(#)z_advance.c	1.2"
#endif

#include <sv_env.h>

#ifndef advance
int advance();
#endif

static int success( val )
int *val;
{
	return( (*val)? 1 : 0 );
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

static struct ftable Tadvance = {
	"advance",
	"a return value other than 0",
	"a return value of 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zadvance(string, buf)
char *string;
char *buf;
{
	int ret_val;

	errno = 0;
	ret_val = advance(string, buf);
	evaluate( &Tadvance, (pointer)&ret_val );
	return( ret_val );
}

