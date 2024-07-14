/*LINTLIBRARY*/
/*
 * z_fcntl.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fcntl.c	1.5"
#endif

#include <fcntl.h>
#include <svvs.h>
#include <sv_env.h>

#ifndef fcntl
int fcntl();
#endif

static int success0( val )
int *val;
{
	return( (*val) >= 0 );
}
static int success3( val )
int *val;
{
	return( (*val) != (-1) );
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

static char *format1( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%d", (*val) & 1 );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}
static int compare1( a, b )
int *a;
int *b;
{
	return( ((*a) & 1) == ((*b) & 1) );
}

int *sv_geti();

static struct ftable Tfcntl0 = {
	"fcntl",
	"a return value greater than or equal to 0",
	"a return value of (-1)",
	format,
	compare,
	success0,
	failure,
	(pfunctptr)sv_geti,
	1,
};
static struct ftable Tfcntl1 = {
	"fcntl",
	"a return value greater than or equal to 0",
	"a return value of (-1)",
	format1,
	compare1,
	success0,
	failure,
	(pfunctptr)sv_geti,
	1,
};
static struct ftable Tfcntl3 = {
	"fcntl",
	"a return value other than (-1)",
	"a return value of (-1)",
	format,
	compare,
	success3,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zfcntl(fildes, cmd, arg)
int fildes;
int cmd;
int *arg;
{
	int ret_val;

	errno = 0;
	ret_val = fcntl(fildes, cmd, arg);
	switch( cmd ) {

	case F_GETFL:
	case F_DUPFD:
		evaluate( &Tfcntl0, (pointer)&ret_val );
		break;
	case F_GETFD:
		evaluate( &Tfcntl1, (pointer)&ret_val );
		break;

	case F_SETFD:
	case F_SETFL:
#ifdef LOCKF
	case F_GETLK:
	case F_SETLK:
	case F_SETLKW:
#endif
		evaluate( &Tfcntl3, (pointer)&ret_val );
		break;

#ifdef	DEVELOPMENT
	default:
		warning("fcntl - called with unrecognized cmd");
#endif
	}
	return( ret_val );
}

