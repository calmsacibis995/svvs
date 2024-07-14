/*LINTLIBRARY*/
/*
 * z_reset_sh.c
 */

#ifdef	UNDEF
#ident	"@(#)z_reset_sh.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef reset_shell_mode
int reset_shell_mode();
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

static struct ftable Treset_shell_mode = {
	"reset_shell_mode",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zreset_shell_mode(){
	int ret_val;

	errno = 0;
	ret_val = reset_shell_mode();
	evaluate( &Treset_shell_mode, (pointer)&ret_val );
	return( ret_val );
}

