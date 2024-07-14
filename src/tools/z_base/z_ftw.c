/*LINTLIBRARY*/
/*
 * z_ftw.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ftw.c	1.2"
#endif

#include <ftw.h>
#include <sv_env.h>

#ifndef ftw
int ftw();
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

static struct ftable Tftw = {
	"ftw",
	"a return value other than (-1)",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zftw(path, fn, depth)
char *path;
int (*fn)();
int depth;
{
	int ret_val;

	errno = 0;
	ret_val = ftw(path, fn, depth);
	evaluate( &Tftw, (pointer)&ret_val );
	return( ret_val );
}

