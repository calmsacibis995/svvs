/*LINTLIBRARY*/
/*
 * z_localtime.c
 */

#ifdef	UNDEF
#ident	"@(#)z_localtim.c	1.3"
#endif

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include <time.h>
#include <svvs.h>
#include <sv_env.h>

#ifndef localtime
struct tm *localtime();
#endif
char	*strcpy();

static int success( val )
struct tm **val;
{
	return( (*val)? 1 : 0 );
}

static char *format( val )
struct tm **val;
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
struct tm **a;
struct tm **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tlocaltime = {
	"localtime",
	"a return value other than NULL",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_getp,
	0,
};

struct tm * Zlocaltime(clock)
ttime *clock;
{
	struct tm * ret_val;

	errno = 0;
	ret_val = localtime(clock);
	evaluate( &Tlocaltime, (pointer)&ret_val );
	return( ret_val );
}

