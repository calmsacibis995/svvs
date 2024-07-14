/*LINTLIBRARY*/
/*
 * z_isprint.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isprint.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef isprint
int isprint();
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

static struct ftable Tisprint = {
	"isprint",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zisprint(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = isprint(c);
	evaluate( &Tisprint, (pointer)&ret_val );
	return( ret_val );
}

