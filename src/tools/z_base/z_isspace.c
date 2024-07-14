/*LINTLIBRARY*/
/*
 * z_isspace.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isspace.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef isspace
int isspace();
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

static struct ftable Tisspace = {
	"isspace",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zisspace(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = isspace(c);
	evaluate( &Tisspace, (pointer)&ret_val );
	return( ret_val );
}

