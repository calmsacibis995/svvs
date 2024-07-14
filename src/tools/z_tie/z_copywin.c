/*LINTLIBRARY*/
/*
 * z_copywin.c
 */

#ifdef	UNDEF
#ident	"@(#)z_copywin.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef copywin
int copywin();
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

static struct ftable Tcopywin = {
	"copywin",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zcopywin( srcwin, dstwin, sminrow, smincol, dminrow, dmincol, dmaxrow, dmaxcol, overlay )
WINDOW *srcwin;
WINDOW *dstwin;
int sminrow;
int smincol;
int dminrow;
int dmincol;
int dmaxrow;
int dmaxcol;
int overlay;
{
	int ret_val;

	errno = 0;
	ret_val = copywin( srcwin, dstwin, sminrow, smincol, dminrow, dmincol, dmaxrow, dmaxcol, overlay );
	evaluate( &Tcopywin, (pointer)&ret_val );
	return( ret_val );
}

