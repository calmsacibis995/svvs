/*LINTLIBRARY*/
/*
 * z_slk_labe.c
 */

#ifdef	UNDEF
#ident	"@(#)z_slk_labe.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef slk_label
char *slk_label();
#endif
char	*strcpy();


static int success( val )
char **val;
{
	return( (*val) != (char *)NULL );
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

static struct ftable Tslk_label = {
	"slk_label",
	"a non-NULL return value",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

char * Zslk_label( labnum )
int labnum;
{
	char * ret_val;

	errno = 0;
	ret_val = slk_label( labnum );
	evaluate( &Tslk_label, (pointer)&ret_val );
	return( ret_val );
}

