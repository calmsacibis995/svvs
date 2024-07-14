#include <sv_env.h>
#include <svvs.h>

#ifdef UNDEF
#ident "@(#)derofs.c	1.1"
#endif

# define	PROT		0777

dmnt_erofs() {
	char path[512];
	prep("Mount the spare remote file system as writable, create a file");
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
		prep("Create '%s/%s' on '%s'.", MOUNT_POINT, EROFS_FILE, SVVS_RFS);
		start
			sprintf( path, "%s/%s", MOUNT_POINT, EROFS_FILE );
			close( creat( path, PROT) );
			sprintf( path, "%s/%s", MOUNT_POINT, EROFS_NOFILE);
			expecting( VOID );
			unlink( path );
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
}
dumnt_erofs() {
	cleanup("Invoke 'umount -d' to unmount '%s'.", SVVS_RFS);
	start
		expecting ( SUCCESS );
		sv_cmd("umount", "-d", SVVS_RFS, (char *)0 );
	finis
}
