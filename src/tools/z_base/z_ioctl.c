/*LINTLIBRARY*/
/*
 * z_ioctl.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ioctl.c	1.2"
#endif

#include <sv_env.h>

#ifndef ioctl
int ioctl();
#endif

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

static struct ftable Tioctl = {
	"ioctl",
	"indeterminate - based upon actual expected value",
	"a return value of (-1)",
	format,
	compare,
	VALUE,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zioctl(fildes, request, arg)
int fildes;
int request;
char *arg;
{
	int ret_val;

	errno = 0;
	ret_val = ioctl(fildes, request, arg);
	evaluate( &Tioctl, (pointer)&ret_val );
	return( ret_val );
}

