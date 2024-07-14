/*LINTLIBRARY*/
/*
 * z_getw.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getw.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef getw
int getw();
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

static struct ftable Tgetw = {
	"getw",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zgetw(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = getw(stream);
	evaluate( &Tgetw, (pointer)&ret_val );
	return( ret_val );
}

