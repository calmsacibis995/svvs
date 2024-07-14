/*LINTLIBRARY*/
/*
 * z_mknod.c
 */

#ifdef	UNDEF
#ident	"@(#)z_mknod.c	1.3"
#endif

#include <sv_env.h>

#ifndef mknod
int mknod();
#endif

static int success( val )
int *val;
{
	return( (*val)? 0 : 1 );
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

static struct ftable Tmknod = {
	"mknod",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zmknod(path, mode, dev)
char *path;
int mode;
int dev;
{
	int ret_val;

	errno = 0;
	ret_val = mknod(path, sv_mode(mode), dev);
	evaluate( &Tmknod, (pointer)&ret_val );
	return( ret_val );
}

