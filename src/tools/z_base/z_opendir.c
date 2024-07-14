#ifndef	SYSVR_2  /*  directory was added in SVR3  */

/*LINTLIBRARY*/
/*
 * sv_opendir.c
 */

#ident	"@(#)z_opendir.c	1.1"

#include <sys/types.h>
#include <dirent.h>

#include <fcntl.h>
#include <stdio.h>
#include <sv_env.h>


static int success( val )
DIR **val;
{
	return( (*val) != ((DIR *) NULL) );
}

static int failure( val )
DIR **val;
{
	return( (*val) == ((DIR *) NULL) );
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

static struct ftable Topendir = {
	"open",
	"a return value of non-null",
	"a return value of null",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

DIR	*Zopendir(path)
char *path;
{
	DIR	*ret_val;

	errno = 0;
	ret_val = opendir(path);
	evaluate( &Topendir, (pointer)&ret_val );
	return( ret_val );
}

#endif
