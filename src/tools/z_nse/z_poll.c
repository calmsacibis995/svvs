/*LINTLIBRARY*/
/*
 * z_poll.c
 */

#ifdef	UNDEF
#ident	"@(#)z_poll.c	1.2"
#endif

#include <poll.h>
#include <sv_env.h>

#ifndef poll
int poll();
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

static struct ftable Tpoll = {
	"poll",
	"a return greater or equal to zero",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zpoll(fds, nfds, timeout)
struct pollfd *fds[];
unsigned long nfds;
long timeout;
{
	int ret_val;

	errno = 0;
	ret_val = poll(fds, nfds, timeout);
	evaluate( &Tpoll, (pointer)&ret_val );
	return( ret_val );
}

