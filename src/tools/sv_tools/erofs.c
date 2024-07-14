#include <sv_env.h>
#include <svvs.h>

#ifdef UNDEF
#ident "@(#)erofs.c	1.1"
#endif

# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777

mnt_erofs() {
	char path[512];

	prep("Mount the spare file system as writable, create a file on the");
	prep("spare file system, umount the spare file system, and mount");
	prep("the spare file system as read only.");
	start
		expecting( SUCCESS );
		exit_on( ERROR );
		prep("Call umount to ensure '%s' is not mounted.", SVVS_FS);
		start
			expecting( VOID );
			(void) umount(SVVS_FS);		/* insurance */
		finis
		prep("Mount '%s' on '%s' as read/write.", MOUNT_POINT, SVVS_FS);
		start
			mount( SVVS_FS, MOUNT_POINT, READ_WRITE );
		finis
		prep("Create '%s/%s' on '%s'.", MOUNT_POINT, EROFS_FILE, SVVS_FS);
		start
			sprintf( path, "%s/%s", MOUNT_POINT, EROFS_FILE );
			close( creat(path, PROT) );
			sprintf( path, "%s/%s", MOUNT_POINT, EROFS_NOFILE);
			expecting( VOID );
			unlink( path );
		finis
		prep("Unmount '%s' so that it can be mounted read only.",SVVS_FS);
		start
			umount(SVVS_FS);
		finis
		prep("Mount '%s' on '%s' as read only.", MOUNT_POINT, SVVS_FS);
		start
			mount(SVVS_FS, MOUNT_POINT, READ_ONLY);
		finis
	finis
}
umnt_erofs() {
	cleanup("Unmount the spare file system, '%s'.", SVVS_FS);
	start
		expecting ( SUCCESS );
		umount(SVVS_FS);
	finis
}
