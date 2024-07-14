/*LINTLIBRARY*/
/*
 * z_tmpfile.c
 */

#ifdef	UNDEF
#ident	"@(#)z_tmpfile.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef tmpfile
FILE *tmpfile();
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

static struct ftable Ttmpfile = {
	"tmpfile",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

FILE * Ztmpfile(){
	FILE * ret_val;

	errno = 0;
	ret_val = tmpfile();
	evaluate( &Ttmpfile, (pointer)&ret_val );
	return( ret_val );
}

