/*LINTLIBRARY*/
/*
 * z_getenv.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getenv.c	1.2"
#endif

#include <sv_env.h>

#ifndef getenv
char *getenv();
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

static struct ftable Tgetenv = {
	"getenv",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

char * Zgetenv(name)
char *name;
{
	char * ret_val;

	errno = 0;
	ret_val = getenv(name);
	evaluate( &Tgetenv, (pointer)&ret_val );
	return( ret_val );
}

