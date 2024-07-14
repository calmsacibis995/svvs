/*LINTLIBRARY*/
/*
 * z_hcreate.c
 */

#ifdef	UNDEF
#ident	"@(#)z_hcreate.c	1.2"
#endif

#include <search.h>
#include <sv_env.h>

#ifndef hcreate
int hcreate();
#endif

static int success( val )
int *val;
{
	return( (*val)? 1 : 0 );
}

static int failure( val )
int *val;
{
	return( (*val)? 0 : 1 );
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

static struct ftable Thcreate = {
	"hcreate",
	"a return value other than 0",
	"a return value of 0",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zhcreate(nel)
unsigned nel;
{
	int ret_val;

	errno = 0;
	ret_val = hcreate(nel);
	evaluate( &Thcreate, (pointer)&ret_val );
	return( ret_val );
}

