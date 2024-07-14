/*LINTLIBRARY*/
/*
 * z_slk_set.c
 */

#ifdef	UNDEF
#ident	"@(#)z_slk_set.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef slk_set
int slk_set();
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

static struct ftable Tslk_set = {
	"slk_set",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zslk_set( labnum, label, fmt )
int labnum;
char *label;
int fmt;
{
	int ret_val;

	errno = 0;
	ret_val = slk_set( labnum, label, fmt );
	evaluate( &Tslk_set, (pointer)&ret_val );
	return( ret_val );
}

