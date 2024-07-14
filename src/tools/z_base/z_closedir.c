#ifndef	SYSVR_2  /*  directory was added in SVR3  */

/*LINTLIBRARY*/
/*
 * sv_closedir.c
 */

#ident	"@(#)z_closedir.c	1.1"

#include <sys/types.h>
#include <dirent.h>

#include <fcntl.h>
#include <sv_env.h>

#ifndef closedir
int closedir();
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

static struct ftable Tclosedir = {
	"closedir",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zclosedir(fildes)
DIR	*fildes;
{
	int ret_val;

	errno = 0;
	ret_val = closedir(fildes);
	evaluate( &Tclosedir, (pointer)&ret_val );
	return( ret_val );
}

#endif
