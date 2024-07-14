/*LINTLIBRARY*/
/*
 * z_reset_pr.c
 */

#ifdef	UNDEF
#ident	"@(#)z_reset_pr.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef reset_prog_mode
int reset_prog_mode();
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

static struct ftable Treset_prog_mode = {
	"reset_prog_mode",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zreset_prog_mode(){
	int ret_val;

	errno = 0;
	ret_val = reset_prog_mode();
	evaluate( &Treset_prog_mode, (pointer)&ret_val );
	return( ret_val );
}

