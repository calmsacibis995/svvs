/*LINTLIBRARY*/
/*
 * z_semctl.c
 */

#ifdef	UNDEF
#ident	"@(#)z_semctl.c	1.4"
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <sv_env.h>

#ifndef semctl
int semctl();
#endif

static int success0( val )
int *val;
{
	return( (*val) == 0 );
}
static int success1( val )
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

static struct ftable Tsemctl0 = {
	"semctl",
	"a return value of 0",
	"a return value of (-1)",
	format,
	compare,
	success0,
	failure,
	(pfunctptr)sv_geti,
	1,
};

static struct ftable Tsemctl1 = {
	"semctl",
	"a non-negative return value",
	"a return value of (-1)",
	format,
	compare,
	success1,
	failure,
	(pfunctptr)sv_geti,
	1,
};

int  Zsemctl(semid, semnum, cmd, arg)
int semid;
int cmd;
int semnum;
union {
int val;
char *buf;
char *array;
} arg;
{
	int ret_val;

	errno = 0;
	ret_val = semctl(semid, semnum, cmd, arg);
	switch( cmd ) {
	case GETVAL:
	case GETNCNT:
	case GETZCNT:
	case GETPID:
		evaluate( &Tsemctl1, (pointer)&ret_val );
		break;
	default:
#ifdef	DEVELOPMENT
		warning("semctl - called with unrecognized cmd");
#endif
	case GETALL:
	case SETALL:
	case SETVAL:
	case IPC_STAT:
	case IPC_SET:
		evaluate( &Tsemctl0, (pointer)&ret_val );
		break;
	case IPC_RMID:
		evaluate( &Tsemctl0, (pointer)&ret_val );
		if( success0( &ret_val ) )
			sv_del_ipc_id( semid, SV_SEM );
		break;
	}
	return( ret_val );
}
