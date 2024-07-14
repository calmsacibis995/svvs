/*LINTLIBRARY*/
/*
 * z_ssignal.c
 */

#ifdef	UNDEF
#ident	"@(#)z_ssignal.c	1.2"
#endif

#include <signal.h>
#include <sv_env.h>

#ifndef ssignal
int (*ssignal())();
#endif
char	*strcpy();

static int success( val )
pointer *val;
{
	return( (*val)? 1 : 0 );
}

static int failure( val )
int (**val)();
{
	return( (*val)? 0 : 1 );
}

static char *format( val )
pointer	*val;
{
	static char buf[STRVAL];

	if( !*val )
		(void) strcpy( buf, "NULL" );
	else if( (*val) == NON_NULL )
		(void) strcpy( buf, "NON-NULL" );
	else
		(void) sprintf( buf, "0x%lx", (long)(*val) );
	return( buf );
}

static int compare( a, b )
pointer *a;
pointer *b;
{
	if( (*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tssignal = {
	"ssignal",
	"a return value other than NULL",
	"a return value of NULL",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	0,
};

int	(*Zssignal(sig, action))()
int sig;
int	(*action)();
{
	int	(*ret_val)();

	errno = 0;
	ret_val = ssignal(sig, action);
	evaluate( &Tssignal, (pointer)&ret_val );
	return( ret_val );
}

