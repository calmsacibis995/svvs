/*LINTLIBRARY*/
/*
 * z_newpad.c
 */

#ifdef	UNDEF
#ident	"@(#)z_newpad.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef newpad
WINDOW *newpad();
#endif
char	*strcpy();

static int success( val )
WINDOW **val;
{
	return( (*val) != (WINDOW *)NULL );
}

static int failure( val )
WINDOW **val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
WINDOW **val;
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
WINDOW **a;
WINDOW **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tnewpad = {
	"newpad",
	"a non-NULL return value",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

WINDOW * Znewpad( nlines, ncols )
int nlines;
int ncols;
{
	WINDOW * ret_val;

	errno = 0;
	ret_val = newpad( nlines, ncols );
	evaluate( &Tnewpad, (pointer)&ret_val );
	return( ret_val );
}

