# include       <string.h>
# include       <sys/types.h>
# include	<svvs.h>
# include	<sv_env.h>
# include	<sv_macro.h>

#ident	"@(#)header.c	1.3"

# ifdef	MSG
# undef	SVIPCFOO
# define	SVIPCFOO 1
# endif

# ifdef SEM
# undef	SVIPCFOO
# define	SVIPCFOO 1
# endif

# ifdef SHM
# undef	SVIPCFOO
# define	SVIPCFOO 1
# endif

# ifdef	ACCT
# include       <sys/acct.h>
# endif

# ifdef	SVIPCFOO
# include       <sys/ipc.h>
# endif

# ifdef	MSG
# include       <sys/msg.h>
# endif

# ifdef	SEM
# include       <sys/sem.h>
# endif

# ifdef	SHM
# include       <sys/shm.h>
# endif

/*
** header
**	Test the sizes of the elements to structures
*/

	int     Catch;
main()
{


setup("header");

# ifdef ACCT
    hsys_acct();
# else
	unsupported ("acct is not supported, test suppressed");
# endif
# ifdef SVIPCFOO
    hsys_ipc();
# else
	unsupported ("ipc is not supported, test suppressed");
# endif
# ifdef MSG
    hsys_msg();
# else
	unsupported ("msg is not supported, test suppressed");
# endif
# ifdef SEM
    hsys_sem();
# else
	unsupported ("sem is not supported, test suppressed");
# endif
# ifdef SHM
    hsys_shm();
# else
	unsupported ("shm is not supported, test suppressed");
# endif
    done();
/*NOTREACHED*/

}


/*
**
*/
# ifdef ACCT
hsys_acct()
{
	struct  acct s_acct;

testing ("acct");
testing ("The sizes of the elements of the acct struct in sys/s_acct.h should be:");
testing ("char ac_flag,ac_stat,ac_comm[8];");
testing ("ushort  ac_uid,ac_gid;   dev_t ac_tty;   time_t ac_btime;");
testing ("comp_t  ac_utime,ac_stime,ac_mem,ac_io,ac_rw,ac_etime;");
start

    Catch = sizeof(s_acct.ac_flag);
    if(Catch != sizeof(char))
	error("element 's_acct.ac_flag' has invalid size of %d instead of %d",
	    Catch, sizeof(char));


    Catch = sizeof(s_acct.ac_stat);
    if(Catch != sizeof(char))
	error("element 's_acct.ac_stat' has invalid size of %d instead of %d",
	    Catch, sizeof(char));


    Catch = sizeof(s_acct.ac_comm);
    if(Catch != (8*sizeof(char)))
	error("element 's_acct.ac_comm' has invalid size of %d instead of %d",
	    Catch, (8*sizeof(char)));


    Catch = sizeof(s_acct.ac_uid);
    if(Catch != sizeof(ushort))
	error("element 's_acct.ac_uid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_acct.ac_gid);
    if(Catch != sizeof(ushort))
	error("element 's_acct.ac_gid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_acct.ac_tty);
    if(Catch != sizeof(dev_t))
	error("element 's_acct.ac_tty' has invalid size of %d instead of %d",
	    Catch, sizeof(dev_t));


    Catch = sizeof(s_acct.ac_btime);
    if(Catch != sizeof(time_t))
	error("element 's_acct.ac_btime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_acct.ac_etime);
    if(Catch != sizeof(comp_t))
	error("element 's_acct.ac_etime' has invalid size of %d instead of %d",
	    Catch, sizeof(comp_t));


    Catch = sizeof(s_acct.ac_utime);
    if(Catch != sizeof(comp_t))
	error("element 's_acct.ac_utime' has invalid size of %d instead of %d",
	    Catch, sizeof(comp_t));


    Catch = sizeof(s_acct.ac_stime);
    if(Catch != sizeof(comp_t))
	error("element 's_acct.ac_stime' has invalid size of %d instead of %d",
	    Catch, sizeof(comp_t));


    Catch = sizeof(s_acct.ac_mem);
    if(Catch != sizeof(comp_t))
	error("element 's_acct.ac_mem' has invalid size of %d instead of %d",
	    Catch, sizeof(comp_t));


    Catch = sizeof(s_acct.ac_io);
    if(Catch != sizeof(comp_t))
	error("element 's_acct.ac_io' has invalid size of %d instead of %d",
	    Catch, sizeof(comp_t));


    Catch = sizeof(s_acct.ac_rw);
    if(Catch != sizeof(comp_t))
	error("element 's_acct.ac_rw' has invalid size of %d instead of %d",
	    Catch, sizeof(comp_t));

finis
}
# endif


/*
**
*/
#ifdef SVIPCFOO
hsys_ipc()
{
	struct ipc_perm s_ipc_perm;

testing ("ipc_perm");
testing ("The sizes of the elements of the ipc_perm struct in sys/ipc.h");
testing ("should be: ushort uid,gid,cuid,cgid,mode;");
start

    Catch = sizeof(s_ipc_perm.uid);
    if(Catch != sizeof(ushort))
	error("element 's_ipc.perm.uid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_ipc_perm.gid);
    if(Catch != sizeof(ushort))
	error("element 's_ipc.perm.gid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_ipc_perm.cuid);
    if(Catch != sizeof(ushort))
	error("element 's_ipc.perm.cuid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_ipc_perm.cgid);
    if(Catch != sizeof(ushort))
	error("element 's_ipc.perm.cgid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_ipc_perm.mode);
    if(Catch != sizeof(ushort))
	error("element 's_ipc.perm.mode' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));

finis

}
#endif


/*
**
*/
#ifdef MSG
hsys_msg()
{
	struct  msqid_ds  s_msqid_ds;
	struct  msgbuf    s_msgbuf;

testing ("msqid_ds");
testing ("The sizes of the elements of the msqid_ds struct in sys/msg.h");
testing ("should be:  ushort  msg_qnum,msg_qbytes,msg_lspid,msg_lrpid;");
testing ("time_t  msg_stime,msg_rtime,msg_ctime");
start

    Catch = sizeof(s_msqid_ds.msg_qnum);
    if(Catch != sizeof(ushort))
	error("element 's_msqid_ds.msg_qnum' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_msqid_ds.msg_qbytes);
    if(Catch != sizeof(ushort))
	error("element 's_msqid_ds.msg_qbytes' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_msqid_ds.msg_lspid);
    if(Catch != sizeof(ushort))
	error("element 's_msqid_ds.msg_lspid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_msqid_ds.msg_lrpid);
    if(Catch != sizeof(ushort))
	error("element 's_msqid_ds.msg_lrpid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_msqid_ds.msg_stime);
    if(Catch != sizeof(time_t))
	error("element 's_msqid_ds.msg_stime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_msqid_ds.msg_rtime);
    if(Catch != sizeof(time_t))
	error("element 's_msqid_ds.msg_rtime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_msqid_ds.msg_ctime);
    if(Catch != sizeof(time_t))
	error("element 's_msqid_ds.msg_ctime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));
finis


testing ("msgbuf");
testing ("The sizes of the elements of the msgbuf struct in sys/msg.h");
testing ("should be: long    mtype;   char    mtext[1];");
start

    Catch = sizeof(s_msgbuf.mtype);
    if(Catch != sizeof(long))
	error("element 's_msbuf.mtype' has invalid size of %d instead of %d",
	    Catch, sizeof(long));


    Catch = sizeof(s_msgbuf.mtext);
    if(Catch != sizeof(char))
	error("element 's_msbuf.mtext' has invalid size of %d instead of %d",
	    Catch, sizeof(char));

finis

}
#endif


/*
**
*/
#ifdef SEM
hsys_sem()
{
	struct  semid_ds  s_semid_ds;
	struct  sem       s_sem;
	struct  sembuf    s_sembuf;

testing ("semid_ds");
testing ("The sizes of the elements of the semid_ds struct in sys/s_sem.h");
testing ("should be: ushort   sem_nsems;   time_t   sem_otime,sem_ctime;");
start

    Catch = sizeof(s_semid_ds.sem_nsems);
    if(Catch != sizeof(ushort))
	error("element 's_semid_ds.sem_nsems' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_semid_ds.sem_otime);
    if(Catch != sizeof(time_t))
	error("element 's_semid_ds.sem_otime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_semid_ds.sem_ctime);
    if(Catch != sizeof(time_t))
	error("element 's_semid_ds.sem_ctime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));

finis


testing ("sem");
testing ("The sizes of the elements of the sem struct in sys/s_sem.h");
testing ("should be: ushort   semval;   short  sempid,semncnt,semzcnt;");
start

    Catch = sizeof(s_sem.semval);
    if(Catch != sizeof(ushort))
	error("element 's_sem.semval' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_sem.sempid);
    if(Catch != sizeof(short))
	error("element 's_sem.sempid' has invalid size of %d instead of %d",
	    Catch, sizeof(short));


    Catch = sizeof(s_sem.semncnt);
    if(Catch != sizeof(short))
	error("element 's_sem.semncnt' has invalid size of %d instead of %d",
	    Catch, sizeof(short));


    Catch = sizeof(s_sem.semzcnt);
    if(Catch != sizeof(short))
	error("element 's_sem.semzcnt' has invalid size of %d instead of %d",
	    Catch, sizeof(short));

finis


testing ("sembuf");
testing ("The sizes of the elements of the sembuf struct in sys/s_sem.h");
testing ("should be: ushort   sem_op;   short    sem_num,sem_flg;");
start

    Catch = sizeof(s_sembuf.sem_op);
    if(Catch != sizeof(ushort))
	error("element 's_sembuf.sem_op' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_sembuf.sem_num);
    if(Catch != sizeof(short))
	error("element 's_sembuf.sem_num' has invalid size of %d instead of %d",
	    Catch, sizeof(short));


    Catch = sizeof(s_sembuf.sem_flg);
    if(Catch != sizeof(short))
	error("element 's_sembuf.sem_flg' has invalid size of %d instead of %d",
	    Catch, sizeof(short));

finis


}
#endif


/*
**
*/
#ifdef SHM
hsys_shm()
{
	struct  shmid_ds  s_shmid_ds;

testing ("shmid_ds");
testing ("The sizes of the elements of the shmid_ds struct in sys/shm.h");
testing ("should be: int    shm_segsz;   ushort   shm_lpid,shm_cpid,shm_nattch;");
testing ("time_t  shm_atime,shm_dtime,shm_ctime;");
start

    Catch = sizeof(s_shmid_ds.shm_lpid);
    if(Catch != sizeof(ushort))
	error("element 's_shmid_ds.shm_lpid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_shmid_ds.shm_cpid);
    if(Catch != sizeof(ushort))
	error("element 's_shmid_ds.shm_cpid' has invalid size of %d instead of %d",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_shmid_ds.shm_atime);
    if(Catch != sizeof(time_t))
	error("element 's_shmid_ds.shm_atime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_shmid_ds.shm_dtime);
    if(Catch != sizeof(time_t))
	error("element 's_shmid_ds.shm_dtime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_shmid_ds.shm_ctime);
    if(Catch != sizeof(time_t))
	error("element 's_shmid_ds.shm_ctime' has invalid size of %d instead of %d",
	    Catch, sizeof(time_t));

finis


}   
#endif
