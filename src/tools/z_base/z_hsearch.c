/*LINTLIBRARY*/
/*
 * z_hsearch.c
 */

#ifdef	UNDEF
#ident	"@(#)z_hsearch.c	1.2"
#endif

#include <search.h>
#include <sv_env.h>

#ifndef hsearch
ENTRY *hsearch();
#endif
char	*strcpy();

static int success( val )
ENTRY **val;
{
	return( (*val)? 1 : 0 );
}

static int failure( val )
ENTRY **val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
ENTRY **val;
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
ENTRY **a;
ENTRY **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Thsearch = {
	"hsearch",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

ENTRY * Zhsearch(item, action)
ENTRY item;
ACTION action;
{
	ENTRY * ret_val;

	errno = 0;
	ret_val = hsearch(item, action);
	evaluate( &Thsearch, (pointer)&ret_val );
	return( ret_val );
}

