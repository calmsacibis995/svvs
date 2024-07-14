/*LINTLIBRARY*/
/*
 * z_strlen.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strlen.c	1.2"
#endif

#include <string.h>
#include <sv_env.h>

#ifndef strlen
int strlen();
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

static struct ftable Tstrlen = {
	"strlen",
	"a return value of at least 0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zstrlen(s)
char *s;
{
	int ret_val;

	errno = 0;
	ret_val = strlen(s);
	evaluate( &Tstrlen, (pointer)&ret_val );
	return( ret_val );
}

