/*LINTLIBRARY*/
/*
 * z_msgsnd.c
 */

#ifdef	UNDEF
#ident	"@(#)z_msgsnd.c	1.2"
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sv_env.h>

#ifndef msgsnd
int msgsnd();
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

static struct ftable Tmsgsnd = {
	"msgsnd",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zmsgsnd(msqid, msgp, msgsz, msgflg)
int msqid;
char *msgp;
int msgsz;
int msgflg;
{
	int ret_val;

	errno = 0;
	ret_val = msgsnd(msqid, msgp, msgsz, msgflg);
	evaluate( &Tmsgsnd, (pointer)&ret_val );
	return( ret_val );
}

