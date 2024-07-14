/*LINTLIBRARY*/
/*
 * z__tolower.c
 */

#ifdef UNDEF
#ident	"@(#)z__tolower.c	1.3"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef _tolower
int _tolower();
#endif

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

static struct ftable T_tolower = {
	"_tolower",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Z_tolower(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = _tolower(c);
	evaluate( &T_tolower, (pointer)&ret_val );
	return( ret_val );
}

