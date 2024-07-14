/*LINTLIBRARY*/
/*
 * z_isxdigit.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isxdigit.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef isxdigit
int isxdigit();
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

static struct ftable Tisxdigit = {
	"isxdigit",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zisxdigit(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = isxdigit(c);
	evaluate( &Tisxdigit, (pointer)&ret_val );
	return( ret_val );
}

