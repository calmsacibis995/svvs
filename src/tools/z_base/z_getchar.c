/*LINTLIBRARY*/
/*
 * z_getchar.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getchar.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef getchar
int getchar();
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

static struct ftable Tgetchar = {
	"getchar",
	"a return value other than EOF",
	"a return value of EOF",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zgetchar(){
	int ret_val;

	errno = 0;
	ret_val = getchar();
	evaluate( &Tgetchar, (pointer)&ret_val );
	return( ret_val );
}

