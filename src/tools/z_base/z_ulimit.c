/*LINTLIBRARY*/
/*
 * z_ulimit.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ulimit.c	1.2"
#endif

#include <sv_env.h>

#ifndef ulimit
long ulimit();
#endif

static int success( val )
long *val;
{
	return( (*val) >= 0 );
}

static int failure( val )
long *val;
{
	return( (*val) == (-1) );
}

static char *format( val )
long *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%ld", *val );
	return( buf );
}

static int compare( a, b )
long *a;
long *b;
{
	return( *a == *b );
}

long *sv_getl();

static struct ftable Tulimit = {
	"ulimit",
	"a non-negative return value",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getl,
	1,
};

long  Zulimit(cmd, newlimit)
int cmd;
long newlimit;
{
	long ret_val;

	errno = 0;
	ret_val = ulimit(cmd, newlimit);
	evaluate( &Tulimit, (pointer)&ret_val );
	return( ret_val );
}

