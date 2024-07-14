/*LINTLIBRARY*/
/*
 * z_winch.c
 */

#ifdef	UNDEF
#ident	"@(#)z_winch.c	1.3"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef winch
chtype winch();
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

static struct ftable Twinch = {
	"winch",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

chtype  Zwinch( win )
WINDOW *win;
{
	chtype ret_val;

	errno = 0;
	ret_val = winch( win );
	evaluate( &Twinch, (pointer)&ret_val );
	return( ret_val );
}

