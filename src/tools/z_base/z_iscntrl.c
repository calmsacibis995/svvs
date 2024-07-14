/*LINTLIBRARY*/
/*
 * z_iscntrl.c
 */

#ifdef	UNDEF
#ident	"@(#)z_iscntrl.c	1.2"
#endif

#include <ctype.h>
#include <sv_env.h>

#ifndef iscntrl
int iscntrl();
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

static struct ftable Tiscntrl = {
	"iscntrl",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Ziscntrl(c)
int c;
{
	int ret_val;

	errno = 0;
	ret_val = iscntrl(c);
	evaluate( &Tiscntrl, (pointer)&ret_val );
	return( ret_val );
}

