/*LINTLIBRARY*/
/*
 * z_isupper.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isupper.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef isupper
int isupper();
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

static struct ftable Tisupper = {
	"isupper",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zisupper(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = isupper(c);
	evaluate( &Tisupper, (pointer)&ret_val );
	return( ret_val );
}

