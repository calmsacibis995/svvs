/*LINTLIBRARY*/
/*
 * z_isatty.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isatty.c	1.2"
#endif

#include <sv_env.h>

#ifndef isatty
int isatty();
#endif

static int success( val )
int *val;
{
	return( (*val) == 1 );
}

static int failure( val )
int *val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%s", (*val)?"TRUE":"FALSE" );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( !*a == !*b );
}

int *sv_geti();

static struct ftable Tisatty = {
	"isatty",
	"a return value of 1",
	"a return value of 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zisatty(fildes)
int fildes;
{
	int ret_val;

	errno = 0;
	ret_val = isatty(fildes);
	evaluate( &Tisatty, (pointer)&ret_val );
	return( ret_val );
}

