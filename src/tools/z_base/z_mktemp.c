/*LINTLIBRARY*/
/*
 * z_mktemp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_mktemp.c	1.2"
#endif

#include <sv_env.h>

#ifndef mktemp
char *mktemp();
#endif
char	*strcpy();

static int success( val )
char **val;
{
	return( (*val) && *(*val) );
}

static int failure( val )
char **val;
{
	return( (*val) && !(*(*val)) );
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

static struct ftable Tmktemp = {
	"mktemp",
	"a non-null return value pointing to a non-null string",
	"a non-null return value pointing to a null string",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	1,
};

char * Zmktemp(template)
char *template;
{
	char * ret_val;

	errno = 0;
	ret_val = mktemp(template);
	evaluate( &Tmktemp, (pointer)&ret_val );
	return( ret_val );
}

