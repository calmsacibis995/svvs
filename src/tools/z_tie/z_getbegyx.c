/*LINTLIBRARY*/
/*
 * z_getbegyx.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getbegyx.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef getbegyx
int getbegyx();
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

static struct ftable Tgetbegyx = {
	"getbegyx",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zgetbegyx( win, y, x )
WINDOW *win;
int *y;
int *x;
{
	int ret_val;

	errno = 0;
	ret_val = getbegyx( win, (*y), (*x) );
	evaluate( &Tgetbegyx, (pointer)&ret_val );
	return( ret_val );
}

