/*LINTLIBRARY*/
/*
 * z_getgid.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getgid.c	1.2"
#endif

#include <sv_env.h>

#ifndef getgid
unsigned short getgid();
#endif

static char *format( val )
sv_ugid *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%u", *val);
	return( buf );
}

static int compare( a, b )
sv_ugid *a;
sv_ugid *b;
{
	return( *a == *b );
}

unsigned short *sv_getus();
unsigned *sv_getu();

static	sv_ugid *sv_getug() {
/*Lint complains about pointer problems and constants in conditionals*/
	if( sizeof( sv_ugid ) == sizeof( unsigned short ) )
		return( (sv_ugid *) sv_getus() );
	return( (sv_ugid *) sv_getu() );
/*Lint stops complaining*/
}

static struct ftable Tgetgid = {
	"getgid",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_getug,
	0,
};

unsigned short  Zgetgid(){
	unsigned short ret_val;
	sv_ugid kludge;

	errno = 0;
	ret_val = getgid();
	kludge = (sv_ugid) ret_val;
	evaluate( &Tgetgid, (pointer)&kludge );
	return( ret_val );
}

