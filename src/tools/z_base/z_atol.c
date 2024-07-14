/*LINTLIBRARY*/
/*
 * z_atol.c
 */

#ifdef	UNDEF
#ident	"@(#)z_atol.c	1.2"
#endif

#include <sv_env.h>

#ifndef atol
long atol();
#endif

static char *format( val )
long *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%ld", *val );
	return( buf );
}

static int compare( a, b )
long *a;
long *b;
{
	return( *a == *b );
}

long *sv_getl();

static struct ftable Tatol = {
	"atol",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getl,
	0,
};

long  Zatol(str)
char *str;
{
	long ret_val;

	errno = 0;
	ret_val = atol(str);
	evaluate( &Tatol, (pointer)&ret_val );
	return( ret_val );
}

