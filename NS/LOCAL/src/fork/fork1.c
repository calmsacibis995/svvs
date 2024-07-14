#ident	"@(#)fork1.c	1.3"
#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

/*
** Fork - Create a new process
**
** When a process forks, the uid, gid, euid, and egid of the  parent
** process should be preserved.  This tests that aspect of fork.
**
** This is a setuid program
**
*/
#ifndef	SYSVR_2
main(argv,argc)
int	argc;
char	*argv[];
{
	unsigned	short	uid,gid,euid,egid;

	setup("fork");
	funct("fork");

	abstract("This tests whether the child of a fork() inherits");
	abstract("its parent's uid and gid as well as its parents");
	abstract("euid and egid.");
	start
		preparation("Find out the current settings of uid, gid, euid");
		preparation("and egid and fork a child.");
		start
			expecting(VOID);
			uid = getuid();
			euid = geteuid();
			gid = getgid();
			egid = getegid();
			if(!fork()){
				testing("Check uid, euid, gid and egid of child");
				start
					exit_on(ERROR);
					expecting(uid);
					(void)getuid();
					expecting(euid);
					(void)geteuid();
					expecting(gid);
					(void)getgid();
					expecting(egid);
					(void)getegid();
				finis
				prep("Change the uid, gid, euid and egid of the child");
				prep("and fork a grandchild, to see if the changes are");
				prep("retained.");
				start
					exit_on(ERROR);
					expecting(SUCCESS);
					setgid(GID);
					setuid(BIN_UID);	
					expecting(VOID);
					uid = getuid();
					euid = geteuid();
					gid = getgid();
					egid = getegid();
					if(!fork()){
						testing("Check uid, euid, gid and egid of child");
						start
							exit_on(ERROR);
							expecting(uid);
							(void)getuid();
							expecting(euid);
							(void)geteuid();
							expecting(gid);
							(void)getgid();
							expecting(egid);
							(void)getegid();
						finis
						exit(0);
					}
					expecting(SUCCESS);
					wait((int *)0);
				finis
				exit(0);
			}
			wait((int *)0);
		finis
	finis
	done();
}
#else
main(argc,argv)
int		argc;
char	*argv;
{
	unsupported("uid, gid, euid, egid after a fork() not defined in SVID for System V Release 2");
}
#endif
