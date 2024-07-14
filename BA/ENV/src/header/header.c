# include	<sv_base.h>
# include       <sys/types.h>
# include       <string.h>
# include	<math.h>
# include	<sys/stat.h>
# include	<sys/times.h>
# include	<sys/utsname.h>
# include	<time.h>
# include	<ustat.h>
# include	<svvs.h>
# include	<sv_env.h>
# include	<sv_macro.h>

# ifndef SYSVR_1
# include	<malloc.h>
# endif

# ifndef SYSVR_1
# ifndef SYSVR_2
# include	<fcntl.h>
# endif
# endif



#ident	"@(#)header.c	1.4"


/*
** header
**	Test the existence and content of the header files
**      for the BASE section.
*/
	int     Catch;
main()
{


setup("header");

# ifndef SYSVR_1
# ifndef SYSVR_2
    hd_fcntl();
# else
    unsupported ("file locking not supported, tests suppressed");
# endif
# else
    unsupported ("file locking not supported, tests suppressed");
# endif
    hd_malloc();
    hd_math();
    hd_sysstat();
    hd_systimes();
    hd_sysutsname();
    hd_ustat();
    hd_time();

    done();
/*NOTREACHED*/

}


# ifndef SYSVR_1
# ifndef SYSVR_2
/*
**
*/
hd_fcntl()
{
	struct  flock  s_flock;

testing ("flock");
testing ("The sizes of the elements of the flock struct in fcntl.h should be:");
testing ("short l_type, l_whence; long l_start, l_len;  short l_pid.");
start
	continue_on (ERROR);
	Catch = sizeof(s_flock.l_type);
	if(Catch != sizeof(short))
		error("Element 's_flock.l_type' has invalid size of %d instead of %d.",
		    Catch, sizeof(short));


	Catch = sizeof(s_flock.l_whence);
	if(Catch != sizeof(short))
		error("Element 's_flock.l_whence' has invalid size of %d instead of %d.",
		    Catch, sizeof(short));


	Catch = sizeof(s_flock.l_start);
	if(Catch != sizeof(long))
		error("Element 's_flock.l_start' has invalid size of %d instead of %d.",
		    Catch, sizeof(long));


	Catch = sizeof(s_flock.l_len);
	if(Catch != sizeof(long))
		error("Element 's_flock.l_len' has invalid size of %d instead of %d.",
		    Catch, sizeof(long));


	Catch = sizeof(s_flock.l_pid);
	if(Catch != sizeof(short))
		error("Element 's_flock.l_pid' has invalid size of %d instead of %d.",
		    Catch, sizeof(short));
finis
}
# endif
# endif



/*
**
*/
hd_malloc()
{
#ifndef SYSVR_1
	struct  mallinfo  s_mallinfo;

testing ("mallinfo");
testing ("The sizes of the elements of the mallinfo struct in malloc.h should be:");
testing ("int   arena,ordblks,smblks,hblks,hblkhd,usmblks,fsmblks,uordblks,forblks,keepcost.");
start
	continue_on (ERROR);

	Catch = sizeof(s_mallinfo.arena);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.arena' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.ordblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.ordblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.smblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.smblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.hblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.hblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.hblkhd);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.hblkhnd' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.usmblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.usmblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.fsmblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.fsmblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.uordblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.uorblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.fordblks);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.fordblks' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));


	Catch = sizeof(s_mallinfo.keepcost);
	if(Catch != sizeof(int))
		error("Element 's_mallinfo.keepcost' has invalid size of %d instead of %d.",
		    Catch, sizeof(int));
finis
#else
    unsupported ("mallinfo not supported, tests suppressed");
#endif
}



/*
**
*/
hd_math()
{
	struct  exception s_exception;

testing ("exception");
testing ("The sizes of the elements of the exception struct in math.h should be ");
testing ("int type;   char * name;   double arg1, arg2, retval;.");
start
    continue_on (ERROR);
    Catch = sizeof(s_exception.type);
    if(Catch != sizeof(int))
	error("Element 's_exception.type' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_exception.name);
    if(Catch != sizeof(char *))
	error("Element 's_exception.name' has invalid size of %d instead of %d.",
	    Catch, sizeof(char *));


    Catch = sizeof(s_exception.arg1);
    if(Catch != sizeof(double))
	error("Element 's_exception.arg1' has invalid size of %d instead of %d.",
	    Catch, sizeof(double));


    Catch = sizeof(s_exception.arg2);
    if(Catch != sizeof(double))
	error("Element 's_exception.arg2' has invalid size of %d instead of %d.",
	    Catch, sizeof(double));


    Catch = sizeof(s_exception.retval);
    if(Catch != sizeof(double))
	error("Element 's_exception.retval' has invalid size of %d instead of %d.",
	    Catch, sizeof(double));
finis
}



/*
**
*/
hd_sysstat()
{
	struct  stat  s_stat;

testing ("stat");
testing ("The sizes of the elements of the stat struct in sys/stat.h should be:");
testing ("dev_t  st_dev,st_rdev;   ino_t st_ino;   ushort st_mode,st_uid,ushort st_gid;  off_t st_size; short st_nlink; time_t st_atime,st_mtime,ctime;.");
start
    continue_on (ERROR);
    Catch = sizeof(s_stat.st_dev);
    if(Catch != sizeof(dev_t))
	error("Element 's_stat.st_dev' has invalid size of %d instead of %d.",
	    Catch, sizeof(dev_t));


    Catch = sizeof(s_stat.st_rdev);
    if(Catch != sizeof(dev_t))
	error("Element 's_stat.st_rdev' has invalid size of %d instead of %d.",
	    Catch, sizeof(dev_t));


    Catch = sizeof(s_stat.st_ino);
    if(Catch != sizeof(ino_t))
	error("Element 's_stat.st_ino' has invalid size of %d instead of %d.",
	    Catch, sizeof(ino_t));


    Catch = sizeof(s_stat.st_mode);
    if(Catch != sizeof(ushort))
	error("Element 's_stat.st_mode' has invalid size of %d instead of %d.",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_stat.st_uid);
    if(Catch != sizeof(ushort))
	error("Element 's_stat.st_uid' has invalid size of %d instead of %d.",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_stat.st_gid);
    if(Catch != sizeof(ushort))
	error("Element 's_stat.st_gid' has invalid size of %d instead of %d.",
	    Catch, sizeof(ushort));


    Catch = sizeof(s_stat.st_size);
    if(Catch != sizeof(off_t))
	error("Element 's_stat.st_size' has invalid size of %d instead of %d.",
	    Catch, sizeof(off_t));


    Catch = sizeof(s_stat.st_nlink);
    if(Catch != sizeof(short))
	error("Element 's_stat.st_nlink' has invalid size of %d instead of %d.",
	    Catch, sizeof(short));


    Catch = sizeof(s_stat.st_atime);
    if(Catch != sizeof(time_t))
	error("Element 's_stat.st_atime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_stat.st_mtime);
    if(Catch != sizeof(time_t))
	error("Element 's_stat.st_mtime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_stat.st_ctime);
    if(Catch != sizeof(time_t))
	error("Element 's_stat.st_stime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));
finis
}



/*
**
*/
hd_systimes()
{
	struct  tms  s_tms;

testing ("tms");
testing ("The sizes of the elements of the tms struct in sys/times.h should be:");
testing (" time_t tms_utime,tms_stime,tms_cutime,tms_cstime.");
start
    continue_on (ERROR);
    Catch = sizeof(s_tms.tms_utime);
    if(Catch != sizeof(time_t))
	error("Element 's_tms.tms_utime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_tms.tms_stime);
    if(Catch != sizeof(time_t))
	error("Element 's_tms.tms_stime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_tms.tms_cutime);
    if(Catch != sizeof(time_t))
	error("Element 's_tms.tms_cutime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));


    Catch = sizeof(s_tms.tms_cstime);
    if(Catch != sizeof(time_t))
	error("Element 's_tms.tms_cstime' has invalid size of %d instead of %d.",
	    Catch, sizeof(time_t));
finis
}



/*
**
*/
hd_sysutsname()
{
	struct  utsname  s_utsname;

testing ("utsname");
testing ("The sizes of the elements of the utsname struct in sys/s_utsname.h should be:");
testing ("char sysname,nodename,release,version,machine .");
start
    continue_on (ERROR);

    Catch = sizeof(s_utsname.sysname);
    if(Catch != (SYS_NMLN*sizeof(char)))
	error("Element 's_utsname.sysname' has invalid size of %d instead of %d.",
	    Catch, (SYS_NMLN*sizeof(char)));


    Catch = sizeof(s_utsname.nodename);
    if(Catch != (SYS_NMLN*sizeof(char)))
	error("Element 's_utsname.nodename' has invalid size of %d instead of %d.",
	    Catch, (SYS_NMLN*sizeof(char)));


    Catch = sizeof(s_utsname.release);
    if(Catch != (SYS_NMLN*sizeof(char)))
	error("Element 's_utsname.release' has invalid size of %d instead of %d.",
	    Catch, (SYS_NMLN*sizeof(char)));


    Catch = sizeof(s_utsname.version);
    if(Catch != (SYS_NMLN*sizeof(char)))
	error("Element 's_utsname.version' has invalid size of %d instead of %d.",
	    Catch, (SYS_NMLN*sizeof(char)));


    Catch = sizeof(s_utsname.machine);
    if(Catch != (SYS_NMLN*sizeof(char)))
	error("Element 's_utsname.machine' has invalid size of %d instead of %d.",
	    Catch, (SYS_NMLN*sizeof(char)));
finis
}



/*
**
*/
hd_time()
{
	struct  tm  s_tm;

testing ("tm");
testing ("The sizes of the elements of the tm struct in time.h should be:");
testing ("int tm_sec, tm_min, tm_hour, tm_mdat, tm_mon, tm_year, tm_wdat, tm_yday, tm_isdst.");
start
    Catch = sizeof(s_tm.tm_sec);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_sec' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_min);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_min' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_hour);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_hour' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_mday);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_mday' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_mon);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_mon' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_year);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_year' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_wday);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_wday' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_yday);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_yday' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


    Catch = sizeof(s_tm.tm_isdst);
    if(Catch != sizeof(int))
	error("Element 's_tm.tm_isdst' has invalid size of %d instead of %d.",
	    Catch, sizeof(int));


finis
}



/*
**
*/
hd_ustat()
{
	struct  ustat  s_ustat;

testing ("ustat");
testing ("The sizes of the elements of the ustat struct in s_ustat.h should be:");
testing ("daddr_t  f_tfree;  ino_t f_tinode;  char f_fname, f_fpack;.");
start
    Catch = sizeof(s_ustat.f_tfree);
    if(Catch != sizeof(daddr_t))
	error("Element 's_ustat.f_tfree' has invalid size of %d instead of %d.",
	    Catch, sizeof(daddr_t));


    Catch = sizeof(s_ustat.f_tinode);
    if(Catch != sizeof(ino_t))
	error("Element 's_ustat.f_tinode' has invalid size of %d instead of %d.",
	    Catch, sizeof(ino_t));


    Catch = sizeof(s_ustat.f_fname);
    if(Catch != (6*sizeof(char)))
	error("Element 's_ustat.f_fname' has invalid size of %d instead of %d.",
	    Catch, (6*sizeof(char)));


    Catch = sizeof(s_ustat.f_fpack);
    if(Catch != (6*sizeof(char)))
	error("Element 's_ustat.f_fpack' has invalid size of %d instead of %d.",
	    Catch, (6*sizeof(char)));

finis
}
