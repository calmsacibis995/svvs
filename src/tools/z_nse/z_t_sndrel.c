/*LINTLIBRARY*/
/*
 * z_t_sndrel.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_sndrel.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_sndrel
int t_sndrel();
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

static struct ftable Tt_sndrel = {
	"t_sndrel",
	"a return value of 0",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_sndrel( fd )
int fd;
{
	int ret_val;

	errno = 0;
	sv_look_fd = fd;
	ret_val = t_sndrel( fd );
	evaluate( &Tt_sndrel, (pointer)&ret_val );
	return( ret_val );
}

