/*LINTLIBRARY*/
/*
 * z_profil.c
 */

#ifdef	UNDEF
#ident	"@(#)z_profil.c	1.3"
#endif

#include <sv_env.h>

#ifndef profil
void profil();
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
int *sv_getv();
#endif

static struct ftable Tprofil = {
	"profil",
	"not defined",
	"not defined",
	format,
	compare,
	(ifunctptr)0,
	(ifunctptr)0,
#ifdef	DEVELOPMENT
	(pfunctptr)sv_getv,
#else
	(pfunctptr)0,
#endif
	0,
};

void  Zprofil(buff, bufsiz, offset, scale)
char *buff;
int bufsiz;
int offset;
int scale;
{
	errno = 0;
	profil(buff, bufsiz, offset, scale);
	evaluate( &Tprofil, (pointer)0 );
	return;
}

