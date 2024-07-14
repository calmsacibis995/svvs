/*LINTLIBRARY*/
/*
 * z_gsignal.c
 */

#ifdef	UNDEF
#ident	"@(#)z_gsignal.c	1.2"
#endif

#include <signal.h>
#include <sv_env.h>

#ifndef gsignal
int gsignal();
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

static struct ftable Tgsignal = {
	"gsignal",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zgsignal(sig)
int sig;
{
	int ret_val;

	errno = 0;
	ret_val = gsignal(sig);
	evaluate( &Tgsignal, (pointer)&ret_val );
	return( ret_val );
}

