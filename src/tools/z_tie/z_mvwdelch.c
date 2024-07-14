/*LINTLIBRARY*/
/*
 * z_mvwdelch.c
 */

#ifdef	UNDEF
#ident	"@(#)z_mvwdelch.c	1.3"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef mvwdelch
int mvwdelch();
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

static struct ftable Tmvwdelch = {
	"mvwdelch",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zmvwdelch( win, y, x )
WINDOW *win;
int y;
int x;
{
	int ret_val;

	errno = 0;
	ret_val = mvwdelch( win, y, x );
	evaluate( &Tmvwdelch, (pointer)&ret_val );
	return( ret_val );
}

