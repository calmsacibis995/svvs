/*LINTLIBRARY*/
/*
 * z_t_accept.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_accept.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_accept
int t_accept();
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

static struct ftable Tt_accept = {
	"t_accept",
	"a return value of 0",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_accept( fd, resfd, call )
int fd;
int resfd;
struct t_call *call;
{
	int ret_val;

	errno = 0;
	ret_val = t_accept( fd, resfd, call );
	sv_look_fd = fd;
	evaluate( &Tt_accept, (pointer)&ret_val );
	return( ret_val );
}

