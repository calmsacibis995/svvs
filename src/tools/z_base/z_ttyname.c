/*LINTLIBRARY*/
/*
 * z_ttyname.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ttyname.c	1.2"
#endif

#include <sv_env.h>

#ifndef ttyname
char *ttyname();
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

static struct ftable Tttyname = {
	"ttyname",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

char * Zttyname(fildes)
int fildes;
{
	char * ret_val;

	errno = 0;
	ret_val = ttyname(fildes);
	evaluate( &Tttyname, (pointer)&ret_val );
	return( ret_val );
}

