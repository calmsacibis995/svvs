/*LINTLIBRARY*/
/*
 * z_msgrcv.c
 */

#ifdef	UNDEF
#ident	"@(#)z_msgrcv.c	1.2"
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sv_env.h>

#ifndef msgrcv
int msgrcv();
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

static struct ftable Tmsgrcv = {
	"msgrcv",
	"a return value of at least 0",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zmsgrcv(msqid, msgp, msgsz, msgtyp, msgflg)
int msqid;
char *msgp;
int msgsz;
long msgtyp;
int msgflg;
{
	int ret_val;

	errno = 0;
	ret_val = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
	evaluate( &Tmsgrcv, (pointer)&ret_val );
	return( ret_val );
}

