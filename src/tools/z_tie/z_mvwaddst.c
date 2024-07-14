/*LINTLIBRARY*/
/*
 * z_mvwaddst.c
 */

#ifdef	UNDEF
#ident	"@(#)z_mvwaddst.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef mvwaddstr
int mvwaddstr();
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

static struct ftable Tmvwaddstr = {
	"mvwaddstr",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zmvwaddstr( win, y, x, str )
WINDOW *win;
int y;
int x;
char *str;
{
	int ret_val;

	errno = 0;
	ret_val = mvwaddstr( win, y, x, str );
	evaluate( &Tmvwaddstr, (pointer)&ret_val );
	return( ret_val );
}

