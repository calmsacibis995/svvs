/*LINTLIBRARY*/
/*
 * z_rand.c
 */

#ifdef	UNDEF
#ident	"@(#)z_rand.c	1.2"
#endif

#include <sv_env.h>

#ifndef rand
int rand();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 && (*val) <= 32767 );
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

static struct ftable Trand = {
	"rand",
	"a return value of at least 0 and at most 32767",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zrand(){
	int ret_val;

	errno = 0;
	ret_val = rand();
	evaluate( &Trand, (pointer)&ret_val );
	return( ret_val );
}

