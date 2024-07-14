/*LINTLIBRARY*/
/*
 * z_pause.c
 */

#ifdef	UNDEF
#ident	"@(#)z_pause.c	1.2"
#endif

#include <sv_env.h>

#ifndef pause
int pause();
#endif

static int success( val )
int *val;
{
	return( (*val) != (-1) );
}

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
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

static struct ftable Tpause = {
	"pause",
	"a return value other than (-1)",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zpause(){
	int ret_val;

	errno = 0;
	ret_val = pause();
	evaluate( &Tpause, (pointer)&ret_val );
	return( ret_val );
}

