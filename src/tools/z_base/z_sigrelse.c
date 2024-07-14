/*LINTLIBRARY*/
/*
 * z_sigrelse.c
 */

#ifdef	UNDEF
#ident	"@(#)z_sigrelse.c	1.2"
#endif

#include <sv_env.h>

#ifndef sigrelse
int sigrelse();
#endif

static int success( val )
int *val;
{
	return( (*val) == 0 );
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

static struct ftable Tsigrelse = {
	"sigrelse",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zsigrelse(sig)
int sig;
{
	int ret_val;

	errno = 0;
	ret_val = sigrelse(sig);
	evaluate( &Tsigrelse, (pointer)&ret_val );
	return( ret_val );
}
