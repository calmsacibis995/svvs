/*LINTLIBRARY*/
/*
 * z_strcmp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strcmp.c	1.2"
#endif

#include <string.h>
#include <sv_env.h>

#ifndef strcmp
int strcmp();
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

static struct ftable Tstrcmp = {
	"strcmp",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zstrcmp(s1, s2)
char *s1;
char *s2;
{
	int ret_val;

	errno = 0;
	ret_val = strcmp(s1, s2);
	evaluate( &Tstrcmp, (pointer)&ret_val );
	return( ret_val );
}

