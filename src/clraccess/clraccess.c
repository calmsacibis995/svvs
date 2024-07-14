#include	<stdio.h>
#include	<svvs.h>

#ident	"@(#)clraccess.c	1.6"

int	zero=0;
int	*ptrzero = &zero;

#define DIRBIT	040000

#define Root_uid	(& &(ROOT_UID))
#define Root_gid	(& &(ROOT_GID))
#define Bin_uid		(& &(BIN_UID))
#define Bin_gid		(& &(BIN_GID))
#define Uid		(& &(UID))
#define Gid		(& &(GID))
#define Cdev		(& &(CDEV))
#define Bdev		(& &(BDEV))
#define	ZERO		(&ptrzero)
struct	perms {
	int	mode;
	int	**owner;
	int	**group;
	char	*file;
} perms[] = {
{ 000755,	Uid,	Gid,	"BA/OS/bin/access1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/chmod1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/chown1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/chown2" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/chown3" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/clearerr0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/creat1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/exec3" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/exec4" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/ferror0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/fopen1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/fopen2" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/freopen1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/freopen2" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/getegid" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/geteuid" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/kill4" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/kill_me_1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/link1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/mknod0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/mount0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/mount2" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/open1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/open2" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/rmdir1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/setgid" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/setgid1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/setuid0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/setuid1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/stime0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/ulimit1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/umount0" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/umount1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/unlink1" },
{ 000755,	Uid,	Gid,	"BA/OS/bin/utime1" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/acct" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/chroot" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/msgctl" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/nice" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/plock" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/semctl1" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/shmctl" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/shmctl1" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/shmget" },
{ 000755,	Uid,	Gid,	"KE/OS/bin/shmop" },
{ 000755,	Uid,	Gid,	"BA/LIB/data/ftw.dir/dirb" },
{ 000755,	Uid,	Gid,	"BA/LIB/data/ftw.dir/dirc" },
{ 000755,	Uid,	Gid,	"BA/OS/data/unlink.dir/noperm.dir/unlink_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/unlink.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/unlink.dir/noperm1.dir/unlink_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/unlink.dir/noperm1.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/link.dir/noperm.dir/link_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/link.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/link.dir/noperm1.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chdir.dir/noread.dir" },
{ 000755,	Uid,	Gid,    "BA/OS/data/chdir.dir/nosearchowner" },
{ 000755,	Uid,	Gid,    "BA/OS/data/chdir.dir/nosearchgroup" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chdir.dir/nosearchall" },

{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/stat_1" },
{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/noperm.dir/stat_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/fstat_1" },
{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/nosearchowner" },
{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/nosearchgroup" },
{ 000755,	Uid,	Gid,	"BA/OS/data/stat.dir/nosearchall" },

{ 000755,	Uid,	Gid,	"BA/OS/data/utime.dir/noperm.dir/utime_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/utime.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/utime.dir/noperm1.dir/utime_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/utime.dir/noperm1.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/utime.dir/utime_root" },
{ 000755,	Uid,	Gid,	"BA/OS/data/access.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chown.dir/noperm.dir/chown_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chown.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chmod.dir/noperm.dir/chmod_tryme" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chmod.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chmod.dir/chmod_root" },
{ 000755,	Uid,	Gid,	"BA/OS/data/chmod.dir/chmod_setgid" },
{ 000755,	Uid,	Gid,	"BA/OS/data/fopen.dir/nosearch.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/fopen.dir/nowrite.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/fopen.dir/noread.data" },
{ 000755,	Uid,	Gid,	"BA/OS/data/fopen.dir/nowrite.data" },
{ 000755,	Uid,	Gid,	"BA/OS/data/fopen.dir/fopen.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/mknod.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/freopen.dir/noread.data" },
{ 000755,	Uid,	Gid,	"BA/OS/data/freopen.dir/nosearch.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/freopen.dir/nowrite.data" },
{ 000755,	Uid,	Gid,	"BA/OS/data/freopen.dir/nowrite.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/creat.dir/noperm.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/creat.dir/noperm1.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/creat.dir/nowrite" },
{ 000755,	Uid,	Gid,	"BA/OS/data/open.dir/noread.data" },
{ 000755,	Uid,	Gid,	"BA/OS/data/open.dir/nowrite.data" },
{ 000755,	Uid,	Gid,	"BA/OS/data/open.dir/nowrite.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/open.dir/nosearch.dir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/exec.dir/noreaddir" },
{ 000755,	Uid,	Gid,	"BA/OS/data/exec.dir/bad_bin_image" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/emptydir" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/fivefiles" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/noreadall" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/noreadgroup" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/noreadowner" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/nosearchall" },
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/nosearchgroup"},
{ 000755,       Uid,    Gid,    "BA/OS/data/directory.dir/nosearchowner"},
{ 000755,       Uid,    Gid,    "NS/CMD/bin/adv" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/dname" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/fumount" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/fusage" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/idload" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/rfadmin" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/rfstart" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/rfstop" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/rmntstat" },
{ 000755,       Uid,    Gid,    "NS/CMD/bin/unadv" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/access1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/chmod1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/chown0" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/clearerr0" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/creat1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/exec3" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/exec4" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/ferror0" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/fopen1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/fopen2" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/fork1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/freopen1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/freopen2" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/link1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/mkdir1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/mknod0" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/mknod2" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/mount" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/open1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/open2" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/ulimit1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/unlink1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/bin/utime1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ftw.dir/dira/errs" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ftw.dir/dirb" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ftw.dir/dirc" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/getc.dir/datagetc" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/getc.dir/getc" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/gets.dir/datagets" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/gets.dir/datanonl" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/gets.dir/gets" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/gets.dir/nonl" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ungetc.dir/dataungetc" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ungetc.dir/ungetc" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/scanf.dir/allcases" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/scanf.dir/mostcases" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/access.dir/noperm.dir/access_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/access.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chdir.dir/noread.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chdir.dir/data.badpath" },
{ 000755,	Uid,	Gid,	"NS/REMOTE/data/chdir.dir/nosearchowner" },
{ 000755,	Uid,	Gid,	"NS/REMOTE/data/chdir.dir/nosearchgroup" },
{ 000755,	Uid,	Gid,	"NS/REMOTE/data/chdir.dir/nosearchall" },

{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chmod.dir/noperm.dir/chmod_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chmod.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chmod.dir/chmod_root" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chmod.dir/chmod_setgid" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chown.dir/noperm.dir/chown_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/chown.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/clearerr.dir/clearerr.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/close.dir/close_1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/creat.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/creat.dir/noperm1.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/creat.dir/nowrite" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/emptydir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/fivefiles" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/noreadall" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/noreadgroup" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/noreadowner" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/nosearchall" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/nosearchgroup"},
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/directory.dir/nosearchowner"},
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/exec.dir/noreaddir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/exec.dir/noreaddir/cant_exec" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/exec.dir/bad_bin_image" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/exec.dir/dataexec" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/exec.dir/no_x_perm" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/exec.dir/not_a_dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fclose.dir/fclose.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ferror.dir/ferror.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fflush.dir/fflush.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/nosearch.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/nowrite.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/noread.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/nowrite.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/fopen.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/fopen.dir/fopen.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/nosearch.dir/fopen.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/badpath.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fopen.dir/startfile.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fork.dir/datafork" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fork.dir/fork" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fread.dir/freadwrite.dat" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/freopen.dir/noread.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/freopen.dir/nosearch.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/freopen.dir/nowrite.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/freopen.dir/nowrite.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fseek.dir/fseek.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fseek.dir/hdata" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/fseek.dir/hlist" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/ioctl.dir/ioctl_notspec" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/link.dir/noperm.dir/link_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/link.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/link.dir/noperm1.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/lockf.dir/lockf_data210" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/lseek.dir/hdata" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/lseek.dir/hlist" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/lseek.dir/lseek.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/mknod.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/noread.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/nosearch.dir/fopen.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/nosearch.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/open.dir/open.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/badpath.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/nowrite.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/nowrite.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/open.dir/open.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/popen.dir/data0" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/popen.dir/data1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/popen.dir/data2" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/popen.dir/environ.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/popen.dir/popen.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/stat.dir/stat_1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/stat.dir/noperm.dir/stat_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/stat.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/stat.dir/fstat_1" },
{ 000755,	Uid,	Gid,	"NS/REMOTE/data/stat.dir/nosearchowner" },
{ 000755,	Uid,	Gid,	"NS/REMOTE/data/stat.dir/nosearchgroup" },
{ 000755,	Uid,	Gid,	"NS/REMOTE/data/stat.dir/nosearchall" },

{ 000755,       Uid,    Gid,    "NS/REMOTE/data/system.dir/data0" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/system.dir/data1" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/system.dir/data2" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/system.dir/environ.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/system.dir/system.data" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/unlink.dir/noperm.dir/unlink_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/unlink.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/unlink.dir/noperm1.dir/unlink_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/unlink.dir/noperm1.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/utime.dir/noperm.dir/utime_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/utime.dir/noperm.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/utime.dir/noperm1.dir/utime_tryme" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/utime.dir/noperm1.dir" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/utime.dir/utime1_root" },
{ 000755,       Uid,    Gid,    "NS/REMOTE/data/utime.dir/utime_root" },


};

#define	NPERMS	(sizeof(perms)/sizeof(struct perms))

struct	specials {
	int	mode;
	int	**owner;
	int	**group;
	int	**dev;
	char	*path;
	char	*name;
} specials[] = {
{ 000777, Uid, Gid, Cdev, "BA/OS/data/stat.dir", "fstat_2" },
{ 000777, Uid, Gid, Bdev, "BA/OS/data/stat.dir", "fstat_3" },
{ 000777, Uid, Gid, ZERO, "BA/OS/data/stat.dir", "fstat_4" },
{ 000777, Uid, Gid, Cdev, "BA/OS/data/stat.dir", "stat_2" },
{ 000777, Uid, Gid, Bdev, "BA/OS/data/stat.dir", "stat_3" },
{ 000777, Uid, Gid, ZERO, "BA/OS/data/stat.dir", "stat_4" },
{ 000777, Uid, Gid, Cdev, "NS/REMOTE/data/stat.dir", "fstat_2" },
{ 000777, Uid, Gid, Bdev, "NS/REMOTE/data/stat.dir", "fstat_3" },
{ 000777, Uid, Gid, ZERO, "NS/REMOTE/data/stat.dir", "fstat_4" },
{ 000777, Uid, Gid, Cdev, "NS/REMOTE/data/stat.dir", "stat_2" },
{ 000777, Uid, Gid, Bdev, "NS/REMOTE/data/stat.dir", "stat_3" },
{ 000777, Uid, Gid, ZERO, "NS/REMOTE/data/stat.dir", "stat_4" },
};

#define	NSPECIALS 	(sizeof(specials)/sizeof(struct specials))

main(argc,argv)
int argc;
char *argv[];
{
	int	i;
	char	buf[512];
	char	sbuf[128];
	void 	exit();

	sv_config();

	if (chdir(SVVSPATH))
	{
		fprintf(stderr,"%s: can not chdir to %s\n",argv[0],SVVSPATH);
		exit(1);
	}
	printf("%s: Restoring access modes for '%s'\n",argv[0],SVVSPATH);
	umask(0);
	for( i = 0; i < NPERMS ; i++ ) 
	{
		if(!access(perms[i].file,0))
		{
			if(chown(perms[i].file,**perms[i].owner,**perms[i].group))
			{
				fprintf(stderr,"%s: Can't chown %s\n",
				  argv[0],perms[i].file);
				exit(1);
			}
			if(chmod(perms[i].file,(perms[i].mode&06777)))
			{
				fprintf(stderr,"%s: Can't chmod %s\n",
				  argv[0],perms[i].file);
				exit(1);
			}
		}
	}
	
	for( i = 0; i < NSPECIALS ; i++ ) 
	{
		if(!access(specials[i].path,0))
		{
			strcpy(buf,specials[i].path);
			strcat(buf,"/");
			strcat(buf,specials[i].name);
			unlink(buf);
		}
	}
	exit(0);
}
