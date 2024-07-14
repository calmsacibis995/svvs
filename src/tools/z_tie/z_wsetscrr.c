/*LINTLIBRARY*/
/*
 * z_wsetscrr.c
 */

#ifdef	UNDEF
#ident	"@(#)z_wsetscrr.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef wsetscrreg
int wsetscrreg();
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

static struct ftable Twsetscrreg = {
	"wsetscrreg",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zwsetscrreg( win, top, bot )
WINDOW *win;
int top;
int bot;
{
	int ret_val;

	errno = 0;
	ret_val = wsetscrreg( win, top, bot );
	evaluate( &Twsetscrreg, (pointer)&ret_val );
	return( ret_val );
}

