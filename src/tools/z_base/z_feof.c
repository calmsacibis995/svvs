/*LINTLIBRARY*/
/*
 * z_feof.c
 */

#ifdef	UNDEF
#ident	"@(#)z_feof.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef feof
int feof();
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

static struct ftable Tfeof = {
	"feof",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zfeof(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = feof(stream);
	evaluate( &Tfeof, (pointer)&ret_val );
	return( ret_val );
}

