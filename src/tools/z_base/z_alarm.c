/*LINTLIBRARY*/
/*
 * z_alarm.c
 */

#ifdef	UNDEF
#ident	"@(#)z_alarm.c	1.2"
#endif

#include <sv_env.h>

#ifndef alarm
unsigned alarm();
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

static struct ftable Talarm = {
	"alarm",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getu,
	0,
};

unsigned  Zalarm(sec)
unsigned sec;
{
	unsigned ret_val;

	errno = 0;
	ret_val = alarm(sec);
	evaluate( &Talarm, (pointer)&ret_val );
	return( ret_val );
}

