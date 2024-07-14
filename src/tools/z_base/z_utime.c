/*LINTLIBRARY*/
/*
 * z_utime.c
 */

#ifdef	UNDEF
#ident	"@(#)z_utime.c	1.2"
#endif

#include <sys/types.h>
#include <sv_env.h>

#ifndef utime
int utime();
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

static struct ftable Tutime = {
	"utime",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

struct utimbuf {
time_t actime;
time_t modtime;
};

int  Zutime(path, times)
char *path;
struct utimbuf *times;
{
	int ret_val;

	errno = 0;
	ret_val = utime(path, times);
	evaluate( &Tutime, (pointer)&ret_val );
	return( ret_val );
}

