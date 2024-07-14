/*LINTLIBRARY*/
/*
 * z_putmsg.c
 */

#ifdef	UNDEF
#ident	"@(#)z_putmsg.c	1.2"
#endif

#include <stropts.h>
#include <sv_env.h>

#ifndef putmsg
int putmsg();
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

static struct ftable Tputmsg = {
	"putmsg",
	"a return of zero",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zputmsg(fd, ctlptr, dataptr, flags)
int fd;
struct strbuf *ctlptr;
struct strbuf *dataptr;
int flags;
{
	int ret_val;

	errno = 0;
	ret_val = putmsg(fd, ctlptr, dataptr, flags);
	evaluate( &Tputmsg, (pointer)&ret_val );
	return( ret_val );
}

