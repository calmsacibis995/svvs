/*LINTLIBRARY*/
/*
 * z_resetty.c
 */

#ifdef	UNDEF
#ident	"@(#)z_resetty.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef resetty
int resetty();
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

static struct ftable Tresetty = {
	"resetty",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zresetty(){
	int ret_val;

	errno = 0;
	ret_val = resetty();
	evaluate( &Tresetty, (pointer)&ret_val );
	return( ret_val );
}

