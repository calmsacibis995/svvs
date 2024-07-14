/*LINTLIBRARY*/
/*
 * z_msgctl.c
 */

#ifdef	UNDEF
#ident	"@(#)z_msgctl.c	1.3"
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sv_env.h>

#ifndef msgctl
int msgctl();
#endif

static int success( val )
int *val;
{
	return( (*val)? 0 : 1 );
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

static struct ftable Tmsgctl = {
	"msgctl",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zmsgctl(msqid, cmd, buf)
int msqid;
int cmd;
struct	msqid_ds *buf;
{
	int ret_val;

	errno = 0;
	ret_val = msgctl(msqid, cmd, buf);
	evaluate( &Tmsgctl, (pointer)&ret_val );
	if( cmd == IPC_RMID && success( &ret_val ) )
		sv_del_ipc_id( msqid, SV_MSG );
	return( ret_val );
}

