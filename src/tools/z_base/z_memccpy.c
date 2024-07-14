/*LINTLIBRARY*/
/*
 * z_memccpy.c
 */

#ifdef	UNDEF
#ident	"@(#)z_memccpy.c	1.2"
#endif

#include <memory.h>
#include <sv_env.h>

#ifndef memccpy
char *memccpy();
#endif
char	*strcpy();

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

static struct ftable Tmemccpy = {
	"memccpy",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getp,
	0,
};

char *Zmemccpy(s1, s2, c, n)
char *s1;
char *s2;
int c;
int n;
{
	char * ret_val;

	errno = 0;
	ret_val = memccpy(s1, s2, c, n);
	evaluate( &Tmemccpy, (pointer)&ret_val );
	return( ret_val );
}

