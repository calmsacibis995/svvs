#ifndef	SYSVR_2  /*  dup2 was added in SVR3  */

/*LINTLIBRARY*/
/*
 * sv_dup2.c
 */

#ident	"@(#)z_dup2.c	1.1"

#include <sv_env.h>

#ifndef dup2
int dup2();
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

static struct ftable Tdup2 = {
	"dup2",
	"a return value greater than 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zdup2(fildes, filedes2)
int fildes, filedes2;
{
	int ret_val;

	errno = 0;
	ret_val = dup2(fildes, filedes2);
	evaluate( &Tdup2, (pointer)&ret_val );
	return( ret_val );
}

#endif
