/*LINTLIBRARY*/
/*
 * z_ustat.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ustat.c	1.3"
#endif

#include <sys/types.h>
#include <ustat.h>
#include <sv_env.h>

#ifndef ustat
int ustat();
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

static struct ftable Tustat = {
	"ustat",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zustat(dev, buf)
#ifndef	SYSVR_2
dev_t dev;
#else
int dev;
#endif

struct ustat *buf;
{
	int ret_val;

	errno = 0;
	ret_val = ustat(dev, buf);
	evaluate( &Tustat, (pointer)&ret_val );
	return( ret_val );
}

