/*LINTLIBRARY*/
/*
 * z_memset.c
 */

#ifdef	UNDEF
#ident	"@(#)z_memset.c	1.2"
#endif

#include <memory.h>
#include <sv_env.h>

#ifndef memset
char *memset();
#endif
char	*strcpy();

static int success( val )
char **val;
{
	return( (*val)? 1 : 0 );
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

static struct ftable Tmemset = {
	"memset",
	"a return value other than NULL",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getp,
	0,
};

char * Zmemset(s, c, n)
char *s;
int c;
int n;
{
	char * ret_val;

	errno = 0;
	ret_val = memset(s, c, n);
	evaluate( &Tmemset, (pointer)&ret_val );
	return( ret_val );
}

