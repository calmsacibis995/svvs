/*LINTLIBRARY*/
/*
 * z_isgraph.c
 */

#ifdef	UNDEF
#ident	"@(#)z_isgraph.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef isgraph
int isgraph();
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

static struct ftable Tisgraph = {
	"isgraph",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zisgraph(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = isgraph(c);
	evaluate( &Tisgraph, (pointer)&ret_val );
	return( ret_val );
}

