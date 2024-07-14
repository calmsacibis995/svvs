/*LINTLIBRARY*/
/*
 * z_isalnum.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isalnum.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef isalnum
int isalnum();
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

static struct ftable Tisalnum = {
	"isalnum",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zisalnum(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = isalnum(c);
	evaluate( &Tisalnum, (pointer)&ret_val );
	return( ret_val );
}

