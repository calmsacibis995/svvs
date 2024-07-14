/*LINTLIBRARY*/
/*
 * z_t_alloc.c
 */

#ifdef	UNDEF
#ident	"@(#)z_t_alloc.c	1.2"
#endif

#include <sys/tiuser.h>
#include <sv_env.h>

#ifndef t_alloc
char *t_alloc();
#endif
char	*strcpy();

static int success( val )
char **val;
{
	return( (*val)? 1 : 0 );
}

static int failure( val )
char **val;
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
char **val;
{
	static char buf[STRVAL];

	if( !*val )
		(void) strcpy( buf, "NULL" );
	else if( (pointer)(*val) == NON_NULL )
		(void) strcpy( buf, "NON-NULL" );
	else
		(void) sprintf( buf, "0x%lx", (long)(*val) );
	return( buf );
}

static int compare( a, b )
char **a;
char **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tt_alloc = {
	"t_alloc",
	"a non-NULL return value",
	"a NULL return value",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	3,
};

char * Zt_alloc( fd, struct_type, fields )
int fd;
int struct_type;
int fields;
{
	char * ret_val;

	errno = 0;
	sv_look_fd = fd;
	ret_val = t_alloc( fd, struct_type, fields );
	evaluate( &Tt_alloc, (pointer)&ret_val );
	return( ret_val );
}
