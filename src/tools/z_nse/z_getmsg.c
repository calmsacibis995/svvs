/*LINTLIBRARY*/
/*
 * z_getmsg.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getmsg.c	1.2"
#endif

#include <stropts.h>
#include <sv_env.h>

#ifndef getmsg
int getmsg();
#endif

static int success( val )
int *val;
{
	return( !(*val) || (*val) == MORECTL || (*val) == MOREDATA || (*val) ==  ( MORECTL | MOREDATA ) );
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

static struct ftable Tgetmsg = {
	"getmsg",
	"a return of zero or MORECTL or MOREDATA or ( MORECTL | MOREDATA )",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zgetmsg(fd, ctlptr, dataptr, flags)
int fd;
struct strbuf *ctlptr;
struct strbuf *dataptr;
int flags;
{
	int ret_val;

	errno = 0;
	ret_val = getmsg(fd, ctlptr, dataptr, flags);
	evaluate( &Tgetmsg, (pointer)&ret_val );
	return( ret_val );
}

