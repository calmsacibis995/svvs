/*LINTLIBRARY*/
/*
 * z_clrtobot.c
 */

#ifdef	UNDEF
#ident	"@(#)z_clrtobot.c	1.1"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef clrtobot
int clrtobot();
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

static struct ftable Tclrtobot = {
	"clrtobot",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zclrtobot(){
	int ret_val;

	errno = 0;
	ret_val = clrtobot();
	evaluate( &Tclrtobot, (pointer)&ret_val );
	return( ret_val );
}

