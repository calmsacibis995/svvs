/*LINTLIBRARY*/
/*
 * z_umask.c
 */

#ifdef	UNDEF
#ident	"@(#)z_umask.c	1.3"
#endif

#include <sv_env.h>

#ifndef umask
int umask();
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

static struct ftable Tumask = {
	"umask",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zumask(cmask)
int cmask;
{
	int ret_val;

	errno = 0;
	ret_val = umask(sv_mode(cmask));
	evaluate( &Tumask, (pointer)&ret_val );
	return( ret_val );
}

