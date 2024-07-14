#ident	"@(#)rmdir1.c	1.7"

/*
 *  This subtest runs as set-user-id ROOT
 *  to allow the mounting and unmounting of a read-only file system.
 */

#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2  /*  rmdir was added in SVR3  */

#define	READ_WRITE	0
#define	READ_ONLY	1
main()
{
char	filename [100];

setup ("rmdir");
funct ("rmdir");

#ifdef SV_RFS
	prep("Mount the spare remote file system as writable, create a directory");
	prep("on the spare remote file system, umount the spare remote file");
	prep("system, and mount the spare remote file system as read only.");
	start
		expecting( SUCCESS );
		exit_on( ERROR );
		prep("Invoke 'umount -d' to ensure '%s' is not mounted.",SVVS_RFS);
		start
			expecting( VOID );
			sv_cmd( "umount", "-d", SVVS_RFS, (char *)0 );
		finis
		prep("Invoke 'mount -d' to mount '%s' on '%s' as read/write.",
		  MOUNT_POINT, SVVS_RFS);
		start
			sv_cmd("mount", "-d", SVVS_RFS, MOUNT_POINT , (char *)0 );
		finis
		prep("Create '%s/%s' on '%s'.", MOUNT_POINT,"junk.dir", SVVS_RFS);
		start
			sprintf( filename, "%s/%s", MOUNT_POINT, "junk.dir" );

			mkdir (filename);
		finis
		prep("Invoke 'umount -d' so that '%s' can be mounted read only.",
		  SVVS_RFS);
		start
			sv_cmd("umount", "-d", SVVS_RFS, (char *)0 );
		finis
		prep("Invoke 'mount -rd' to mount '%s' on '%s' as read only.",
		  MOUNT_POINT, SVVS_RFS);
		start
			sv_cmd("mount", "-rd", SVVS_RFS, MOUNT_POINT, (char *)0 );
		finis
	finis
#else
prep ("mount the SVVS_FS file system and make a directory in it");
prep ("then unmount the file system and re-mount it as read-only");
start
	abort_on (ERROR | SEVERR);
	expecting( VOID );
	umount( SVVS_FS );	/* make sure SVVS_FS is not mounted */
	expecting (SUCCESS);	/*  this will fail if not SUID root  */
	mount (SVVS_FS, MOUNT_POINT, READ_WRITE);

	break_on (ERROR | SEVERR);	/*  must unlink the file system  */
	expecting (VOID);
	sprintf (filename, "%s/junkdir", MOUNT_POINT);

	expecting (SUCCESS);
	mkdir (filename);

	expecting (SUCCESS);
	umount (SVVS_FS);

	expecting (SUCCESS);	/*  this will fail if not SUID root  */
	mount (SVVS_FS, MOUNT_POINT, READ_ONLY);
finis
#endif
testing ("rmdir should fail on the read-only file system with EROFS");
start
	break_on (ERROR | SEVERR);/*  must still unlink the file system  */
	expecting (EROFS);
	rmdir (filename);
finis


testing ("rmdir (a directory that is busy) should error with EBUSY");
start
	expecting (EBUSY);
	rmdir (MOUNT_POINT);
finis

#ifdef SV_RFS
	dumnt_erofs();
	cleanup("Mount the spare remote file system as writable, and remove");
	cleanup("'junk.dir'.  Then unmount the spare remote file system.");
	start
		expecting( SUCCESS );
		exit_on( ERROR );
		cleanup("Invoke 'mount -d' to mount '%s' on '%s' as read/write.",
		  MOUNT_POINT, SVVS_RFS);
		start
			sv_cmd("mount", "-d", SVVS_RFS, MOUNT_POINT , (char *)0 );
		finis
		cleanup("Remove '%s/%s'.", MOUNT_POINT,"junk.dir");
		start
			sprintf( filename, "%s/%s", MOUNT_POINT, "junk.dir" );

			rmdir (filename);
		finis
		cleanup("unmount the spare remote file system.");
		start
			sv_cmd("umount", "-d", SVVS_RFS, (char *)0 );
		finis
	finis
#else
cleanup ("unmount the file system and re-mount it as read-write");
cleanup ("then remove the test directory and unmount the file system.");
start
	expecting (SUCCESS);
	umount (SVVS_FS);

	expecting (SUCCESS);	/*  this will fail if not SUID root  */
	mount (SVVS_FS, MOUNT_POINT, READ_WRITE);
	rmdir (filename);
	umount (SVVS_FS);
finis
#endif

done ();
/*NOTREACHED*/
}

#else

main ()
{
unsupported ("rmdir is not a supported feature of SVR2");
}

#endif
