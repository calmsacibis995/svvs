/*LINTLIBRARY*/
/*
 * z_sleep.c
 */

#ifdef	UNDEF
#ident	"@(#)z_sleep.c	1.2"
#endif

#include <sv_env.h>

#ifndef sleep
unsigned sleep();
#endif

static char *format( val )
unsigned *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%u", *val);
	return( buf );
}

static int compare( a, b )
unsigned *a;
unsigned *b;
{
	return( *a == *b );
}

unsigned *sv_getu();

static struct ftable Tsleep = {
	"sleep",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getu,
	0,
};

unsigned  Zsleep(seconds)
unsigned seconds;
{
	unsigned ret_val;

	errno = 0;
	ret_val = sleep(seconds);
	evaluate( &Tsleep, (pointer)&ret_val );
	return( ret_val );
}

