/*LINTLIBRARY*/
/*
 * z_pnoutref.c
 */

#ifdef	UNDEF
#ident	"@(#)z_pnoutref.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef pnoutrefresh
int pnoutrefresh();
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

static struct ftable Tpnoutrefresh = {
	"pnoutrefresh",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zpnoutrefresh( pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol )
WINDOW *pad;
int pminrow;
int pmincol;
int sminrow;
int smincol;
int smaxrow;
int smaxcol;
{
	int ret_val;

	errno = 0;
	ret_val = pnoutrefresh( pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol );
	evaluate( &Tpnoutrefresh, (pointer)&ret_val );
	return( ret_val );
}

