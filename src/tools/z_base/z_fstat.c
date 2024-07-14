/*LINTLIBRARY*/
/*
 * z_fstat.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fstat.c	1.2"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sv_env.h>

#ifndef fstat
int fstat();
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

static struct ftable Tfstat = {
	"fstat",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zfstat(fildes, buf)
int fildes;
struct stat *buf;
{
	int ret_val;

	errno = 0;
	ret_val = fstat(fildes, buf);
	evaluate( &Tfstat, (pointer)&ret_val );
	return( ret_val );
}

