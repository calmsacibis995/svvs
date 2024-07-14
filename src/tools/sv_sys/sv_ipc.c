# include <svvs.h>
# include <sv_env.h>
# include <sys/types.h>
# include <sys/stat.h>

# ifdef	UNDEF
# ident	"@(#)sv_ipc.c	1.3"
# endif

# ifdef SV_IPC
# include <sys/ipc.h>
# endif

# ifdef MSG
# include <sys/msg.h>
# endif

# ifdef SEM
# include <sys/sem.h>
# endif

# ifdef SHM
# include <sys/shm.h>
# endif

# include <fcntl.h>

# ifndef SYSVR_2
# include <unistd.h>
# endif


 
#ifndef SYSVR_2
# define 	MODE	( S_IXOTH | S_IWOTH | S_IROTH | S_IXGRP | S_IWGRP | S_IRGRP | S_IEXEC | S_IWRITE | S_IREAD )
# else
# define	MODE	0777
# endif

#ifdef	SV_IPC
static char *ipcfile() {
	static	char	path[ 64 ];
	int	olderrno = errno;

	sprintf( path, "%s/tmp/%s", SVVSPATH, SV_IPCFILE );
	errno = olderrno;
	return( path );
}
sv_add_ipc_id( id, typ )
int id;
int typ;
{

	int fd;
	struct ipc_id	ipc_id;
	int	olderrno = errno;

	if(( fd = open( ipcfile(), O_RDWR | O_CREAT, MODE )) < 0 ) {
		errno = olderrno;
		return;
	}
	while( read( fd, &ipc_id, IPCSIZE ) == IPCSIZE ) {
		if( ipc_id.id == id && ipc_id.typ == typ ) {
			close( fd );
			errno = olderrno;
			return;
		}
	}
	ipc_id.id = id;
	ipc_id.typ = typ;
	write( fd, &ipc_id, IPCSIZE );
	close( fd );
	errno = olderrno;
}
sv_del_ipc_id( id, typ )
int id;
int typ;
{
	long	lseek();
	long	loc;

	struct	ipc_id	ipc_id;
	int	fd;
	int	olderrno = errno;

	if(( fd = open( ipcfile(), O_RDWR )) < 0 ) {
		errno = olderrno;
		return;
	}
	loc = 0l;
	while( read( fd, &ipc_id, IPCSIZE ) == IPCSIZE ) {
		if( ipc_id.id == id && ipc_id.typ == typ ) {
			if( lseek( fd, (long)-IPCSIZE, SEEK_CUR ) == loc ){
				ipc_id.id = 0;
				ipc_id.typ = 0;
				write( fd, &ipc_id, IPCSIZE );
			}
			break;
		}
		loc += (long)IPCSIZE;
	}
	errno = olderrno;
	close( fd );
}
sv_clr_ipc_file() {
	unlink( ipcfile() );
}
#endif
