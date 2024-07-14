/*LINTLIBRARY*/
/*
 * z_strspn.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strspn.c	1.2"
#endif

#include <string.h>
#include <sv_env.h>

#ifndef strspn
int strspn();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
}

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

static struct ftable Tstrspn = {
	"strspn",
	"a return value of at least 0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zstrspn(s1, s2)
char *s1;
char *s2;
{
	int ret_val;

	errno = 0;
	ret_val = strspn(s1, s2);
	evaluate( &Tstrspn, (pointer)&ret_val );
	return( ret_val );
}

