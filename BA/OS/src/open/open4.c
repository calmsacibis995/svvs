#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef		SYSVR_2
#include	<unistd.h>
#include	<fcntl.h>

#ident	"@(#)open4.c	1.3"

#define	PROT	S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_ENFMT
#define	NPROT	S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

/*
 *  This test is identical to creat3.c,
 *  substituting "open (path, O_RDWR | O_TRUNC | O_CREAT);" for the creat
 */


main(argc,argv)
int	argc;
char	*argv[];
{
	int		pid,fd;
	char	path[512];
	struct	flock	flock;

	setup("open");
	funct("open");

	abstract("This tests open() when attempting to truncate a file");
	abstract("with record locking in effect");
	abstract("In enforcement mode the attempt to");
	abstract("open an existing locked file should fail, in advisory");
	abstract("mode it should succeed.");
	start
		strcpy(path, temp_path());
		strcat(path, "open_enf");
		preparation("Create %s with enforcement mode record locking",path);
		preparation("with the entire file write locked.");
		start
			expecting (VOID);
			umask (0);
			expecting (SUCCESS);
			fd = open (path, O_RDWR | O_TRUNC | O_CREAT, PROT);
			lockf(fd,F_LOCK,0L);
		finis
		preparation("Fork a child to try to open %s.",path);
		start
			if(!(pid = fork())){
				testing("Attempt to open %s which is",path);
				testing("locked, this should result in a failure");
				testing("with errno equal to EAGAIN.");
				start
					expecting(EAGAIN);
					open (path, O_RDONLY | O_TRUNC);
				finis
				exit();
			}
			wait((int *)0);
		finis
		prep ("remove the write lock and read lock the entire file");
		start
			expecting (SUCCESS);
			lockf(fd,F_ULOCK,0L);
			/*  fcntl with type = F_UNLCK should work too  */

			flock.l_type = F_RDLCK;
			flock.l_whence = 0;
			flock.l_start = 0L;
			flock.l_len = 0L;
			expecting (SUCCESS);
			fcntl (fd, F_SETLK, &flock);
		finis
		preparation("Fork a child to try to open %s.",path);
		start
			if(!(pid = fork())){
				testing("Attempt to open %s which is",path);
				testing("locked, this should result in a failure");
				testing("with errno equal to EAGAIN.");
				start
					expecting(EAGAIN);
					open (path, O_RDONLY | O_TRUNC);
				finis
				exit();
			}
			wait((int *)0);
			close(fd);
		finis
		strcpy(path, temp_path());
		strcat(path, "open_noenf");
		preparation("Create %s with advisory record locking",path);
		preparation("set for the entire file.");
		start
			fd = creat(path,0);
			chmod(path,NPROT);
			lockf(fd,F_LOCK,0L);
		finis
		preparation("Fork a child to try to open %s.",path);
		start
			if(!(pid = fork())){
				testing("Attempt to open %s which is",path);
				testing("locked, this should succeed.");
				start
					open (path, O_RDONLY | O_TRUNC);
				finis
				exit();
			}
			wait((int *)0);
			close(fd);
		finis
	finis
	done();
}

#else

main(argc,argv)
int		argc;
char	*argv[];
{
	unsupported("Enforcement mode record/file locking is not supported by System V Release 2");
	done();
}
#endif
