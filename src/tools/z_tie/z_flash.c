/*LINTLIBRARY*/
/*
 * z_flash.c
 */

#ifdef	UNDEF
#ident	"@(#)z_flash.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef flash
int flash();
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

static struct ftable Tflash = {
	"flash",
	"a return value of OK",
	"a return value of ERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zflash(){
	int ret_val;

	errno = 0;
	ret_val = flash();
	evaluate( &Tflash, (pointer)&ret_val );
	return( ret_val );
}
