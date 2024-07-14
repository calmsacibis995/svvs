/*LINTLIBRARY*/
/*
 * z_execv.c
 */

#ifdef	UNDEF
#ident	"@(#)z_execv.c	1.2"
#endif

#include <sv_env.h>

#ifndef execv
int execv();
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

static struct ftable Texecv = {
	"execv",
	"no return from the function",
	"a return value of (-1)",
	format,
	compare,
	GONE,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zexecv(path, argv)
char *path;
char  *argv[ ];
{
	int ret_val;

	(void) sv_mproc();
	errno = 0;
	ret_val = execv(path, argv);
	evaluate( &Texecv, (pointer)&ret_val );
	return( ret_val );
}

