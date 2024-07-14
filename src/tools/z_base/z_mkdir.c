#ifndef	SYSVR_2  /*  mkdir was added in SVR3  */

/*LINTLIBRARY*/
/*
 * sv_mkdir.c
 */

#ident	"@(#)z_mkdir.c	1.1"

#include <sv_env.h>

#ifndef mkdir
int mkdir();
#endif

static int success( val )
int *val;
{
	return( (*val) == 0 );
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

static struct ftable Tmkdir = {
	"mkdir",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zmkdir(path, mode)
char *path;
int mode;
{
	int ret_val;

	errno = 0;
	ret_val = mkdir(path, mode);
	evaluate( &Tmkdir, (pointer)&ret_val );
	return( ret_val );
}

#endif
