/*LINTLIBRARY*/
/*
 * z_lcong48.c
 */

#ifdef	UNDEF
#ident	"@(#)z_lcong48.c	1.3"
#endif

#include <sv_env.h>

#ifndef lcong48
void lcong48();
#endif

static char *format() {
	return( "void/undefined" );
}

static int compare() {
#ifdef	DEVELOPMENT
	warning( "sv_test for void function called" );
#endif
	return( 0 );
}

#ifdef	DEVELOPMENT
void sv_getv();
#endif

static struct ftable Tlcong48 = {
	"lcong48",
	"return from the function",
	"not defined",
	format,
	compare,
	RET,
	(ifunctptr)0,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

void  Zlcong48(param)
unsigned short param[7];
{
	errno = 0;
	lcong48(param);
	evaluate( &Tlcong48, (pointer)0 );
	return;
}
