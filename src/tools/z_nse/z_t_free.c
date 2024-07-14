/*LINTLIBRARY*/
/*
 * z_t_free.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_free.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_free
int t_free();
#endif

static int success( val )
int *val;
{
	return( (*val)? 0 : 1 );
}

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
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

static struct ftable Tt_free = {
	"t_free",
	"a return value of 0",
	"a return value of -1",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	3,
};

int  Zt_free( ptr, struct_type )
char *ptr;
int struct_type;
{
	int ret_val;

	errno = 0;
	ret_val = t_free( ptr, struct_type );
	evaluate( &Tt_free, (pointer)&ret_val );
	return( ret_val );
}

