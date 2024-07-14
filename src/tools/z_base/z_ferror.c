/*LINTLIBRARY*/
/*
 * z_ferror.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ferror.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef ferror
int ferror();
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

static struct ftable Tferror = {
	"ferror",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zferror(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = ferror(stream);
	evaluate( &Tferror, (pointer)&ret_val );
	return( ret_val );
}

