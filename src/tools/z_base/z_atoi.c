/*LINTLIBRARY*/
/*
 * z_atoi.c
 */

#ifdef	UNDEF
#ident	"@(#)z_atoi.c	1.2"
#endif

#include <sv_env.h>

#ifndef atoi
int atoi();
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

static struct ftable Tatoi = {
	"atoi",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zatoi(str)
char *str;
{
	int ret_val;

	errno = 0;
	ret_val = atoi(str);
	evaluate( &Tatoi, (pointer)&ret_val );
	return( ret_val );
}

