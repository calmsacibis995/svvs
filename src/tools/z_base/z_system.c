/*LINTLIBRARY*/
/*
 * z_system.c
 */

#ifdef	UNDEF
#ident	"@(#)z_system.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef system
int system();
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

static struct ftable Tsystem = {
	"system",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zsystem(string)
char *string;
{
	int ret_val;

	errno = 0;
	ret_val = system(string);
	evaluate( &Tsystem, (pointer)&ret_val );
	return( ret_val );
}

