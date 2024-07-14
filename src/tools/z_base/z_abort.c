/*LINTLIBRARY*/
/*
 * z_abort.c
 */

#ifdef	UNDEF
#ident	"@(#)z_abort.c	1.2"
#endif

#include <sv_env.h>

#ifndef abort
int abort();
#endif

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

static struct ftable Tabort = {
	"abort",
	"not defined",
	"not defined",
	format,
	compare,
	(ifunctptr)0,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zabort(){
	int ret_val;

	errno = 0;
	ret_val = abort();
	evaluate( &Tabort, (pointer)&ret_val );
	return( ret_val );
}

