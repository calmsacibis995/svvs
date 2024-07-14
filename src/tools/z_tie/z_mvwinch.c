/*LINTLIBRARY*/
/*
 * z_mvwinch.c
 */

#ifdef	UNDEF
#ident	"@(#)z_mvwinch.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef mvwinch
int mvwinch();
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

static struct ftable Tmvwinch = {
	"mvwinch",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zmvwinch( win, y, x )
WINDOW *win;
int y;
int x;
{
	int ret_val;

	errno = 0;
	ret_val = mvwinch( win, y, x );
	evaluate( &Tmvwinch, (pointer)&ret_val );
	return( ret_val );
}

