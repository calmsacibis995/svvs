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

#ident	"@(#)semctl.c	1.8"


# ifdef SEM

union Semun
{
	int			val;
	struct	semid_ds	*buf;
	ushort			*array;
};

typedef	union Semun	SEMUN;

# endif

# define	ALLPERM	0666
# define	NOGPERM	0606
# define	NOPERM	0
# define	NSEMS	8	/* number of semaphores in a semaphore array */

/*
** semctl
**	Test the semaphore operations routine.
*/
main()
{
# ifdef SEM
	int			ret_val;
	int			i;
	SEMUN			arg;
	key_t			key;
	int			valid_sem;
	int			valid_array;
	int			noperm_sem;
	int			noperm_array;
	ushort			array[NSEMS];
	struct	semid_ds	buf[NSEMS];
	struct	semid_ds	sem_ds;
	int			pid;
	int			cpid,wpid,sts;
	extern	char	*err_lookup ();

setup("semctl");
funct("semctl");

testing("Setting up the initial semaphores.");
/*  testing("normal return values from semget.");  */
start
	/* 
	 *  If this fails, abort the test since no queues are available
	 */
	exit_on (ERROR | SEVERR);

	expecting (VOID);
	pid = getpid();

	key = (key_t) 1024;
	if ( (valid_sem = semget(key,1,IPC_CREAT|ALLPERM)) == -1 )
		error ("can not create a valid semaphore with a key of 1024");

	key = (key_t) 1025;
	if ( (valid_array = semget(key,NSEMS,IPC_CREAT|ALLPERM)) == -1 )
		error ("can not create a valid semaphore array with a key of 1025");

	key = (key_t) 1026;
	if ( (noperm_sem = semget(key,1,IPC_CREAT|ALLPERM)) == -1 )
		error ("can not create a valid read only semaphore with a key of 1026");

	key = (key_t) 1027;
	if ( (noperm_array = semget(key,NSEMS,IPC_CREAT|NOPERM)) == -1 )
		error ("can not create a valid read only semaphore array with a key of 1027");
finis


testing("Using IPC_SET to change the owner and permission");
testing("of the initial semaphores");
testing("should return 0, with the uid,gid and mode changed.");
start
	expecting (VOID);
	/*
	 *  set the UID to something that isn't BIN, SVVS nor ROOT
	 */
	if ((sem_ds.sem_perm.uid = geteuid() + 5) == BIN_UID)
		sem_ds.sem_perm.uid += 1;
	sem_ds.sem_perm.gid = getegid() + 4;

/*
 *	Fill in cuid and cgid fields, even though these should not
 *	be set by IPC_SET.  If they are set, then it will be to
 *	predictable values.
 */
	sem_ds.sem_perm.cuid = geteuid() + 7;
	sem_ds.sem_perm.cgid = getegid() + 6;
	sem_ds.sem_perm.mode = NOPERM;
	arg.buf = &sem_ds;

	expecting (SUCCESS);
	semctl(noperm_sem,0,IPC_SET,arg);
finis

#ifndef	SYSVR_2

testing ("A second IPC_SET should work because the CUIDs match");
start
	expecting (SUCCESS);
	semctl(noperm_sem,0,IPC_SET,arg);
finis

#endif


testing("semctl with an invalid semid.");
testing("should return -1, with errno set to EINVAL.");
start
	expecting (-1);
	expecting (EINVAL);
	semctl(-1,1,GETVAL,arg);
finis


testing("Passing a semnum less than 0");
testing("should return -1, with an errno of EINVAL.");
start
	expecting (-1);
	expecting (EINVAL);
	semctl(valid_sem,-1,GETVAL,arg);
finis


testing("Passing a semnum that is greater than sem_nsems");
testing("should return -1, with an errno of EINVAL.");
start
	expecting (-1);
	expecting (EINVAL);
	semctl(valid_array,NSEMS + 5,GETVAL,arg);
finis

testing("SETVAL beyond the maximum allowed");
testing("should return -1, with an errno of ERANGE.");
start
	arg.val = INT_MAX;

	expecting (-1);
	expecting (ERANGE);
	semctl(valid_sem,0,SETVAL,arg);
finis


testing("SETALL beyond the maximum allowed");
testing("should return -1, with an errno of ERANGE.");
start
	for ( i = 0 ; i < NSEMS ; i++ )
		array[i] = USHRT_MAX;

	arg.array = array;

	expecting (-1);
	expecting (ERANGE);
	semctl(valid_array,NSEMS,SETALL,arg);
finis


testing("Use SETVAL on a semaphore, then use GETVAL to read that value");
testing("should return 0 from the SETVAL, and the GETVAL return the");
testing("value we just set.");
start
	arg.val = 5;

	expecting (SUCCESS);
	semctl(valid_sem,0,SETVAL,arg);

	expecting (5);
	semctl(valid_sem,0,GETVAL,arg);
finis


testing("Using SETALL on an array of semaphores, then GETALL to");
testing("read back the values");
testing("should return 0 from the SETALL, and GETALL, and the");
testing("semaphore values back.");
start
	for ( i = 0 ; i < NSEMS ; i++ )
		array[i] = 5;
	arg.array = array;

	expecting (SUCCESS);
	semctl(valid_array,NSEMS,SETALL,arg);
	for ( i = 0 ; i < NSEMS ; i++ )
		array[i] = 8;

	expecting (SUCCESS);
	semctl(valid_array,NSEMS,GETALL,arg);

	for ( i = 0 ; i < NSEMS ; i++ )
	{
		if ( array[i] != 5 )
		{
			error("%s, array [%d] = %d, 5 was expected",
				"semctl(GETALL) did not set the right values",
				i, array[i]);
			break;
		}
	}
finis


testing("SETVAL on a semaphore on which we do not have write permission");
testing("should return -1, with an errno of EACCES.");
start
	arg.val = 7;

	expecting (-1);
	expecting (EACCES);
	semctl(noperm_sem,0,SETVAL,arg);
finis


testing("Using GETPID on a semaphore");
testing("should return the current process id.");
start
	expecting (VOID);
	ret_val = semctl(valid_sem,0,GETPID,arg);
	if ( pid != ret_val )
		warning ("%s\n\
semctl (valid_sem, 0, GETPID, arg) returned %d, %d was expected",
		ADVISORY, ret_val, pid);
finis


testing("Using GETPID on a semaphore on which we do not have read permission");
testing("should return -1, with an errno of EACCES.");
start
	expecting (-1);
	expecting (EACCES);
	semctl(noperm_sem,0,GETPID,arg);
	/*****  This may be needed
	if ( ret_val != -1 )
		error( ADVISORY,WARNING,pid,ret_val,0);
	else
		if ( errno != EACCES )
			errore("semctl did not set the errno correctly",ERROR,EACCES);
	*****/
finis


testing("Using GETNCNT on a semaphore should return 0.");
start
	expecting (0);
	semctl(valid_sem,0,GETNCNT,arg);
finis


testing("Using GETNCNT on a semaphore on which we do not have read permission");
testing("should return -1, with an errno of EACCES.");
start
	expecting (-1);
	expecting (EACCES);
	semctl(noperm_sem,0,GETNCNT,arg);
finis


testing("Using GETZCNT on a semaphore should return 0.");
start
	expecting (0);
	semctl(valid_sem,0,GETZCNT,arg);
finis


testing("Using GETZCNT on a semaphore on which we do not have read permission");
testing("should return -1, with an errno of EACCES.");
start
	expecting (-1);
	expecting (EACCES);
	semctl(noperm_sem,0,GETZCNT,arg);
finis


testing("Using GETALL on an array of semaphores on which we do not have");
testing("read permission should return -1, with an errno of EACCES.");
start
	for ( i = 0 ; i < NSEMS ; i++ )
		array[i] = 5;
	arg.array = array;

	expecting (-1);
	expecting (EACCES);
	semctl(noperm_array,NSEMS,GETALL,arg);
finis


testing("Using SETALL on an array of sem's on which we do not have");
testing ("write permission should return -1, with an errno of EACCES.");
start
	for ( i = 0 ; i < NSEMS ; i++ )
		array[i] = 5;
	arg.array = array;

	expecting (-1);
	expecting (EACCES);
	semctl(noperm_array,NSEMS,SETALL,arg);
finis


testing("Using IPC_STAT on a group of semaphores");
testing("should return 0, and the status info.");
start
	arg.buf = buf;

	expecting (0);
	semctl(valid_array,NSEMS,IPC_STAT,arg);

	expecting (VOID);  /*  getuid cannot fail  */

	/*
	 *  report all errors
	 */
	continue_on (ERROR);

	i = 0;
	if ( buf[i].sem_perm.uid != getuid() )
		error("sem_perm.uid is %d, %d was expected",
			buf[i].sem_perm.uid, getuid());

	if ( buf[i].sem_perm.gid != getgid() )
		error("sem_perm.gid is %d, %d was expected",
			buf[i].sem_perm.gid, getgid());

	buf[i].sem_perm.mode &= 0777;
	if ( buf[i].sem_perm.mode != ALLPERM )
		error("sem_perm.mode is %o, %o wasexpected",
			buf[i].sem_perm.mode, ALLPERM);

	if ( buf[i].sem_perm.cuid != getuid() )
		error("sem_perm.cuid is %d, %d was expected",
			buf[i].sem_perm.cuid, getuid());

	if ( buf[i].sem_perm.cgid != getgid() )
		error("sem_perm.cgid is %d, %d was expected",
			buf[i].sem_perm.cgid, getgid());

	if ( buf[i].sem_nsems != NSEMS )
		error("sem_nsems is %d, %d was expected",
			buf[i].sem_nsems, NSEMS);
finis


testing("IPC_STAT on a group of semaphores we");
testing("do not have read permission on");
testing("should return -1, with an errno of EACCES.");
start
	arg.buf = buf;

	expecting (-1);
	expecting (EACCES);
	semctl(noperm_array,NSEMS,IPC_STAT,arg);
finis


testing("Using IPC_SET on a group of semaphores");
testing("should return 0, with uid,gid and mode set.");
start
	expecting (VOID);  /*  getuid cannot fail  */
	for ( i = 0 ; i < NSEMS ; i++ )
	{
		buf[i].sem_perm.uid = getuid();
		buf[i].sem_perm.gid = getgid() + 7;
		buf[i].sem_perm.mode = NOGPERM;
	}
	arg.buf = buf;

	expecting (0);
	semctl(valid_array,NSEMS,IPC_SET,arg);

	arg.buf = buf;
	semctl(valid_array,NSEMS,IPC_STAT,arg);


	/*
	 *  report all errors
	 */
	continue_on (ERROR);

	i = 0;
	expecting (VOID);  /*  getuid cannot fail  */
	if ( buf[i].sem_perm.uid != getuid() )
		error("sem_perm.uid is %d, %d was expected",
			buf[i].sem_perm.uid, getuid());

	if ( buf[i].sem_perm.gid != (getgid() + 7) )
		error("sem_perm.gid is %d, %d was expected",
			buf[i].sem_perm.gid, getgid());

	buf[i].sem_perm.mode &= 0777;
	if ( buf[i].sem_perm.mode != NOGPERM )
		error("sem_perm.mode is %d, %d was expected",
			buf[i].sem_perm.mode, NOGPERM);

	if ( buf[i].sem_perm.cuid != getuid() )
		error("sem_perm.cuid is %d, %d was expected",
			buf[i].sem_perm.cuid, getuid());

	if ( buf[i].sem_perm.cgid != getgid() )
		error("sem_perm.cgid is %d, %d was expected",
			buf[i].sem_perm.cgid, getgid());

	if ( buf[i].sem_nsems != NSEMS )
		error("sem_nsems is %d, %d was expected",
			buf[i].sem_nsems, NSEMS);
finis


testing("Removing a semaphore");
testing("should return 0, with the semaphore deleted.");
start
	expecting (0);
	semctl(valid_sem,0,IPC_RMID,arg);

	expecting (VOID);  /*  give a better diagnostic  */
	key = (key_t) 1024;
	ret_val = semget(key,1,ALLPERM);
	if ( ret_val != -1 )
		error ("semctl did not remove the semaphore");
	else
		if ( errno != ENOENT )
			error ("ERRNO = %s, ENOENT was expected",
				err_lookup (errno) );
finis


testing("Removing a semaphore on which the UID and CUID don't match");
testing("should return -1, with an errno of EPERM.");
start
	expecting (SUCCESS);
	cpid = fork();
	if ( cpid == (-1) ) 
	{
		error ("semctl: cannot fork");
	}
	if ( cpid == 0 ) 
	{
		expecting (SUCCESS);
		execlp( "./semctl1", "./semctl1", (char *)0 );
		error ( "semctl: cannot exec" );
	}

	expecting (cpid);
	wpid = wait(&sts);

	if ( sts != 0 )
	{
		error( "semctl: subtest terminated abnormally with status %o",
			sts);
	}
	/*
	** Make sure the semaphores are removed from the system.
	*/

	expecting (VOID);
	semctl(valid_sem,0,IPC_RMID,arg);
	semctl(valid_array,NSEMS,IPC_RMID,arg);
	semctl(noperm_sem,0,IPC_RMID,arg);
	semctl(noperm_array,NSEMS,IPC_RMID,arg);
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
