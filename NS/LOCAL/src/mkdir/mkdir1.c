#ident	"@(#)mkdir1.c	1.6"

/*
 *  This subtest runs as set-user-id ROOT
 *  to allow the mounting and unmounting of a read-only file system.
 */

#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2  /*  mkdir was added in SVR3  */

#define	READ_ONLY	1
main()
{
char	filename [100];

setup ("mkdir");

#ifdef SV_RFS
prep("Invoke 'umount -d' to ensure '%s' is not mounted.",SVVS_RFS);
start
	expecting( VOID );
	sv_cmd( "umount", "-d", SVVS_RFS, (char *)0 );
finis
prep("Invoke 'mount -rd' to mount '%s' on '%s' as read only.",
 MOUNT_POINT, SVVS_RFS);
start
	sv_cmd("mount", "-rd", SVVS_RFS, MOUNT_POINT, (char *)0 );
finis
#else
prep ("Mount '%s' as a read-only file system", SVVS_FS);
start
	abort_on (ERROR | SEVERR);
	expecting( VOID );
	umount( SVVS_FS );	/* make sure SVVS_FS is not mounted */
	expecting (SUCCESS);	/*  this will fail if not SUID root  */
	mount (SVVS_FS, MOUNT_POINT, READ_ONLY);
finis
#endif

testing ("try to make a directory on a read-only file system");
start
	break_on (ERROR | SEVERR);	/*  must unlink the file system  */
	expecting (VOID);
	sprintf (filename, "%s/junkdir", MOUNT_POINT);

	expecting (EROFS);
	mkdir (filename);
finis

#ifdef SV_RFS
	dumnt_erofs();
#else
	umnt_erofs();
#endif

done ();
/*NOTREACHED*/
}

#else

main()
{
unsupported("mkdir is not a supported feature in SVR2");
}

#endif
