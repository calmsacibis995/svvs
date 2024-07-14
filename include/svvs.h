/*
 *	svvs.h - macro definitions for svvs 3.0 test control.
 */
#ifdef	UNDEF
ident	"@(#)svvs.h	1.17"
#endif
/*
** The following macros are defined to make KE/OS tests build properly.
** They will force the SUPPORTED condition to be true for all of the
** KE/OS tests except SHM which is optional.
*/
#define	MSG	1
#define	SEM	1
#define	ACCT	1
#define	CHROOT	1
#define	NICE	1
#define	PLOCK	1
#define	PROFIL	1
#define	PTRACE	1

/*
** End of the KE/OS  macros.
*/

#include <sv_const.h>
#include <sv_conf.h>

#undef	SVVSVERSION
#define	SVVSVERSION	"SVVS 3.0"

#ifndef	SIG_ERR
#define	SIG_ERR	(int(*)())-1
#endif

#undef	BAD_FILDES
#define	BAD_FILDES	-1		/* Good ones should be pos. 			*/
#undef	SVVS_UNROOT
#define	SVVS_UNROOT	-1		/* Device # of unmounted fs				*/
#undef	LOW_FREE
#define	LOW_FREE	2		/* At least that many free 				*/
#undef	LOW_INODES
#define	LOW_INODES	2			/* At least that many free 				*/
#undef	BLOCK
#define	BLOCK		512			/* blocksize used by ulimit				*/
#undef	WAITMUL
#define	WAITMUL		60			/* timeout for child processes			*/
#undef	EUID
#define EUID		UID	 		/* effective uid for svvs 				*/ 
#undef	EGID
#define EGID	 	GID	 		/* effective gid for svvs 				*/ 
#undef	ROOT_EUID
#define ROOT_EUID	ROOT_UID	/* effective user id for root 			*/ 
#undef	ROOT_EGID
#define ROOT_EGID	ROOT_GID	/* effective group id for root			*/ 
#undef	BIN_EUID
#define BIN_EUID	BIN_UID	 	/* effective user id for bin 			*/ 
#undef	BIN_EGID
#define BIN_EGID	BIN_GID	 	/* effective group id for bin 			*/ 
#undef	OS_BIN
#ifndef SV_RFS
#define OS_BIN		"/BA/OS/bin"	/* sub path for BA/OS/bin 	*/
#else
#define OS_BIN		"/NS/REMOTE/bin"/* sub path for NS/REMOTE/bin 	*/
#endif

#undef	STRLEN
#define	STRLEN		512			/* length of strings					*/
#undef	ADVISORY
#define ADVISORY "Advisory: Inconsistent with SVID - see release notes"
#undef	FUTURDIR
#define FUTURDIR "Advisory: Future Direction Implemented - see release notes"
#define	temp_path	sv_temp_path
#define	data_path	sv_data_path
#define	no_close	sv_no_close
#define	no_sig		sv_no_sig
#define map_modes	sv_mp_mode
#define	DCMP		sv_dcmp
#define	dcmp		sv_dcmp
char	*temp_path();
char 	*data_path();

#ifndef TRUE
#define	TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif

#define FILESIZ	(BUFSIZ * 3/2 + 17) /* used by a lot of stdio tests */
#define NO_TRUNC	0
#define TRUNC		1
#define ACCESS		2

#define	SUBTMP		"subtmp"

#ifndef	SYSVR_2		/* used for optional tests in BA_OS */
#undef	LOCKF
#define LOCKF
typedef	void	siggy;
#define	ttime	time_t
#define	S_ISUID	04000
#define	S_ISGID	02000
#else
typedef	int	siggy;
typedef	long	ttime;
typedef	int size_t;
#undef	SEEK_SET
#define	SEEK_SET	0
#undef	SEEK_CUR
#define	SEEK_CUR	1
#undef	SEEK_END
#define	SEEK_END	2
#endif

#define	EROFS_FILE	"erofs.file"
#define EROFS_NOFILE	"erofs.nofile"
