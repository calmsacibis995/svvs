/*LINTLIBRARY*/
/*
 * z_islower.c
 */

#ifdef	UNDEF
#ident	"@(#)z_islower.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef islower
int islower();
#endif

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

static struct ftable Tislower = {
	"islower",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zislower(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = islower(c);
	evaluate( &Tislower, (pointer)&ret_val );
	return( ret_val );
}

