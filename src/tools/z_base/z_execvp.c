/*LINTLIBRARY*/
/*
 * z_execvp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_execvp.c	1.2"
#endif

#include <sv_env.h>

#ifndef execvp
int execvp();
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

static struct ftable Texecvp = {
	"execvp",
	"no return from the function",
	"a return value of (-1)",
	format,
	compare,
	GONE,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zexecvp(file, argv)
char *file;
char *argv[ ];
{
	int ret_val;

	(void) sv_mproc();
	errno = 0;
	ret_val = execvp(file, argv);
	evaluate( &Texecvp, (pointer)&ret_val );
	return( ret_val );
}

