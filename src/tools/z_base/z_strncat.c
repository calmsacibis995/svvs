/*LINTLIBRARY*/
/*
 * z_strncat.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strncat.c	1.3"
#endif

#include <string.h>
#ifndef	SYSVR_2
#include	<sys/types.h>
#endif
#include <sv_env.h>

#ifndef strncat
char *strncat();
#endif

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

static struct ftable Tstrncat = {
	"strncat",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	1,
};

char * Zstrncat(s1, s2, n)
char *s1;
char *s2;

#ifndef	SYSVR_2
	size_t	n;
#else
	int	n;
#endif
{
	char * ret_val;

	errno = 0;
	ret_val = strncat(s1, s2, n);
	evaluate( &Tstrncat, (pointer)&ret_val );
	return( ret_val );
}

