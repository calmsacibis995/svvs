/*LINTLIBRARY*/
/*
 * z_ctime.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ctime.c	1.3"
#endif

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include <time.h>
#include <svvs.h>
#include <sv_env.h>

#ifndef ctime
char *ctime();
#endif
char	*strcpy();

static int success( val )
char **val;
{
	return( (*val)? 1 : 0 );
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

static struct ftable Tctime = {
	"ctime",
	"a return value other than NULL",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getp,
	0,
};

char * Zctime(clock)
ttime *clock;
{
	char * ret_val;

	errno = 0;
	ret_val = ctime(clock);
	evaluate( &Tctime, (pointer)&ret_val );
	return( ret_val );
}

