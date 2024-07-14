/*LINTLIBRARY*/
/*
 * z_strtol.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strtol.c	1.2"
#endif

#include <sv_env.h>

#ifndef strtol
long strtol();
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

static struct ftable Tstrtol = {
	"strtol",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getl,
	0,
};

long  Zstrtol(str, ptr, base)
char *str;
char **ptr;
int base;
{
	long ret_val;

	errno = 0;
	ret_val = strtol(str, ptr, base);
	evaluate( &Tstrtol, (pointer)&ret_val );
	return( ret_val );
}

