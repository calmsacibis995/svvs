/*LINTLIBRARY*/
/*
 * z_creat.c
 */

#ifdef	UNDEF
#ident	"@(#)z_creat.c	1.3"
#endif

#include <sv_env.h>

#ifndef creat
int creat();
#endif

static int success( val )
int *val;
{
	return( (*val) > 0 );
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

static struct ftable Tcreat = {
	"creat",
	"a return value greater than 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zcreat(path, mode)
char *path;
int mode;
{
	int ret_val;

	errno = 0;
	ret_val = creat(path, sv_mode(mode));
	evaluate( &Tcreat, (pointer)&ret_val );
	return( ret_val );
}

