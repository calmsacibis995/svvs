/*LINTLIBRARY*/
/*
 * z_compile.c
 */

#ifdef	UNDEF
#ident	"@(#)z_compile.c	1.3"
#endif

#include <sv_env.h>

#ifndef compile
char *compile();
#endif
char	*strcpy();

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

#ifdef	DEVELOPMENT
void sv_getv();
#endif

static struct ftable Tcompile = {
	"compile",
	"indeterminate - based upon actual expected value",
	"indeterminate - based upon actual expected value",
	format,
	compare,
	VALUE,
	VALUE,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

char * Zcompile( instring, expbuf, endbuf, eof)
char *instring;
char *expbuf;
char *endbuf;
int eof;
{
	char * ret_val;

	errno = 0;
	ret_val = compile( instring, expbuf, endbuf, eof);
	evaluate( &Tcompile, (pointer)&ret_val );
	return( ret_val );
}

