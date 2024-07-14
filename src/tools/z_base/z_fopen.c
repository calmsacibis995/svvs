/*LINTLIBRARY*/
/*
 * z_fopen.c
 */

#ifdef	UNDEF
#ident	"@(#)z_fopen.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef fopen
FILE *fopen();
#endif
char	*strcpy();

static int success( val )
FILE **val;
{
	return( (*val)? 1 : 0 );
}

static int failure( val )
FILE **val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
FILE **val;
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
FILE **a;
FILE **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tfopen = {
	"fopen",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	1,
};

FILE * Zfopen(filename, type)
char *filename;
char *type;
{
	FILE * ret_val;

	errno = 0;
	ret_val = fopen(filename, type);
	evaluate( &Tfopen, (pointer)&ret_val );
	return( ret_val );
}

