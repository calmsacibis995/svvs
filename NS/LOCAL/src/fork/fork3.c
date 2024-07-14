#ident	"@(#)fork3.c	1.3"
#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#include	<fcntl.h>
#ifdef	LOCKF
#include	<unistd.h>
#endif
#include	<sys/stat.h>

#define	MODE	S_IWRITE | S_IREAD | S_IWGRP | S_IRGRP
#define	DIE	10		/*Child 1 should die after 10 seconds*/

/*
** fork - Create a new process
**
** File locks in effect in the parent are not inherited by the child.
**
*/
#ifndef	SYSVR_2
main(argc,argv)
int		argc;
char	*argv;
{
	int		fd;
	char	path[512];

	setup("fork");
	funct("fork");

	abstract("This test tests whether record/file locks are inherited");
	abstract("by children when they are set in the parent.  It consists");
	abstract("of a parent process which will set a lock on a file and");
	abstract("release it, and two children, one which is forked");
	abstract("immediately after the lock is put on the file and one");
	abstract("which is forked immediately after the lock is removed.");
	abstract("The second child will check to see if the file locked by");
	abstract("the parent is still locked.  If it is, the first child");
	abstract("inherited the lock, which it should not have done.");
	start
		preparation("Create a temporary file.");
		start
			strcpy(path,temp_path());
			strcat(path,"fork_lockf");
			expecting(SUCCESS);
			fd = creat(path,MODE);
		finis
		preparation("Lock the temporary file.");
		start
			(void)lockf(fd,F_LOCK,0L);
		finis
		child1(fd);
		preparation("Release the temporary file.");
		start
			expecting(SUCCESS);
			(void)lockf(fd,F_ULOCK,0L);
		finis
		child2(fd);
		wait((int *)0);
		wait((int *)0);
	finis
	done();
}

child1(fd)
int		fd;
{
	time_t	time(),die;

	preparation("Fork a child to hold any locks it may have");
	preparation("inherited for %d seconds.",DIE);
	start
		if(!fork()){
			die = time((time_t *)0) + DIE;
			while(time((time_t *)0) < die);
			exit(0);
		}
	finis
}

child2(fd)
int		fd;
{
	preparation("Fork a child and test for locks on the file the");
	preparation("parent just unlocked.  If the test fails with EACCES");
	preparation("(the file is still locked) then child #1 must have");
	preparation("inherited the file lock.");
	start
		if(!fork()){
			expecting(SUCCESS);
			(void)lockf(fd,F_TEST,0L);
			exit();
		}
	finis
}
#else
main(argc,argv)
int		argc;
char	*argv[];
{
	unsupported("Effects of fork() on file locks not defined in SVID for System V Release 2");
	done();
}
#endif
