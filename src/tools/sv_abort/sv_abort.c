/*
 *	sv_abort.c - abort mechanism for svvs tests, kills pgrp
 */

#ident	"@(#)sv_abort.c	1.2"

#include <sv_env.h>

#ifdef SV_IPC
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#endif

#ifdef MSG
#include <sys/msg.h>
#endif

#ifdef SEM
#include <sys/sem.h>
#endif

#ifdef SHM
#include <sys/shm.h>
#endif

#include <stdio.h>

main(argc, argv)
int argc;
char *argv[];
{
#ifdef	SV_IPC
	if( argc > 1 )
		sv_rm_ipc( argv[1] );
#endif
	if( kill(0,9) )
		printf("SEVERR:\n\tcould not abort test, kill failed\n");
	exit( SEVERR );
}
#ifdef SV_IPC
rm_id( ipc_id )
struct ipc_id *ipc_id;
{
	if( ipc_id->id < 0 ) return;

	switch( ipc_id->typ ) {
#ifdef	MSG
	case SV_MSG:
		msgctl( ipc_id->id, IPC_RMID, (struct msgqid_ds *)0 );
		break;
#endif
#ifdef	SEM
	case SV_SEM:
		semctl( ipc_id->id, 0, IPC_RMID, (struct semid_ds *)0 );
		break;
#endif
#ifdef	SHM
	case SV_SHM:
		shmctl( ipc_id->id, IPC_RMID, (struct shmid_ds *)0 );
		break;
#endif
	default:
		break;
	}
}
sv_rm_ipc( path ) 
char *path; 
{
	int	fd;
	char	file[ 512 ];
	struct ipc_id	ipc_id;

	sprintf( file, "%s/tmp/%s", path, SV_IPCFILE );
	if( (fd = open( file, O_RDONLY )) < 0 ) return;
	unlink( file );
	while( read( fd, &ipc_id, IPCSIZE ) == IPCSIZE )
		rm_id( &ipc_id );
	close( fd );
}
#endif
