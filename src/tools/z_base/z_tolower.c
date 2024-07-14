/*LINTLIBRARY*/
/*
 * z_tolower.c
 */

#ifdef	UNDEF
#ident	"@(#)z_tolower.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef tolower
int tolower();
#endif

static int success( val )
int *val;
{
	return( (*val) >= (-1) && (*val) <= 255 );
}

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%d", *val );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}

int *sv_geti();

static struct ftable Ttolower = {
	"tolower",
	"a return value of at least (-1) and at most 255",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Ztolower(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = tolower(c);
	evaluate( &Ttolower, (pointer)&ret_val );
	return( ret_val );
}

