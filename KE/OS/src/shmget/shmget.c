#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
# ifdef	SHM

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

# include	<fcntl.h>
# include	<sys/types.h>
# include	<sys/stat.h>
# include	<sys/ipc.h>
# include	<sys/shm.h>
# endif

#ident	"@(#)shmget.c	1.9"



#ifndef SYSVR_2
# define 	WORLD	(S_IROTH | S_IWGRP | S_IRGRP | S_IWUSR | S_IRUSR)
# define 	ALLPERM	(S_IWOTH | S_IROTH | S_IWGRP | S_IRGRP | S_IWUSR | S_IRUSR)
# else
# define	WORLD	0664
# define	ALLPERM	0666
# endif

# define	KEYVAL	4321

/*
** shmget
**	Test the shared memory routine.
*/
main()
{
#ifdef SHM
	int			ret_val;
	int			shmid;
	int			shared;
	int			smin, smax;
	int			pid;
	int			parentpid;
	int			parentuid;
	char			*shmaddr;
	int			i;
	int			fd;
	key_t			key;
	struct	shmid_ds	buf;
	extern	char		*shmat();
	extern	char		*err_lookup();

setup("shmget");
funct("shmget");

testing("Find the system imposed minimum segment size");
testing("and create a segment of that size.");
/*  testing("A return != -1, and a segment to be created.");  */
start
	exit_on (ERROR);

	expecting (VOID);
	key = (key_t) KEYVAL;
	/*  try sizes in powers of two  */
	for(smin=1; smin; smin<<=1 )
	{
		shmid = shmget(key,smin,IPC_CREAT|ALLPERM);
		if ( shmid != -1 )
			break;
		if (errno != EINVAL)
			error ("errno = %s, EINVAL was expected",
				err_lookup (errno));
	}
	/*
	 *  abort if this fails
	 */
	exit_on (ERROR);

	if(!smin)
		error ("shmget could not create a segment");
finis


testing("Check that all the variables associated");
testing("with this shared memory segment are properly set");
testing("by returning a valid shmid_ds structure.");
start
	expecting (SUCCESS);
	shmctl(shmid,IPC_STAT,&buf);

	/*
	 *  report all errors
	 */
	continue_on (ERROR);

	expecting (VOID);  /*  getpid, getuid, getgid can never be wrong  */
	if ( buf.shm_segsz != smin )
		error ("shmget set shm_segsz to %d, %d was exepcted",
			buf.shm_segsz, smin);
			
	if ( buf.shm_cpid != getpid() )
		warning ("%s\nshmget set shm_cpid to %d, %d was expected",
			ADVISORY, buf.shm_cpid, getpid());

	if ( buf.shm_lpid != 0 )
		warning ("%s\nshmget set shm_lpid to %d, 0 was expected",
			ADVISORY, buf.shm_lpid);

	if ( buf.shm_nattch != 0 )
		error ("shmctl set shm_nattch = %d, 0 was exepected",
			buf.shm_nattch);

	if ( buf.shm_atime != 0 )
		error ("shmctl set shm_atime = %d, 0 was expected",
			buf.shm_atime);

	if ( buf.shm_dtime != 0 )
		error ("shmctl set shm_dtime = %d, 0 was expected",
			buf.shm_dtime);

	if ( buf.shm_perm.uid != geteuid() )
		error("shmctl set shm_perm.uid = %d, %d was expected",
			buf.shm_perm.uid, geteuid());

	if ( buf.shm_perm.gid != getgid() )
		error("shmctl set shm_perm.gid = %d, %d was expected",
			buf.shm_perm.gid, getgid());

	buf.shm_perm.mode &= 0777;
	if ( buf.shm_perm.mode != ALLPERM )
		error ("shmctl set shm_perm.mode = 0%o, 0%o was expected",
			buf.shm_perm.mode, ALLPERM);
finis


testing("Create a private segment, check the segment, then remove it.");
/*  testing("A return != -1, and a segment to be created, then deleted.");  */
start
	expecting (SUCCESS);

	key = (key_t) IPC_PRIVATE;
	shared = shmget(key,smin,IPC_CREAT|ALLPERM);

	shmctl(shared,IPC_STAT,&buf);

	if ( buf.shm_segsz != smin )
		error ("segment size = %d, %d was expected",
			buf.shm_segsz, smin);

	/*
	 *  report all errors
	 */
	continue_on (ERROR);

	expecting (VOID);  /*  getpid, getuid, getgid can never be wrong  */
	if ( buf.shm_cpid != getpid() )
		warning ("%s\nshm_cpid = %d, %d was expected",
			ADVISORY, buf.shm_cpid, getpid());

	if ( buf.shm_lpid != 0 )
		warning ("%s\nshm_lpid = %d, 0 was expected",
			ADVISORY, buf.shm_lpid);

	if ( buf.shm_nattch != 0 )
		error("shm_nattch = %d, 0 was expected", buf.shm_nattch);

	if ( buf.shm_atime != 0 )
		error("shm_atime = %d, 0 was expected", buf.shm_atime);

	if ( buf.shm_dtime != 0 )
		error("shm_dtime = %d, 0 was expected", buf.shm_dtime);

	if ( buf.shm_perm.uid != geteuid() )
		error("shm_perm.uid = %d, %d was expected",
			buf.shm_perm.uid, geteuid());

	if ( buf.shm_perm.gid != getgid() )
		error("shm_perm.gid = %d, %d was expected",
			buf.shm_perm.gid, getgid());

	buf.shm_perm.mode &= 0777;
	if ( buf.shm_perm.mode != ALLPERM )
		error("shm_perm.mode = 0%o, 0%o was exepcted",
			buf.shm_perm.mode, ALLPERM);

	expecting (SUCCESS);
	shmctl(shared,IPC_RMID,&buf);
finis


testing("Create a segment, have a subprocess get, and ctl on it.");
/*  testing("A return != -1, and a segment to be created.");  */
start
	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	parentpid = getpid();
	parentuid = geteuid();

	expecting (SUCCESS);
	key = (key_t) KEYVAL * 2;
	shared = shmget(key,smin,IPC_CREAT|ALLPERM);


	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");

	if ( pid == 0 )
	{
		exit_on (ERROR | SEVERR);
		expecting (SUCCESS);

		key = (key_t) KEYVAL * 2;
		shared = shmget(key,smin,ALLPERM);

		shmctl(shared,IPC_STAT,&buf);

		/*
		 *  report all errors
		 */
		continue_on (ERROR);

		if ( buf.shm_segsz != smin )
			error("segment size = %d, %d was expected",
				buf.shm_segsz, smin);

		if ( buf.shm_cpid != parentpid )
			error("creator id = %d, %d was expected",
				buf.shm_cpid, parentpid);

		if ( buf.shm_lpid != 0 )
			error("lpid = %d, 0 was expected", buf.shm_lpid);

		if ( buf.shm_nattch != 0 )
			error("nattch = %d, 0 was expected", buf.shm_nattch);

		if ( buf.shm_atime != 0 )
			error("atime = %d, 0 was expected", buf.shm_atime);

		if ( buf.shm_dtime != 0 )
			error("dtime = %d, 0 was expected", buf.shm_dtime);

		if ( buf.shm_perm.uid != parentuid )
			error("shm_perm.uid = %d, %d was expected",
				buf.shm_perm.uid, parentuid);

		expecting (VOID);
		if ( buf.shm_perm.gid != getgid() )
			error ("shm_perm.gid = %d, %d was expected",
				buf.shm_perm.gid, getgid());

		buf.shm_perm.mode &= 0777;
		if ( buf.shm_perm.mode != ALLPERM )
			error ("shm_perm.mode = 0%o, 0%o was expected",
				buf.shm_perm.mode, ALLPERM);

		exit(0);
	}
	finis

	expecting (VOID);
	ret_val = wait_for(pid,5);
	if ( ret_val )
	{
		error("subprocess died abnormally status = 0%o",
			ret_val);

	}

	expecting (SUCCESS);
	shmctl(shared,IPC_RMID,&buf);
finis


testing("Create a segment, have a subprocess with a different uid");
testing("do a get, and ctl on it.");
/*  testing("A return != -1, and a segment to be created.");  */
start
	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	parentpid = getpid();
	parentuid = geteuid();

	expecting (SUCCESS);
	key = (key_t) KEYVAL * 2;
	shared = shmget(key,smin,IPC_CREAT|ALLPERM);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");

	if ( pid == 0 )
	{
		expecting (SUCCESS);
		setuid(BIN_UID);

		key = (key_t) KEYVAL * 2;
		shared = shmget(key,smin,ALLPERM);

		shmctl(shared,IPC_STAT,&buf);

		/*
		 *  report all errors
		 */
		continue_on (ERROR);

		if ( buf.shm_segsz != smin )
			error("shm_segsz = %d, %d was expected",
				buf.shm_segsz, smin);

		if ( buf.shm_cpid != parentpid )
			error ("shm_cpid = %d, %d was expected",
				buf.shm_cpid, parentpid);

		if ( buf.shm_lpid != 0 )
			error ("shm_lpid = %d, 0 was expected", buf.shm_lpid);

		if ( buf.shm_nattch != 0 )
			error ("shm_nattch = %d, p was expected",
				buf.shm_nattch);

		if ( buf.shm_atime != 0 )
			error ("shm_atime = %d, 0 was expected",
				buf.shm_atime);

		if ( buf.shm_dtime != 0 )
			error ("shm_dtime = %d, 0 was expected",
				buf.shm_dtime);

		if ( buf.shm_perm.uid != parentuid )
			error ("shm_perm.uid = %d, %d was expected",
				buf.shm_perm.uid, parentuid);

		expecting (VOID);  /*  getuid, getgid can never be wrong  */
		if ( buf.shm_perm.gid != getgid() )
			error ("shm_perm.gid = %d, %d was expected",
				buf.shm_perm.gid, getgid());

		buf.shm_perm.mode &= 0777;
		if ( buf.shm_perm.mode != ALLPERM )
			error ("shm_perm.mode = 0%o, 0%o was exepcted",
				buf.shm_perm.mode, ALLPERM);

		exit(0);
	}
	finis

	expecting (VOID);
	ret_val = wait_for(pid,5);
	if ( ret_val )
	{
		error("subprocess died abnormally status = 0%o",
			ret_val);

	}


	expecting (SUCCESS);
	shmctl(shared,IPC_RMID,&buf);
	shmctl(shmid,IPC_RMID,&buf);
finis


testing("Create a segment, have a subprocess get, and shmat on it.");
/*  testing("A return != -1, and a segment to be created.");  */
start
	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	parentpid = getpid();
	parentuid = geteuid();

	expecting (SUCCESS);
	key = (key_t) KEYVAL * 2;
	shared = shmget(key,smin,IPC_CREAT|ALLPERM);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");

	if ( pid == 0 )
	{
		expecting (SUCCESS);
		key = (key_t) KEYVAL * 2;
		shared = shmget(key,smin,ALLPERM);
		shmaddr = shmat(shared,(char *) 0, 0);
		shmdt(shmaddr);
		exit(0);
	}
	finis

	expecting (VOID);
	ret_val = wait_for(pid,5);
	if ( ret_val )
	{
		error("subprocess died abnormally status = 0%o",
			ret_val);

	}

	expecting (SUCCESS);
	shmctl(shared,IPC_RMID,&buf);
finis


testing("Create a segment, have a subprocess with a different uid");
testing("do a get, and a shmat on it.");
/*  testing("A return != -1, and a segment to be created.");  */
start
	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	parentpid = getpid();
	parentuid = geteuid();

	expecting (SUCCESS);
	key = (key_t) KEYVAL * 2;
	shared = shmget(key,smin,IPC_CREAT|ALLPERM);

	prep ("Fork a subtest.");
	start

	pid = fork();
	if ( pid == -1 )
		error ("can not fork");

	if ( pid == 0 )
	{
		expecting (SUCCESS);

		setuid(BIN_UID);

		key = (key_t) KEYVAL * 2;
		shared = shmget(key,smin,ALLPERM);

		shmaddr = shmat(shared,(char *) 0, 0);

		shmdt(shmaddr);

		exit(0);
	}
	finis

	expecting (VOID);
	ret_val = wait_for(pid,5);
	if ( ret_val )
	{
		error("subprocess died abnormally status = 0%o",
			ret_val);

	}

	expecting (SUCCESS);
	shmctl(shared,IPC_RMID,&buf);
finis


/*  vstrcpy("Robustness level (2).");  */
testing("Asking for a segment with a size equal to 0 should fail with EINVAL.");
/*  testing("should return of -1, and an errno of EINVAL."); */
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);
	setuid(BIN_UID);

	key_fail((key_t) KEYVAL,0,IPC_CREAT|ALLPERM,EINVAL);
finis


testing("shmget with a size equal to INT_MAX");
testing("should fail with an errno of EINVAL.");
start
	break_on (ERROR | SEVERR);
	key_fail((key_t) KEYVAL,INT_MAX,IPC_CREAT|ALLPERM,EINVAL);
finis


testing("Asking for a segment on which we do not have write permission");
testing("should fail with an errno of EACCES.");
start
	break_on (ERROR | SEVERR);

	expecting (SUCCESS);
	key = (key_t) KEYVAL;
	shmid = shmget(key,smin,IPC_CREAT);

	key_fail(key,smin,ALLPERM,EACCES);

	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	buf.shm_perm.uid = getuid();
	buf.shm_perm.mode = ALLPERM;
	shmctl(shmid,IPC_SET,&buf);
	shmctl(shmid,IPC_RMID,&buf);
finis


testing("Asking for a segment on which we do not have read permission");
testing("should fail with an errno of EACCES.");
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);

	key = (key_t) KEYVAL;
	shmid = shmget(key,smin,IPC_CREAT);

	key_fail(key,smin,ALLPERM,EACCES);

	expecting (VOID);  /*  getuid, getgid can never be wrong  */
	buf.shm_perm.uid = getuid();
	buf.shm_perm.mode = ALLPERM;
	shmctl(shmid,IPC_SET,&buf);
	shmctl(shmid,IPC_RMID,&buf);
finis


testing("Asking for a segment which exists,");
testing("but with size greater than the segment size");
testing("should fail with an errno of EINVAL.");
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);

	key = (key_t) KEYVAL;
	shmid = shmget(key,smin,IPC_CREAT|ALLPERM);

	key_fail(key,smin * 2,ALLPERM,EINVAL);

	shmctl(shmid,IPC_RMID,&buf);
finis


testing("Asking for a nonexistent segment");
testing("should fail with an errno of ENOENT.");
start
	key_fail((key_t) KEYVAL,smin,ALLPERM,ENOENT);
finis


testing("creating a segment that already exists");
testing("should fail with an errno of EEXIST.");
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);

	key = (key_t) KEYVAL;
	shmid = shmget(key,smin,IPC_CREAT|ALLPERM|IPC_EXCL);

	key_fail(key,smin,IPC_CREAT|ALLPERM|IPC_EXCL,EEXIST);
	shmctl(shmid,IPC_RMID,&buf);
finis


testing("overflowing the system imposed limit on the number of");
testing("shared memory segments allowed");
testing("should fail with an errno of ENOSPC.");
start
	break_on (ERROR | SEVERR);
	expecting (SUCCESS);

	/*
	 *  Write all the shared memory ids into a temp file
	 *  for later deletion
	 */
	fd = temp_open("shmget.id",O_RDWR | O_TRUNC | O_CREAT, WORLD);

	expecting (VOID);  /*  loop until shmget fails  */
	i = 0 ;
	do {
		key = (key_t) KEYVAL + i;
		shmid = shmget(key,smin,IPC_CREAT|ALLPERM);
		if ( shmid == -1 )
			break;
		if(write(fd,&shmid,sizeof(int))!=sizeof(int))
			error ("can not write to temp file");
	} while(++i);
	if ( !i )
		error("shmget never returned an error.");

	if ( errno != ENOSPC )
		if (errno == ENOMEM)
			error ("shmget can not reach system imposed limit");
		else
			error ("shmget set errno = %s, ENOMEM was expected",
				err_lookup (errno) );

	expecting (0L);
	lseek(fd,0l,SEEK_SET);

	expecting (VOID);
	while(read(fd,&shmid,sizeof(int))==sizeof(int))
	{
		expecting (SUCCESS);
		shmctl(shmid, IPC_RMID, &buf);
	}

	expecting (SUCCESS);
	close(fd);
finis


testing("Find the maximum segment size allowed.");
testing("Create the maximum number of shared memory segments");
testing("of the maximum size allowed,");
testing("causing an errno of ENOMEM, or ENOSPC.");
start
	continue_on (ERROR | SEVERR);
	expecting (SUCCESS);

	smax = find_max();

	/*
	 *  Write all the shared memory ids into a temp file
	 *  for later deletion
	 */
	fd = temp_open("shmget.id",O_RDWR | O_TRUNC | O_CREAT, WORLD);

	expecting (VOID);  /*  loop until shmget fails  */
	i = 0 ;
	do {
		key = (key_t) KEYVAL + i;
		shmid = shmget(key,smax,IPC_CREAT|ALLPERM);
		if ( shmid == -1 )
			{
			if ( (errno != ENOMEM) && (errno != ENOSPC) )
				error ("shmget set errno = %s",
					err_lookup (errno));
/*	paging systems (where the shared memory is virtual)
 *	or if (SHMMAX * SHMMNI) < (SHMALL * 2048)
 *	will get ENOSPC before ENOMEM.
 *	(the number of table entries runs out before the memory does)
 *	The physical memory could be forced to be consumed by locking
 *	the segments, but that is not in the SVID.  Anyway, there is a
 *	limit on the number of segments per process.
 */
			break;
			}

		if(write(fd,&shmid,sizeof(int))!=sizeof(int))
			error ("can not write to temp file");
	} while( (++i) < 1024);  /*  quit sometime  */

	expecting (0L);
	lseek(fd,0l,SEEK_SET);

	expecting (VOID);
	while(read(fd,&shmid,sizeof(int))==sizeof(int))
	{
		expecting (SUCCESS);
		shmctl(shmid, IPC_RMID, &buf);
	}

	expecting (SUCCESS);
	close(fd);
finis


done();
/*NOTREACHED*/
}

/*
** key_fail
**	Take some repetitive code, and move it into a subroutine.
**	This routine assumes that by calling shmget with these flags,
**	shmget will fail.
*/
key_fail(key,size,flag,errval)
key_t	key;
int	size;
int	flag;
int	errval;
{
	int	ret_val;
	struct	shmid_ds	buf;

	expecting (VOID);  /*  it SHOULD fail here  */
	ret_val = shmget(key,size,flag);
	if ( ret_val != -1 )  /*  it succeeded when it should've failed  */
	{
		if ( (flag | IPC_CREAT) )  /* cleanup  */
			shmctl(ret_val,IPC_RMID,&buf);

		error("shmget returned %d, failure was expected", ret_val);
	}
	else
		if ( errno != errval )
			error ("shmget set errno = %s, %s was expected",
				err_lookup (errno), err_lookup (errval));
}


find_max() {
	unsigned		i,j;
	int			shmid,ret_val;
	struct	shmid_ds	buf;
	key_t			key;

testing ("finding the maximum shared memory segment size");
start
	break_on (ERROR | SEVERR);
	expecting (VOID);

	key = (key_t) IPC_PRIVATE;
	j = INT_MAX;
	do {
		i = 1;
		 while( i&j ) {
			expecting (VOID);
			shmid = shmget(key,(i^j),IPC_CREAT|ALLPERM);
			if(shmid != (-1))
			{
				expecting (SUCCESS);
				shmctl(shmid,IPC_RMID,&buf);
				break;
			}
			i <<= 1;
		}
		i >>= 1;
		j ^= (i);
	} while(i);
finis

return(j-1);


# else
	void exit();
	
	unsupported ("Shared memory subsystem is not supported.");
	exit(0);

#endif
}
