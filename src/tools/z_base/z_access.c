/*LINTLIBRARY*/
/*
 * sv_access.c
 */

#ident	"@(#)z_access.c	1.2"

#include <sv_env.h>

#ifndef access
int access();
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

static struct ftable Taccess = {
	"access",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zaccess(path, amode)
char *path;
int amode;
{
	int ret_val;

	errno = 0;
	ret_val = access(path, sv_mode(amode));
	evaluate( &Taccess, (pointer)&ret_val );
	return( ret_val );
}

