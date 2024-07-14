/*LINTLIBRARY*/
/*
 * z_t_rcvcon.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_rcvcon.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_rcvconnect
int t_rcvconnect();
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

static struct ftable Tt_rcvconnect = {
	"t_rcvconnect",
	"a return value of 0",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_rcvconnect( fd, call )
int fd;
struct t_call *call;
{
	int ret_val;

	errno = 0;
	sv_look_fd = fd;
	ret_val = t_rcvconnect( fd, call );
	evaluate( &Tt_rcvconnect, (pointer)&ret_val );
	return( ret_val );
}

