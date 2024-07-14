/*LINTLIBRARY*/
/*
 * z_encrypt.c
 */

#ifdef	UNDEF
#ident	"@(#)z_encrypt.c	1.3"
#endif

#include <sv_env.h>

#ifndef encrypt
void encrypt();
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

static struct ftable Tencrypt = {
	"encrypt",
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

void  Zencrypt(block, fake)
char *block;
int fake;
{
	errno = 0;
	encrypt(block, fake);
	evaluate( &Tencrypt, (pointer)0 );
	return;
}

