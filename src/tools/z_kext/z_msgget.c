/*LINTLIBRARY*/
/*
 * z_msgget.c
 */

#ifdef	UNDEF
#ident	"@(#)z_msgget.c	1.3"
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sv_env.h>

#ifndef msgget
int msgget();
#endif

static int success( val )
int *val;
{
	return( (*val) >= 0 );
}

static int failure( val )
int *val;
{
	return( (*val) == (-1) );
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

static struct ftable Tmsgget = {
	"msgget",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zmsgget(key, msgflg)
key_t key;
int msgflg;
{
	int ret_val;

	errno = 0;
	ret_val = msgget(key, msgflg);
	evaluate( &Tmsgget, (pointer)&ret_val );
	if( success( &ret_val ) )
		sv_add_ipc_id( ret_val, SV_MSG );
	return( ret_val );
}

