/*LINTLIBRARY*/
/*
 * z_open.c
 */

#ifdef	UNDEF
#ident	"@(#)z_open.c	1.3"
#endif

#include <fcntl.h>
#include <sv_env.h>

#ifndef open
int open();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
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

static struct ftable Topen = {
	"open",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zopen(path, oflag ,mode)
char *path;
int oflag;
int mode;
{
	int ret_val;

	errno = 0;
	ret_val = open(path, oflag, sv_mode(mode));
	evaluate( &Topen, (pointer)&ret_val );
	return( ret_val );
}

