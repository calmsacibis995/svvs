/*LINTLIBRARY*/
/*
 * z_getpgrp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_getpgrp.c	1.2"
#endif

#include <sv_env.h>

#ifndef getpgrp
int getpgrp();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
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

static struct ftable Tgetpgrp = {
	"getpgrp",
	"a return value of at least 0",
	"not defined",
	format,
	compare,
	success,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zgetpgrp(){
	int ret_val;

	errno = 0;
	ret_val = getpgrp();
	evaluate( &Tgetpgrp, (pointer)&ret_val );
	return( ret_val );
}

