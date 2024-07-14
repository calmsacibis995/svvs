#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mount0.c	1.5"

# define	BLOCK_SPEC	060777
# define	PROT		0777
# define	READ_ONLY	1
# define	READ_WRITE	0

/*
** mount
**	Test the mount function.
**
*/

main()
{
	char	mount_point[PATH_MAX];
	char	my_node[PATH_MAX];
	char	buf[PATH_MAX];
	char	tmp_file[PATH_MAX];
	char	PathPointer[PATH_MAX];
	int	mount();
	int	umount();
	int	mknod();
	int	unlink();

	setup("mount");
	funct("mount");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("A legal invocation of mount.");
	abstract("Expect a return value of zero.");
	start

		prep("Unmount SVVS_FS...just in case a prev. test failed.");
		start
			expecting ( VOID );
			(void) umount(SVVS_FS); /* make sure SVVS_FS is not mounted */
		finis

		testing("Issue the call to mount.");
		start
			expecting ( SUCCESS );
			mount(SVVS_FS, MOUNT_POINT, READ_ONLY);
		finis

		cleanup("Unmount SVVS_FS.");
		start
			expecting ( SUCCESS );
			umount(SVVS_FS);
		finis

	finis

	abstract("Mounting a device special which does not exist.");
	abstract("Expect a return value of -1 and errno of ENXIO");
	start

		prep("Issue a call to mknod to create a mount point.");
		start
			strcpy(my_node, temp_path());
			strcat(my_node, "mount_spec");

			expecting ( SUCCESS );
			mknod(my_node, BLOCK_SPEC, OFF_LINE_DEV);
		finis

		testing("Issue the call to mount.");
		start
			expecting( ENXIO );
			mount(my_node, MOUNT_POINT, READ_WRITE);
		finis

		cleanup("Unlink the mount point.");
		start
			expecting ( SUCCESS );
			unlink(my_node);
		finis

	finis

	abstract("Mount -- argument has a component that is not a directory.");
	abstract("Expect a return value of -1 and errno of ENOTDIR");
	start

		prep("Create a temporary file.");
		start	
		    temp_creat("mount_bad_dir", PROT);

		strcpy(mount_point, temp_path());
		strcat(mount_point, "mount_bad_dir");
		strcpy(tmp_file, mount_point);
		strcat(mount_point, "/dummy_point");
	finis

	testing("Issue the call to mount.");
	start
		expecting( ENOTDIR );
		mount(SVVS_FS, mount_point, READ_WRITE);
	finis

	cleanup("Unlink the temporary file.");
	start
		expecting ( SUCCESS );
		unlink(tmp_file);
	finis

finis

abstract("Mounting at a mount point that does not exist.");
abstract("Expect a return value of -1 and errno of ENOENT.");
start

	prep("Remove the normal mount point.");
	start
		strcpy(mount_point, temp_path());
		strcat(mount_point, "dummy_point");

		expecting ( VOID );
		(void) unlink(mount_point);
	finis

	testing("Issue the call to mount.");
	start
		expecting( ENOENT );
		mount(SVVS_FS, mount_point, READ_WRITE);
	finis

finis

abstract("Mounting a named special file that does not exist.");
abstract("Expect a return value of -1 and errno of ENOENT.");
start

	prep("Unlink the special file.");
	start
		strcpy(my_node, temp_path());
		strcat(my_node, "dummy_node");

		expecting ( VOID );
		(void) unlink(my_node);
	finis

	testing("Issue the call to mount.");
	start
		expecting( ENOENT );
		mount(my_node, MOUNT_POINT, READ_WRITE);
	finis

finis

abstract("Mounting a non block special device.");
abstract("Expect a return value of -1 and errno of ENOTBLK");
start

	prep("Create a temporary file.");
	start
		temp_creat("dummy_point", PROT);

		strcpy(mount_point, temp_path());
		strcat(mount_point, "dummy_point");
		strcpy(PathPointer, temp_path());
		PathPointer[strlen(PathPointer)-1] = 0;
	finis

	testing("Issue the call to mount.");
	start
		expecting( ENOTBLK );
		mount(mount_point, PathPointer, READ_WRITE);
	finis

	cleanup("Remove the temporary file.");
	start
		expecting ( SUCCESS );
		unlink(mount_point);
	finis


finis

abstract("Mount a device which is currently mounted, by");
abstract("attempting to mount the device that contains the root filesystem.");
abstract("Expect a return value of -1 and errno of EBUSY.");
start

	expecting( EBUSY );
	mount(SVVS_ROOT, MOUNT_POINT, READ_WRITE);

finis

abstract("Mount a device on a directory which is currently busy.");
abstract("Expect a return value of -1 and errno of EBUSY.");
start

	strcpy(buf, SVVSPATH);
	strcat(buf, OS_BIN);

	expecting( EBUSY );
	mount(SVVS_FS, buf, READ_WRITE);

finis

done();
/*NOTREACHED*/
}
