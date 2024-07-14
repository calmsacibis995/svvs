/*LINTLIBRARY*/
/*
 * z_fork.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fork.c	1.2"
#endif

#include <sv_env.h>

#ifndef fork
int fork();
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

static struct ftable Tfork = {
	"fork",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zfork(){
	int ret_val;
	int saved_errno;

	errno = 0;
	ret_val = fork();
	saved_errno = errno;
	if( ret_val )
		sv_pproc();
	else
		sv_cproc();
	errno = saved_errno;
	evaluate( &Tfork, (pointer)&ret_val );
	return( ret_val );
}

