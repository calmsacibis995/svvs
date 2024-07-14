/*LINTLIBRARY*/
/*
 * z_t_rcv.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_rcv.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_rcv
int t_rcv();
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

static struct ftable Tt_rcv = {
	"t_rcv",
	"a return value of at least 0",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_rcv( fd, buf, nbytes, flags )
int fd;
char *buf;
unsigned nbytes;
int *flags;
{
	int ret_val;

	errno = 0;
	sv_look_fd = fd;
	ret_val = t_rcv( fd, buf, nbytes, flags );
	evaluate( &Tt_rcv, (pointer)&ret_val );
	return( ret_val );
}

