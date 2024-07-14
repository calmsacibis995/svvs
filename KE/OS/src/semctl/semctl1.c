#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
# ifdef SEM
# include	<sys/types.h>
# include	<sys/ipc.h>
# include	<sys/sem.h>
# endif

#ident	"@(#)semctl1.c	1.6"


# define	NOPERM	0
# ifdef SEM
union Semun
{
	int			val;
	struct	semid_ds	*buf;
	ushort			*array;
};

typedef	union Semun	SEMUN;
# endif
main() {
#ifdef SEM
	int			ret_val;
	SEMUN			arg;
	key_t			key;
	int			noperm_sem;
	struct	semid_ds	sem_ds;

setup("semctl");
/*
 *  This program needs to be set user-id to something that is
 *  NOT root, svvs, nor svvs + 5 (or svvs + 6 if that equals BIN_UID)
 *  so that the permissions will always
 *  be false for the IPC_RMID.
 *  BIN is a good ID to use.
 *
 *  You can do an IPC_SET or IPC_RMID
 *  if the effective UID is 0, or == sem_perm.uid, or ==sem_perm.cuid,
 *  thus this test cannot be done in the parent process.
 *  (You can't change your effective UID to anything other than your
 *  real UID unless you're ROOT and there's no reason for super user permission
 *  other than changing UID.  In fact, we don't want root privileges
 *  in this test).
 *
 *  If all three calls succeed, then check if this executable is
 *  owned by bin and has the set-user-id bit set (mode 04555).
 */

testing ("A semget for a key of 1026 should succeed in a child process");
testing ("proving that it is not private");
start
	key = (key_t) 1026;

	expecting (SUCCESS);
	noperm_sem = semget(key,1,NOPERM);
finis


#ifndef	SYSVR_2

testing ("Using IPC_SET on semaphore 1026 should fail since the");
testing ("UID and CUID don't match.");
start
	expecting (VOID);
	/*
	 *  set the UID to something that isn't BIN, SVVS nor ROOT
	 */
	if ((sem_ds.sem_perm.uid = geteuid() + 5) == BIN_UID)
		sem_ds.sem_perm.uid += 1;
	sem_ds.sem_perm.gid = getegid() + 4;

/*	Fill in the cuid and cgid fields, even though these should not
 *	be set by IPC_SET.  If they are set, then it will be to 
 *	predictable values.
 */
	sem_ds.sem_perm.cuid = geteuid() + 7;
	sem_ds.sem_perm.cgid = getegid() + 6;
	sem_ds.sem_perm.mode = NOPERM;
	arg.buf = &sem_ds;

	expecting (-1);
	expecting (EPERM);
	semctl(noperm_sem,0,IPC_SET,arg);
finis

#endif


testing ("A semctl to remove semaphore 1026 should fail since the");
testing ("UID and CUID don't match.");
start
	expecting (-1);
	expecting (EPERM);
	semctl(noperm_sem,0,IPC_RMID,arg);
finis

done();
/*NOTREACHED*/

#endif
}
