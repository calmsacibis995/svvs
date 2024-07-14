/*LINTLIBRARY*/
/*
 * z_erasecha.c
 */

#ifdef	UNDEF
#ident	"@(#)z_killchar.c	1.2"
#endif

#include <curses.h>
#include <sv_env.h>

#ifndef killchar
char killchar();
#endif

static char *format( val )
char *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "0x%x", *val );
	return( buf );
}

static int compare( a, b )
char *a;
char *b;
{
	return( *a == *b );
}

char *sv_getc();

static struct ftable Tkillchar = {
	"killchar",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getc,
	0,
};

char  Zkillchar(){
	char ret_val;

	errno = 0;
	ret_val = killchar();
	evaluate( &Tkillchar, (pointer)&ret_val );
	return( ret_val );
}

