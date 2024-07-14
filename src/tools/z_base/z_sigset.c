/*LINTLIBRARY*/
/*
 * z_sigset.c
 */

#ifdef	UNDEF
#ident	"@(#)z_sigset.c	1.2"
#endif

#include <signal.h>
#include <sv_env.h>

#ifndef sigset
void (*sigset())();
#endif
char	*strcpy();

typedef int (*sigptr())();

#ifndef	SIG_ERR
#define	SIG_ERR	(void (*)())(-1)
#endif

static int success( val )
void	(**val)();
{
	return( (*val) != SIG_ERR );
}

static int failure( val )
void	(**val)();
{
	return( (*val) == SIG_ERR );
}

static char *format( val )
pointer *val;
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

static struct ftable Tsigset = {
	"sigset",
	"a return value other than SIGERR",
	"a return value of SIGERR",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	1,
};

void (* Zsigset(sig, func))()
int sig;
void (*func)();
{
	void	(*ret_val)();

	errno = 0;
	ret_val = sigset(sig, func);
	evaluate( &Tsigset,(pointer)&ret_val );
	return( ret_val );
}
