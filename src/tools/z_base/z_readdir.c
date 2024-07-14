#ifndef	SYSVR_2  /*  directory was added in SVR3  */

/*LINTLIBRARY*/
/*
 * sv_readdir.c
 */

#ident	"@(#)z_readdir.c	1.1"

#include	<sys/types.h>
#include	<dirent.h>

#include <stdio.h>
#include <sv_env.h>


static int success( val )
struct dirent **val;
{
	return( (*val) != (struct dirent *)NULL );
}

static int failure( val )
struct dirent **val;
{
	return( (*val) == (struct dirent *)NULL );
}


static char *format( val )
char **val;
{
	static char buf[STRVAL];

	if( !*val )
		(void) strcpy( buf, "NULL" );
	else if( (pointer)(*val) == NON_NULL )
		(void) strcpy( buf, "NON-NULL" );
	else
		(void) sprintf( buf, "0x%lx", (long)(*val) );
	return( buf );
}


static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}

int *sv_geti();

static struct ftable Treaddir = {
	"readdir",
	"a non-NULL return value",
	"a NULL return value",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

struct dirent *Zreaddir(ptr, size, nitems, stream)
char *ptr;
int size;
int nitems;
FILE *stream;
{
	struct dirent *ret_val;

	errno = 0;
	ret_val = readdir(ptr, size, nitems, stream);
	evaluate( &Treaddir, (pointer)&ret_val );
	return( ret_val );
}

#endif
