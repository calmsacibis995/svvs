/*LINTLIBRARY*/
/*
 * z_slk_clea.c
 */

#ifdef	UNDEF
#ident	"@(#)z_slk_clea.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef slk_clear
int slk_clear();
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

static struct ftable Tslk_clear = {
	"slk_clear",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zslk_clear(){
	int ret_val;

	errno = 0;
	ret_val = slk_clear();
	evaluate( &Tslk_clear, (pointer)&ret_val );
	return( ret_val );
}

