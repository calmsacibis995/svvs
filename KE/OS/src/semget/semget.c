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

#ident	"@(#)semget.c	1.5"


# define	ALLPERM	0666
# define	NOPERM	0
# define	NSEMS	8	/* number of semaphores in a semaphore array */

# define	KEYVAL	3034
# define	NEWVAL	4044
# ifdef SEM
union Semun
{
	int			val;
	struct	semid_ds	*buf;
	ushort			*array;
};

typedef	union Semun	SEMUN;
# endif
/*
** semget
**	Test the semget routine.
*/
main()
{
#ifdef SEM
	int			ret_val;
	int			semval;
	int			excl_sem;
	int			noperm_sem;
	int			i;
	int			semarray[500];
	SEMUN			arg;
	struct	semid_ds	sem_ds;
	key_t			key;
	extern			char *err_lookup ();

setup("semget");
funct("semget");

testing("creating a semaphore.");
testing("should return the value of the semaphore, with the stat structure set.");
start
	/*
	 *  abort the test if this fails,
	 *  since all subsequent cases will probably fail
	 */
	exit_on (ERROR);

	key = (key_t) KEYVAL;

	expecting (SUCCESS);
	semval = semget(key,NSEMS,IPC_CREAT|IPC_EXCL|ALLPERM);

	arg.buf = &sem_ds;
	ret_val = semctl(semval,0,IPC_STAT,arg);
	
	expecting (VOID);  /*  getuid never fails  */
	if ( sem_ds.sem_perm.uid != getuid() )
	{
		error("sem_perm.uid = %d, %d was expected",
			sem_ds.sem_perm.uid, getuid());
	}
	if ( sem_ds.sem_perm.gid != getgid() )
	{
		error("sem_perm.gid = %d, %d was expected",
			sem_ds.sem_perm.gid, getgid());
	}
	sem_ds.sem_perm.mode &= 0777;
	if ( sem_ds.sem_perm.mode != ALLPERM )
	{
		error("sem_perm.mode = %o, %o was expected",
			sem_ds.sem_perm.mode, ALLPERM);
	}
	if ( sem_ds.sem_perm.cuid != getuid() )
	{
		error("sem_perm.cuid = %d, %d was expected",
			sem_ds.sem_perm.cuid, getuid());
	}
	if ( sem_ds.sem_perm.cgid != getgid() )
	{
		error("sem_perm.cgid = %d, %d was expected",
			sem_ds.sem_perm.cgid, getgid());
	}
	if ( sem_ds.sem_nsems != NSEMS )
	{
		error("sem_nsems = %d, %d was expected",
			sem_ds.sem_nsems, NSEMS);
	}
finis


testing("Passing a value of nsems that is less then 0");
testing("should return -1, with an errno of EINVAL.");
start
	key = (key_t) 2025;

	expecting (-1);
	expecting (EINVAL);
	semget(key,-1,IPC_CREAT|ALLPERM);
finis


testing("Passing a value of nsems that is greater than the maximum");
testing("should return -1, with an errno of EINVAL.");
start
	key = (key_t) 2025;

	expecting (-1);
	expecting (EINVAL);
	semget(key,INT_MAX,IPC_CREAT|ALLPERM);
finis


testing("Asking for an existing semaphore that has fewer keys then nsems");
testing("should return -1, with an errno of EINVAL.");
start
	key = (key_t) KEYVAL;

	expecting (-1);
	expecting (EINVAL);
	semget(key,NSEMS * 2,ALLPERM);
finis


testing("Asking for a nonexistent semaphore");
testing("should return -1, with an errno of ENOENT.");
start
	key = (key_t) 2029;

	expecting (-1);
	expecting (ENOENT);
	semget(key,NSEMS,ALLPERM);
finis


testing("Trying to create an existing semaphore");
testing("should return -1, with an errno of EEXIST.");
start
	key = (key_t) KEYVAL+1;

	expecting (SUCCESS);
	excl_sem = semget(key,NSEMS,IPC_CREAT|IPC_EXCL|ALLPERM);

	/* create another one now */
	expecting (-1);
	expecting (EEXIST);
	ret_val = semget(key,NSEMS,IPC_CREAT|IPC_EXCL|ALLPERM);

	expecting (SUCCESS);
	semctl(excl_sem,NSEMS,IPC_RMID,arg);
finis


testing("Asking for a semaphore for which we do not have permission");
testing("should return -1, with an errno of EACCES.");
start
	break_on (ERROR | SEVERR);
	/*  first create the semaphore  */
	key = (key_t) (KEYVAL * 2);

	expecting (SUCCESS);
	noperm_sem = semget(key,NSEMS,IPC_CREAT|ALLPERM);

	/*  change the permissions  */
	expecting (VOID);  /*  getuid never fails  */
	sem_ds.sem_perm.uid = getuid() + 20;
	sem_ds.sem_perm.mode = NOPERM;
	arg.buf = &sem_ds;

	expecting (SUCCESS);
	ret_val = semctl(noperm_sem,NSEMS,IPC_SET,arg);

	/*  now, try to get it */

	expecting (-1);
	expecting (EACCES);
	semget(key, NSEMS,ALLPERM);

	/* delete the semaphore  */
	expecting (SUCCESS);
	semctl(noperm_sem,NSEMS,IPC_RMID,arg);
finis


testing("Creating more semaphores than the system allows");
testing("should return -1, with an errno of ENOSPC.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( i = 0 ; i < 500 ; i++ )
	{
		key = (key_t) (NEWVAL + (20 * i));
		ret_val = semget(key,NSEMS,IPC_CREAT|ALLPERM);
		if ( ret_val == -1 )
			break;
		semarray[i] = ret_val;
	}

	if ( ret_val != -1 )
		error("semget never returned -1");
	if ( errno != ENOSPC )
		error ("%s\nreturning %s when ENOSPC was expected",
			"semget did not set errno correctly",
			err_lookup (errno));
	i--;

	expecting (SUCCESS);
	while ( i >= 0 )
	{
		semctl(semarray[i],NSEMS,IPC_RMID,arg);
		i--;
	}

	expecting (VOID);
	semctl(semval,NSEMS,IPC_RMID,arg);
finis

done();
/*NOTREACHED*/
#else
	void exit();
	
	unsupported ("Semaphore subsystem not a supported kernel extension.");
	exit(0);
/*NOTREACHED*/

#endif
}
