/*LINTLIBRARY*/
/*
 * z_scr_rest.c
 */

#ifdef	UNDEF
#ident	"@(#)z_scr_rest.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef scr_restore
int scr_restore();
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

static struct ftable Tscr_restore = {
	"scr_restore",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zscr_restore( filename )
char *filename;
{
	int ret_val;

	errno = 0;
	ret_val = scr_restore( filename );
	evaluate( &Tscr_restore, (pointer)&ret_val );
	return( ret_val );
}

