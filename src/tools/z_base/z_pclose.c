/*LINTLIBRARY*/
/*
 * z_pclose.c
 */

#ifdef	UNDEF
#ident	"@(#)z_pclose.c	1.2"
#endif

#include <stdio.h>
#include <sv_env.h>

#ifndef pclose
int pclose();
#endif

static int success( val )
int *val;
{
	return( (*val) != (-1) );
}

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
}

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

static struct ftable Tpclose = {
	"pclose",
	"a return value other than (-1)",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

int  Zpclose(stream)
FILE *stream;
{
	int ret_val;

	errno = 0;
	ret_val = pclose(stream);
	evaluate( &Tpclose, (pointer)&ret_val );
	return( ret_val );
}

