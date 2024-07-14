#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>

#ident	"@(#)setaccess.c	1.8"

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
{ 004755, Root_uid, Root_gid, "bin/clraccess" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/access1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/chmod1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/chown0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/clearerr0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/creat1" },
{ 004755, Root_uid, Gid,      "BA/OS/bin/exec3" },
{ 002755, Root_uid, Bin_gid,  "BA/OS/bin/exec4" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/ferror0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/fopen1" },
{ 004755, Root_uid, Root_gid, "BA/OS/bin/fork1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/freopen1" },
{ 006755, Uid,      Gid,      "BA/OS/bin/getegid" },
{ 006755, Uid,      Gid,      "BA/OS/bin/geteuid" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/kill4" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/kill_me_1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/link1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/mkdir1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/mknod0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/mknod2" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/mount0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/mount2" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/open1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/open2" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/rmdir1" },
{ 006755, Root_uid, Root_gid, "BA/OS/bin/setgid" },
{ 002755, Bin_uid,  Bin_gid,  "BA/OS/bin/setgid1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/setuid0" },
{ 004755, Bin_uid,  Bin_gid,  "BA/OS/bin/setuid1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/stime0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/ulimit1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/umount0" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/umount1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/unlink1" },
{ 004755, Root_uid, Bin_gid,  "BA/OS/bin/utime1" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/acct" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/chroot" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/msgctl" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/nice" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/plock" },
{ 004755, Bin_uid,  Bin_gid,  "KE/OS/bin/semctl1" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/shmctl" },
{ 004755, Bin_uid,  Bin_gid,  "KE/OS/bin/shmctl1" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/shmget" },
{ 004755, Root_uid, Bin_gid,  "KE/OS/bin/shmop" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/adv" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/dname" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/fumount" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/fusage" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/idload" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/rfadmin" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/rfstart" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/rfstop" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/rmntstat" },
{ 004755, Root_uid, Bin_gid,  "NS/CMD/bin/unadv" },
{ 040664, Uid,      Gid,      "BA/LIB/data/ftw.dir/dira/errs" },
{ 040377, Uid,      Gid,      "BA/LIB/data/ftw.dir/dirb" },
{ 040677, Uid,      Gid,      "BA/LIB/data/ftw.dir/dirc" },
{ 040664, Uid,      Gid,      "BA/LIB/data/getc.dir/datagetc" },
{ 040664, Uid,      Gid,      "BA/LIB/data/getc.dir/getc" },
{ 040664, Uid,      Gid,      "BA/LIB/data/gets.dir/datagets" },
{ 040664, Uid,      Gid,      "BA/LIB/data/gets.dir/datanonl" },
{ 040664, Uid,      Gid,      "BA/LIB/data/gets.dir/gets" },
{ 040664, Uid,      Gid,      "BA/LIB/data/gets.dir/nonl" },
{ 040664, Uid,      Gid,      "BA/LIB/data/ungetc.dir/dataungetc" },
{ 040664, Uid,      Gid,      "BA/LIB/data/ungetc.dir/ungetc" },
{ 040666, Uid,      Gid,      "BA/LIB/data/scanf.dir/allcases" },
{ 040666, Uid,      Gid,      "BA/LIB/data/scanf.dir/mostcases" },
{ 000777, Uid,      Gid,      "BA/OS/data/access.dir/noperm.dir/access_tryme" },
{ 040400, Uid,      Gid,      "BA/OS/data/access.dir/noperm.dir" },
{ 040000, Uid,      Gid,      "BA/OS/data/chdir.dir/noread.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/chdir.dir/data.badpath" },
{ 040677, Uid,      Gid,      "BA/OS/data/chdir.dir/nosearchowner" },
{ 040767, Bin_uid,  Gid,      "BA/OS/data/chdir.dir/nosearchgroup" },
{ 040776, Bin_uid,  Bin_gid,  "BA/OS/data/chdir.dir/nosearchall" },
{ 000777, Uid,      Gid,      "BA/OS/data/chmod.dir/noperm.dir/chmod_tryme" },
{ 040400, Uid,      Gid,      "BA/OS/data/chmod.dir/noperm.dir" },
{ 000777, Root_uid, Bin_gid,  "BA/OS/data/chmod.dir/chmod_root" },
{ 002777, Uid,      Bin_gid,  "BA/OS/data/chmod.dir/chmod_setgid" },
{ 000777, Uid,      Gid,      "BA/OS/data/chown.dir/noperm.dir/chown_tryme" },
{ 040400, Uid,      Gid,      "BA/OS/data/chown.dir/noperm.dir" },
{ 040777, Uid,      Gid,      "BA/OS/data/clearerr.dir/clearerr.data" },
{ 040777, Uid,      Gid,      "BA/OS/data/close.dir/close_1" },
{ 040400, Uid,      Gid,      "BA/OS/data/creat.dir/noperm.dir" },
{ 040500, Uid,      Gid,      "BA/OS/data/creat.dir/noperm1.dir" },
{ 000555, Uid,      Gid,      "BA/OS/data/creat.dir/nowrite" },
{ 040400, Uid,      Gid,      "BA/OS/data/directory.dir/emptydir" },
{ 040500, Uid,      Gid,      "BA/OS/data/directory.dir/fivefiles" },
{ 040003, Uid,      Gid,      "BA/OS/data/directory.dir/noreadall" },
{ 040037, Uid,      Gid,      "BA/OS/data/directory.dir/noreadgroup" },
{ 040377, Uid,      Gid,      "BA/OS/data/directory.dir/noreadowner" },
{ 040006, Uid,      Gid,      "BA/OS/data/directory.dir/nosearchall" },
{ 040067, Uid,      Gid,      "BA/OS/data/directory.dir/nosearchgroup"},
{ 040677, Uid,      Gid,      "BA/OS/data/directory.dir/nosearchowner"},
{ 040220, Uid,      Gid,      "BA/OS/data/exec.dir/noreaddir" },
{ 000777, Uid,      Gid,      "BA/OS/data/exec.dir/noreaddir/cant_exec" },
{ 000775, Uid,      Gid,      "BA/OS/data/exec.dir/bad_bin_image" },
{ 000777, Uid,      Gid,      "BA/OS/data/exec.dir/dataexec" },
{ 000666, Uid,      Gid,      "BA/OS/data/exec.dir/no_x_perm" },
{ 000777, Uid,      Gid,      "BA/OS/data/exec.dir/not_a_dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/fclose.dir/fclose.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/ferror.dir/ferror.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/fflush.dir/fflush.data" },
{ 040666, Uid,      Gid,      "BA/OS/data/fopen.dir/nosearch.dir" },
{ 040555, Uid,      Gid,      "BA/OS/data/fopen.dir/nowrite.dir" },
{ 000220, Uid,      Gid,      "BA/OS/data/fopen.dir/noread.data" },
{ 000444, Uid,      Gid,      "BA/OS/data/fopen.dir/nowrite.data" },
{ 040777, Uid,      Gid,      "BA/OS/data/fopen.dir/fopen.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/fopen.dir/fopen.dir/fopen.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/fopen.dir/nosearch.dir/fopen.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/fopen.dir/badpath.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/fopen.dir/startfile.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/fork.dir/datafork" },
{ 000777, Uid,      Gid,      "BA/OS/data/fork.dir/fork" },
{ 000777, Uid,      Gid,      "BA/OS/data/fread.dir/freadwrite.dat" },
{ 000220, Uid,      Gid,      "BA/OS/data/freopen.dir/noread.data" },
{ 040664, Uid,      Gid,      "BA/OS/data/freopen.dir/nosearch.dir" },
{ 000444, Uid,      Gid,      "BA/OS/data/freopen.dir/nowrite.data" },
{ 040555, Uid,      Gid,      "BA/OS/data/freopen.dir/nowrite.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/fseek.dir/fseek.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/fseek.dir/hdata" },
{ 000777, Uid,      Gid,      "BA/OS/data/fseek.dir/hlist" },
{ 000777, Uid,      Gid,      "BA/OS/data/ioctl.dir/ioctl_notspec" },
{ 000777, Uid,      Gid,      "BA/OS/data/link.dir/noperm.dir/link_tryme" },
{ 040400, Uid,      Gid,      "BA/OS/data/link.dir/noperm.dir" },
{ 040555, Uid,      Gid,      "BA/OS/data/link.dir/noperm1.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/lockf.dir/lockf_data210" },
{ 000777, Uid,      Gid,      "BA/OS/data/lseek.dir/hdata" },
{ 000777, Uid,      Gid,      "BA/OS/data/lseek.dir/hlist" },
{ 000777, Uid,      Gid,      "BA/OS/data/lseek.dir/lseek.data" },
{ 040400, Uid,      Gid,      "BA/OS/data/mknod.dir/noperm.dir" },
{ 000220, Uid,      Gid,      "BA/OS/data/open.dir/noread.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/open.dir/nosearch.dir/fopen.data" },
{ 040664, Uid,      Gid,      "BA/OS/data/open.dir/nosearch.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/open.dir/open.dir/open.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/open.dir/badpath.data" },
{ 000444, Uid,      Gid,      "BA/OS/data/open.dir/nowrite.data" },
{ 040555, Uid,      Gid,      "BA/OS/data/open.dir/nowrite.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/open.dir/open.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/popen.dir/data0" },
{ 000777, Uid,      Gid,      "BA/OS/data/popen.dir/data1" },
{ 000777, Uid,      Gid,      "BA/OS/data/popen.dir/data2" },
{ 000777, Uid,      Gid,      "BA/OS/data/popen.dir/environ.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/popen.dir/popen.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/stat.dir/stat_1" },
{ 000777, Uid,      Gid,      "BA/OS/data/stat.dir/noperm.dir/stat_tryme" },
{ 040000, Uid,      Gid,      "BA/OS/data/stat.dir/noperm.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/stat.dir/nosearchowner/stat_tryme" },
{ 040677, Uid,      Gid,      "BA/OS/data/stat.dir/nosearchowner" },
{ 000777, Uid,      Gid,      "BA/OS/data/stat.dir/nosearchgroup/stat_tryme" },
{ 040767, Bin_uid,  Gid,      "BA/OS/data/stat.dir/nosearchgroup" },
{ 000777, Uid,      Gid,      "BA/OS/data/stat.dir/nosearchall/stat_tryme" },
{ 040776, Bin_uid,  Bin_gid,  "BA/OS/data/stat.dir/nosearchall" },
{ 000777, Uid,      Gid,      "BA/OS/data/stat.dir/fstat_1" },
{ 000777, Uid,      Gid,      "BA/OS/data/system.dir/data0" },
{ 000777, Uid,      Gid,      "BA/OS/data/system.dir/data1" },
{ 000777, Uid,      Gid,      "BA/OS/data/system.dir/data2" },
{ 000777, Uid,      Gid,      "BA/OS/data/system.dir/environ.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/system.dir/system.data" },
{ 000777, Uid,      Gid,      "BA/OS/data/unlink.dir/noperm.dir/unlink_tryme" },
{ 040400, Uid,      Gid,      "BA/OS/data/unlink.dir/noperm.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/unlink.dir/noperm1.dir/unlink_tryme" },
{ 040555, Uid,      Gid,      "BA/OS/data/unlink.dir/noperm1.dir" },
{ 000777, Uid,      Gid,      "BA/OS/data/utime.dir/noperm.dir/utime_tryme" },
{ 040000, Uid,      Gid,      "BA/OS/data/utime.dir/noperm.dir" },
{ 000555, Root_uid, Gid,      "BA/OS/data/utime.dir/noperm1.dir/utime_tryme" },
{ 040555, Uid,      Gid,      "BA/OS/data/utime.dir/noperm1.dir" },
{ 000777, Uid, 	    Gid,      "BA/OS/data/utime.dir/utime1_root" },
{ 000755, Root_uid, Gid,      "BA/OS/data/utime.dir/utime_root" },

{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/access1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/chmod1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/chown0" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/clearerr0" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/creat1" },
{ 004755, Root_uid, Gid,      "NS/REMOTE/bin/exec3" },
{ 002755, Root_uid, Bin_gid,  "NS/REMOTE/bin/exec4" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/ferror0" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/fopen1" },
{ 004755, Root_uid, Root_gid, "NS/REMOTE/bin/fork1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/freopen1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/link1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/mkdir1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/mknod0" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/mknod2" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/mount" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/open1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/open2" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/rmdir1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/ulimit1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/unlink1" },
{ 004755, Root_uid, Bin_gid,  "NS/REMOTE/bin/utime1" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/ftw.dir/dira/errs" },
{ 040377, Uid,      Gid,      "NS/REMOTE/data/ftw.dir/dirb" },
{ 040677, Uid,      Gid,      "NS/REMOTE/data/ftw.dir/dirc" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/getc.dir/datagetc" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/getc.dir/getc" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/gets.dir/datagets" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/gets.dir/datanonl" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/gets.dir/gets" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/gets.dir/nonl" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/ungetc.dir/dataungetc" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/ungetc.dir/ungetc" },
{ 040666, Uid,      Gid,      "NS/REMOTE/data/scanf.dir/allcases" },
{ 040666, Uid,      Gid,      "NS/REMOTE/data/scanf.dir/mostcases" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/access.dir/noperm.dir/access_tryme" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/access.dir/noperm.dir" },
{ 040000, Uid,      Gid,      "NS/REMOTE/data/chdir.dir/noread.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/chdir.dir/data.badpath" },
{ 040677, Uid,      Gid,      "NS/REMOTE/data/chdir.dir/nosearchowner" },
{ 040767, Bin_uid,  Gid,      "NS/REMOTE/data/chdir.dir/nosearchgroup" },
{ 040776, Bin_uid,  Bin_gid,  "NS/REMOTE/data/chdir.dir/nosearchall" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/chmod.dir/noperm.dir/chmod_tryme" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/chmod.dir/noperm.dir" },
{ 000777, Root_uid, Bin_gid,  "NS/REMOTE/data/chmod.dir/chmod_root" },
{ 002777, Uid,      Bin_gid,  "NS/REMOTE/data/chmod.dir/chmod_setgid" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/chown.dir/noperm.dir/chown_tryme" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/chown.dir/noperm.dir" },
{ 040777, Uid,      Gid,      "NS/REMOTE/data/clearerr.dir/clearerr.data" },
{ 040777, Uid,      Gid,      "NS/REMOTE/data/close.dir/close_1" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/creat.dir/noperm.dir" },
{ 040500, Uid,      Gid,      "NS/REMOTE/data/creat.dir/noperm1.dir" },
{ 000555, Uid,      Gid,      "NS/REMOTE/data/creat.dir/nowrite" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/directory.dir/emptydir" },
{ 040500, Uid,      Gid,      "NS/REMOTE/data/directory.dir/fivefiles" },
{ 040003, Uid,      Gid,      "NS/REMOTE/data/directory.dir/noreadall" },
{ 040037, Uid,      Gid,      "NS/REMOTE/data/directory.dir/noreadgroup" },
{ 040377, Uid,      Gid,      "NS/REMOTE/data/directory.dir/noreadowner" },
{ 040006, Uid,      Gid,      "NS/REMOTE/data/directory.dir/nosearchall" },
{ 040067, Uid,      Gid,      "NS/REMOTE/data/directory.dir/nosearchgroup"},
{ 040677, Uid,      Gid,      "NS/REMOTE/data/directory.dir/nosearchowner"},
{ 040220, Uid,      Gid,      "NS/REMOTE/data/exec.dir/noreaddir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/exec.dir/noreaddir/cant_exec" },
{ 000775, Uid,      Gid,      "NS/REMOTE/data/exec.dir/bad_bin_image" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/exec.dir/dataexec" },
{ 000666, Uid,      Gid,      "NS/REMOTE/data/exec.dir/no_x_perm" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/exec.dir/not_a_dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fclose.dir/fclose.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/ferror.dir/ferror.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fflush.dir/fflush.data" },
{ 040666, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/nosearch.dir" },
{ 040555, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/nowrite.dir" },
{ 000220, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/noread.data" },
{ 000444, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/nowrite.data" },
{ 040777, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/fopen.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/fopen.dir/fopen.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/nosearch.dir/fopen.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/badpath.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fopen.dir/startfile.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fork.dir/datafork" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fork.dir/fork" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fread.dir/freadwrite.dat" },
{ 000220, Uid,      Gid,      "NS/REMOTE/data/freopen.dir/noread.data" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/freopen.dir/nosearch.dir" },
{ 000444, Uid,      Gid,      "NS/REMOTE/data/freopen.dir/nowrite.data" },
{ 040555, Uid,      Gid,      "NS/REMOTE/data/freopen.dir/nowrite.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fseek.dir/fseek.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fseek.dir/hdata" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/fseek.dir/hlist" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/ioctl.dir/ioctl_notspec" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/link.dir/noperm.dir/link_tryme" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/link.dir/noperm.dir" },
{ 040555, Uid,      Gid,      "NS/REMOTE/data/link.dir/noperm1.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/lockf.dir/lockf_data210" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/lseek.dir/hdata" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/lseek.dir/hlist" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/lseek.dir/lseek.data" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/mknod.dir/noperm.dir" },
{ 000220, Uid,      Gid,      "NS/REMOTE/data/open.dir/noread.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/open.dir/nosearch.dir/fopen.data" },
{ 040664, Uid,      Gid,      "NS/REMOTE/data/open.dir/nosearch.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/open.dir/open.dir/open.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/open.dir/badpath.data" },
{ 000444, Uid,      Gid,      "NS/REMOTE/data/open.dir/nowrite.data" },
{ 040555, Uid,      Gid,      "NS/REMOTE/data/open.dir/nowrite.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/open.dir/open.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/popen.dir/data0" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/popen.dir/data1" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/popen.dir/data2" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/popen.dir/environ.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/popen.dir/popen.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/stat.dir/stat_1" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/stat.dir/noperm.dir/stat_tryme" },
{ 040000, Uid,      Gid,      "NS/REMOTE/data/stat.dir/noperm.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/stat.dir/nosearchowner/stat_tryme" },
{ 040677, Uid,      Gid,      "NS/REMOTE/data/stat.dir/nosearchowner" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/stat.dir/nosearchgroup/stat_tryme" },
{ 040767, Bin_uid,  Gid,      "NS/REMOTE/data/stat.dir/nosearchgroup" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/stat.dir/nosearchall/stat_tryme" },
{ 040776, Bin_uid,  Bin_gid,  "NS/REMOTE/data/stat.dir/nosearchall" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/stat.dir/fstat_1" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/system.dir/data0" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/system.dir/data1" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/system.dir/data2" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/system.dir/environ.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/system.dir/system.data" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/unlink.dir/noperm.dir/unlink_tryme" },
{ 040400, Uid,      Gid,      "NS/REMOTE/data/unlink.dir/noperm.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/unlink.dir/noperm1.dir/unlink_tryme" },
{ 040555, Uid,      Gid,      "NS/REMOTE/data/unlink.dir/noperm1.dir" },
{ 000777, Uid,      Gid,      "NS/REMOTE/data/utime.dir/noperm.dir/utime_tryme" },
{ 040000, Uid,      Gid,      "NS/REMOTE/data/utime.dir/noperm.dir" },
{ 000555, Root_uid, Gid,      "NS/REMOTE/data/utime.dir/noperm1.dir/utime_tryme" },
{ 040555, Uid,      Gid,      "NS/REMOTE/data/utime.dir/noperm1.dir" },
{ 000777, Uid, 	    Gid,      "NS/REMOTE/data/utime.dir/utime1_root" },
{ 000755, Root_uid, Gid,      "NS/REMOTE/data/utime.dir/utime_root" },

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
{ 020777, Uid, Gid, Cdev, "BA/OS/data/stat.dir", "fstat_2" },
{ 060777, Uid, Gid, Bdev, "BA/OS/data/stat.dir", "fstat_3" },
{ 010777, Uid, Gid, ZERO, "BA/OS/data/stat.dir", "fstat_4" },
{ 020777, Uid, Gid, Cdev, "BA/OS/data/stat.dir", "stat_2" },
{ 060777, Uid, Gid, Bdev, "BA/OS/data/stat.dir", "stat_3" },
{ 010777, Uid, Gid, ZERO, "BA/OS/data/stat.dir", "stat_4" },
{ 020777, Uid, Gid, Cdev, "NS/REMOTE/data/stat.dir", "fstat_2" },
{ 060777, Uid, Gid, Bdev, "NS/REMOTE/data/stat.dir", "fstat_3" },
{ 010777, Uid, Gid, ZERO, "NS/REMOTE/data/stat.dir", "fstat_4" },
{ 020777, Uid, Gid, Cdev, "NS/REMOTE/data/stat.dir", "stat_2" },
{ 060777, Uid, Gid, Bdev, "NS/REMOTE/data/stat.dir", "stat_3" },
{ 010777, Uid, Gid, ZERO, "NS/REMOTE/data/stat.dir", "stat_4" },
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

	if(geteuid()) 
	{
		fprintf(stderr,"%s: must be executed as root\n",argv[0]);
		exit(1);
	}
	if (chdir(SVVSPATH))
	{
		fprintf(stderr,"%s: can not chdir to %s\n",argv[0],SVVSPATH);
		exit(1);
	}
	printf("%s: setting access modes for '%s'\n",argv[0],SVVSPATH);
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
			if(chmod(perms[i].file,cvtmod((perms[i].mode&06777))))
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
			if(mknod(buf,cvtmod(specials[i].mode),**specials[i].dev))
			{
				fprintf(stderr,"%s: Can't mknod %s\n",
				  argv[0],buf);
				exit(1);
			}
			if(chown(buf,**specials[i].owner,**specials[i].group))
			{
				fprintf(stderr,"%s: Can't chown %s\n",
				  argv[0],buf);
				exit(1);
			}

		}
	}
	exit(0);
}
#ifndef SYSVR_2
struct	modcvt	{
	int	old;
	int	new;
};

struct	modcvt	modes[] = {
	{0000001,	S_IXOTH},
	{0000002,	S_IWOTH},
	{0000004,	S_IROTH},
	{0000010,	S_IXGRP},
	{0000020,	S_IWGRP},
	{0000040,	S_IRGRP},
	{0000100,	S_IXUSR},
	{0000200,	S_IWUSR},
	{0000400,	S_IRUSR},
	{0001000,	S_ISVTX},
	{0002000,	S_ISGID},
	{0004000,	S_ISUID},
	{0010000,	S_IFIFO},
	{0020000,	S_IFCHR},
	{0040000,	S_IFDIR},
	{0060000,	S_IFBLK},
	{0100000,	S_IFREG},
	{0170000,	S_IFMT},
};

#define	NMOD	(sizeof(modes) / sizeof(struct modcvt))
#endif
int	cvtmod(mod)
int	mod;
{
	int	bit;
	int	ret=0;
#ifndef	SYSVR_2
	for(bit=0; bit < NMOD; ++bit){
		if((mod & modes[bit].old) == modes[bit].old){
			ret |= modes[bit].new;
		}
	}
#else
	ret = mod;
#endif
	return(ret);
}
