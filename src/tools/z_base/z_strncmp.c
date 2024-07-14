/*LINTLIBRARY*/
/*
 * z_strncmp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_strncmp.c	1.3"
#endif

#include <sys/types.h>
#include <string.h>
#include <sv_env.h>

#ifndef strncmp
int strncmp();
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

static struct ftable Tstrncmp = {
	"strncmp",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zstrncmp(s1, s2, n)
char *s1;
char *s2;
#ifndef	SYSVR_2
size_t	n;
#else
int n;
#endif
{
	int ret_val;

	errno = 0;
	ret_val = strncmp(s1, s2, n);
	evaluate( &Tstrncmp, (pointer)&ret_val );
	return( ret_val );
}

