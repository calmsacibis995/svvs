#ifndef	SYSVR_2  /*  rmdir was added in SVR3  */

/*LINTLIBRARY*/
/*
 * sv_rmdir.c
 */

#ident	"@(#)z_rmdir.c	1.1"

#include <sv_env.h>

#ifndef rmdir
int rmdir();
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

static struct ftable Trmdir = {
	"rmdir",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zrmdir(path)
char *path;
{
	int ret_val;

	errno = 0;
	ret_val = rmdir(path);
	evaluate( &Trmdir, (pointer)&ret_val );
	return( ret_val );
}

#endif
