/*LINTLIBRARY*/
/*
 * z_abs.c
 */

#ifdef	UNDEF
#ident	"@(#)z_abs.c	1.2"
#endif

#include <sv_env.h>

#ifndef abs
int abs();
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

static struct ftable Tabs = {
	"abs",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zabs(i)
int i;
{
	int ret_val;

	errno = 0;
	ret_val = abs(i);
	evaluate( &Tabs, (pointer)&ret_val );
	return( ret_val );
}

