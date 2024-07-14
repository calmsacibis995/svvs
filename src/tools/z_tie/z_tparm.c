/*LINTLIBRARY*/
/*
 * z_tparm.c
 */

#ifdef	UNDEF
#ident	"@(#)z_tparm.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef tparm
char *tparm();
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

static struct ftable Ttparm = {
	"tparm",
	"a non-NULL return value",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

char * Ztparm( str, p1, p2, p3, p4, p5, p6, p7, p8, p9 )
char *str;
int p1;
int p2;
int p3;
int p4;
int p5;
int p6;
int p7;
int p8;
int p9;
{
	char * ret_val;

	errno = 0;
	ret_val = tparm( str, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	evaluate( &Ttparm, (pointer)&ret_val );
	return( ret_val );
}

