/*LINTLIBRARY*/
/*
 * z_memcpy.c
 */

#ifdef	UNDEF
#ident	"@(#)z_memcpy.c	1.2"
#endif

#include <memory.h>
#include <sv_env.h>

#ifndef memcpy
char *memcpy();
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

static struct ftable Tmemcpy = {
	"memcpy",
	"a return value other than NULL",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getp,
	0,
};

char * Zmemcpy(s1, s2, n)
char *s1;
char *s2;
int n;
{
	char * ret_val;

	errno = 0;
	ret_val = memcpy(s1, s2, n);
	evaluate( &Tmemcpy, (pointer)&ret_val );
	return( ret_val );
}

