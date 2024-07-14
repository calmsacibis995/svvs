/*LINTLIBRARY*/
/*
 * z_setpgrp.c
 */

#ifdef	UNDEF
#ident	"@(#)z_setpgrp.c	1.2"
#endif

#include <sv_env.h>

#ifndef setpgrp
int setpgrp();
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

static struct ftable Tsetpgrp = {
	"setpgrp",
	"indeterminate - based upon actual expected value",
	"not defined",
	format,
	compare,
	VALUE,
	(ifunctptr)0,
	(pfunctptr)sv_geti,
	0,
};

int  Zsetpgrp(){
	int ret_val;

	errno = 0;
	ret_val = setpgrp();
	evaluate( &Tsetpgrp, (pointer)&ret_val );
	return( ret_val );
}

