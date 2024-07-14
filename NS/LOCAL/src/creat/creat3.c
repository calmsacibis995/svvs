#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef		SYSVR_2
#include	<unistd.h>

#ident	"@(#)creat3.c	1.3"

#define	PROT	S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP | S_ENFMT
#define	NPROT	S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP

main(argc,argv)
int	argc;
char	*argv[];
{
	int		pid,fd;
	char	path[512];

	setup("creat");
	funct("creat");

	abstract("This tests creat() when used on a file with record");
	abstract("locking in effect.  In enforcement mode the attempt to");
	abstract("create an existing locked file should fail, in advisory");
	abstract("mode it should succeed.");
	start
		strcpy(path, temp_path());
		strcat(path, "creat_enf");
		preparation("Create %s with enforcement mode record locking",path);
		preparation("set for the entire file.");
		start
			fd = creat(path,0);
			chmod(path,PROT);
			lockf(fd,F_LOCK,0L);
		finis
		preparation("Fork a child to try to creat %s.",path);
		start
			if(!(pid = fork())){
				testing("Attempt to create %s which is",path);
				testing("locked, this should result in a failure");
				testing("with errno equal to EAGAIN.");
				start
					expecting(EAGAIN);
					creat(path,0);
				finis
				exit();
			}
			wait((int *)0);
			close(fd);
		finis
		strcpy(path, temp_path());
		strcat(path, "creat_noenf");
		preparation("Create %s with advisory record locking",path);
		preparation("set for the entire file.");
		start
			fd = creat(path,0);
			chmod(path,NPROT);
			lockf(fd,F_LOCK,0L);
		finis
		preparation("Fork a child to try to creat %s.",path);
		start
			if(!(pid = fork())){
				testing("Attempt to create %s which is",path);
				testing("locked, this should succeed.");
				start
					creat(path,0);
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
