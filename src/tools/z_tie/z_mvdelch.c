/*LINTLIBRARY*/
/*
 * z_mvdelch.c
 */

#ifdef	UNDEF
#ident	"@(#)z_mvdelch.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef mvdelch
int mvdelch();
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

static struct ftable Tmvdelch = {
	"mvdelch",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zmvdelch( y, x )
int y;
int x;
{
	int ret_val;

	errno = 0;
	ret_val = mvdelch( y, x );
	evaluate( &Tmvdelch, (pointer)&ret_val );
	return( ret_val );
}
