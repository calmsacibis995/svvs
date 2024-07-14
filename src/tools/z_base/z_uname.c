/*LINTLIBRARY*/
/*
 * z_uname.c
 */

#ifdef	UNDEF
#ident	"@(#)z_uname.c	1.2"
#endif

#include <sys/utsname.h>
#include <sv_env.h>

#ifndef uname
int uname();
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

static struct ftable Tuname = {
	"uname",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zuname(name)
struct utsname *name;
{
	int ret_val;

	errno = 0;
	ret_val = uname(name);
	evaluate( &Tuname, (pointer)&ret_val );
	return( ret_val );
}

