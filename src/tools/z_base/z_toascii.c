/*LINTLIBRARY*/
/*
 * z_toascii.c
 */

#ifdef	UNDEF
#ident	"@(#)z_toascii.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef toascii
int toascii();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 && (*val) < 0177 );
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

static struct ftable Ttoascii = {
	"toascii",
	"a return value of at least 0 and less than 0177",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Ztoascii(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = toascii(c);
	evaluate( &Ttoascii, (pointer)&ret_val );
	return( ret_val );
}

