/*LINTLIBRARY*/
/*
 * z_t_rcvuda.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_rcvuda.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_rcvudata
int t_rcvudata();
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

static struct ftable Tt_rcvudata = {
	"t_rcvudata",
	"a return value of 0",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_rcvudata( fd, unitdata, flags )
int fd;
struct t_unitdata *unitdata;
int *flags;
{
	int ret_val;

	errno = 0;
	sv_look_fd = fd;
	ret_val = t_rcvudata( fd, unitdata, flags );
	evaluate( &Tt_rcvudata, (pointer)&ret_val );
	return( ret_val );
}

