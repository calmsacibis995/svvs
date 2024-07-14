/*LINTLIBRARY*/
/*
 * z_getcwd.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getcwd.c	1.2"
#endif

#include <sv_env.h>

#ifndef getcwd
char *getcwd();
#endif
char	*strcpy();

static int success( val )
char **val;
{
	return( (*val)? 1 : 0 );
}

static int failure( val )
char **val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
char **val;
{
	static char buf[STRVAL];

	if( !*val )
		(void) strcpy( buf, "NULL" );
	else if( (pointer)(*val) == NON_NULL )
		(void) strcpy( buf, "NON-NULL" );
	else
		(void) sprintf( buf, "0x%lx", (long)(*val) );
	return( buf );
}

static int compare( a, b )
char **a;
char **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tgetcwd = {
	"getcwd",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	1,
};

char * Zgetcwd(buf, size)
char *buf;
int size;
{
	char * ret_val;

	errno = 0;
	ret_val = getcwd(buf, size);
	evaluate( &Tgetcwd, (pointer)&ret_val );
	return( ret_val );
}

