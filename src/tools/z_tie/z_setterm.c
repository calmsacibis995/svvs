/*LINTLIBRARY*/
/*
 * z_setterm.c
 */

#ifdef	UNDEF
#ident	"@(#)z_setterm.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef setterm
int setterm();
#endif

static int success( val )
int *val;
{
	return( (*val) == OK );
}

static int failure( val )
int *val;
{
	return( (*val) == ERR );
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

static struct ftable Tsetterm = {
	"setterm",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zsetterm( term )
char *term;
{
	int ret_val;

	errno = 0;
	ret_val = setterm( term );
	evaluate( &Tsetterm, (pointer)&ret_val );
	return( ret_val );
}

