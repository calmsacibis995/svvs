#ifndef	SYSVR_2  /*  directory was added in SVR3  */

/*LINTLIBRARY*/
/*
 * z_rewinddi.c
 */

#ifdef	UNDEF
#ident	"@(#)z_rewinddi.c	1.1"
#endif

#include <sys/types.h>
#include <dirent.h>
#include <sv_env.h>

#ifndef rewinddir
void rewinddir();
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

static struct ftable Trewinddir = {
	"rewinddir",
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

void  Zrewinddir( dirp )
DIR *dirp;
{
	errno = 0;
	rewinddir( dirp );
	evaluate( &Trewinddir, (pointer)0 );
	return;
}

#endif
