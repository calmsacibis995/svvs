/*LINTLIBRARY*/
/*
 * z_memcmp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_memcmp.c	1.2"
#endif

#include <memory.h>
#include <sv_env.h>

#ifndef memcmp
int memcmp();
#endif

#ifdef THREE_WAY_COMPARE

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	if( !*val ) (void) sprintf( buf, "0" );
	if( *val < 0 ) (void) sprintf( buf, "a value less than 0" );
	if( *val > 0 ) (void) sprintf( buf, "a value greater than 0" );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	if( !*b )
		return( !*a );
	if( *b < 0 )
		return( *a < 0 );
	return( *a > 0 );
}

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

static struct ftable Tmemcmp = {
	"memcmp",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zmemcmp(s1, s2, n)
char *s1;
char *s2;
int n;
{
	int ret_val;

	errno = 0;
	ret_val = memcmp(s1, s2, n);
	evaluate( &Tmemcmp, (pointer)&ret_val );
	return( ret_val );
}

