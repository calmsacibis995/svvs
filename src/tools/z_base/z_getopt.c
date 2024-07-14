/*LINTLIBRARY*/
/*
 * z_getopt.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getopt.c	1.2"
#endif

#include <sv_env.h>

#ifndef getopt
int getopt();
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

static struct ftable Tgetopt = {
	"getopt",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zgetopt(argc, argv, optstring)
int argc;
char **argv;
char *optstring;
{
	int ret_val;

	errno = 0;
	ret_val = getopt(argc, argv, optstring);
	evaluate( &Tgetopt, (pointer)&ret_val );
	return( ret_val );
}

