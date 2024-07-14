/*LINTLIBRARY*/
/*
 * z_t_sync.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_sync.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_sync
int t_sync();
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

static struct ftable Tt_sync = {
	"t_sync",
	"indeterminate - based upon actual expected value",
	"a return value of -1",
	format,
	compare,
	VALUE,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_sync( fd )
int fd;
{
	int ret_val;

	errno = 0;
	sv_look_fd = fd;
	ret_val = t_sync( fd );
	evaluate( &Tt_sync, (pointer)&ret_val );
	return( ret_val );
}

