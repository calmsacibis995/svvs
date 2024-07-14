/*LINTLIBRARY*/
/*
 * z_newterm.c
 */

#ifdef	UNDEF
#ident	"@(#)z_newterm.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef newterm
SCREEN *newterm();
#endif
char	*strcpy();


static int success( val )
SCREEN **val;
{
	return( (*val) != (SCREEN *)NULL );
}

static int failure( val )
SCREEN **val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
SCREEN **val;
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
SCREEN **a;
SCREEN **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tnewterm = {
	"newterm",
	"a non-NULL return value",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

SCREEN * Znewterm(typ, outfd, infd)
char *typ;
FILE *outfd;
FILE *infd;
{
	SCREEN * ret_val;

	errno = 0;
	ret_val = newterm(typ, outfd, infd);
	evaluate( &Tnewterm, (pointer)&ret_val );
	return( ret_val );
}

