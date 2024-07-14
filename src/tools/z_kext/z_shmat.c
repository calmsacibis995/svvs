/*LINTLIBRARY*/
/*
 * z_shmat.c
 */

#ifdef	UNDEF
#ident	"@(#)z_shmat.c	1.2"
#endif

#include <sv_env.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#ifndef shmat
char *shmat();
#endif
char	*strcpy();

static int success( val )
char **val;
{
	return( ((*val) && (*val) != (char *)(-1)) ? 1 : 0 );
}

static int failure( val )
char **val;
{
	return( (*val) == (char *)(-1) );
}

static char *format( val )
char **val;
{
	static char buf[STRVAL];

	if( !*val )
		(void) strcpy( buf, "NULL" );
	else if( (pointer)(*val) == NON_NULL )
		(void) strcpy( buf, "NON-NULL" );
	else
		(void) sprintf( buf, "0x%lx", (long)(*val) );
	return( buf );
}

static int compare( a, b )
char **a;
char **b;
{
	if( (pointer)(*b) == NON_NULL )
		return( (*a)? 1 : 0 );
	return( *a == *b );
}

pointer *sv_getp();

static struct ftable Tshmat = {
	"shmat",
	"a return value other than NULL or (-1)",
	"a return value of (-1)",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_getp,
	1,
};

char * Zshmat(shmid, shmaddr, shmflg)
int shmid;
char *shmaddr;
int shmflg;
{
	char * ret_val;

	errno = 0;
	ret_val = shmat(shmid, shmaddr, shmflg);
	evaluate( &Tshmat, (pointer)&ret_val );
	return( ret_val );
}

