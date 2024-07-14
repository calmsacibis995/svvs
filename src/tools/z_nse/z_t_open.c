/*LINTLIBRARY*/
/*
 * z_t_open.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_open.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_open
int t_open();
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

static struct ftable Tt_open = {
	"t_open",
	"a return value greater than or equal to zero",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_open(path,oflag,info)
char *path;
int oflag;
struct t_info *info;
{
	int ret_val;

	errno = 0;
	ret_val = t_open(path,oflag,info);
	evaluate( &Tt_open, (pointer)&ret_val );
	return( ret_val );
}

