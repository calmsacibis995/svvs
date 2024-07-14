#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)umount0.c	1.7"

# define	BLOCK_SPEC	060777
# define	PROT		0777
# define	READ_ONLY	1

/*
** umount
**	Test the umount function.
**
*/

main()
{
	int	expected;
	int	ret_val;

	char	my_node[PATH_MAX];
	char	tmp_file[PATH_MAX];
	int	umount();
	int	mount();
	int	mknod();
	int	unlink();

	setup("umount");

	funct("umount");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("A legal invocation of umount.");
	abstract("Expect a return value of zero.");
	start
		prep("Unmount SVVS_FS (if it's mounted) and mount it.");
		start
			expecting ( VOID );
			(void) umount(SVVS_FS); /* insurance */

			expecting ( SUCCESS );
			mount(SVVS_FS, MOUNT_POINT, READ_ONLY);
		finis

		testing("Issue the call to umount.");
		start
			expecting ( SUCCESS );
			umount(SVVS_FS);
		finis

	finis

	abstract("Unmounting a device special which does not exist.");
	abstract("Expect a return value of -1 and errno of ENXIO or EINVAL.");
	start

		prep("Create a temporary node.");
		start
			strcpy(my_node, temp_path());
			strcat(my_node, "umount_spec");

			expecting ( SUCCESS );

			mknod(my_node, BLOCK_SPEC, OFF_LINE_DEV);

		finis

		testing("Issue the call to umount.");
		start
			expecting( VOID );
			expected = -1;
			ret_val = umount(my_node);

			if ( ret_val != expected )
			{
				error("Incorrect return value from umount.\nThe expected value was %d.\nThe actual value was %d.\n", expected, ret_val);	/* ERROR */
			}
			else if ( (errno != ENXIO) && (errno != EINVAL) )
				error ("errno = %s", err_lookup (errno));
		finis


		cleanup("Remove the temporary node.");
		start
			expecting ( SUCCESS );
			unlink(my_node);
		finis
	finis

	abstract("unmount -- argument has a component that is not a directory.");
	abstract("Expect a return value of -1 and errno of ENOTDIR");
	start

		prep("Create a file which will be the illegal component of the path.");
		start
			temp_creat("bad_dir", PROT);

			strcpy(my_node, temp_path());
			strcat(my_node, "bad_dir");
			strcpy(tmp_file, my_node);
			strcat(my_node, "/dummy_point");
		finis

		testing("Issue the call to umount.");
		start
			expecting( ENOTDIR );
			umount(my_node);
		finis

		cleanup("Remove the temporary file.");
		start
			expecting ( SUCCESS );
			unlink(tmp_file);
		finis

	finis

	abstract("Unmounting a named special file that does not exist.");
	abstract("Expect a return value of -1 and errno of ENOENT.");
	start

		strcpy(my_node, temp_path());
		strcat(my_node, "dummy_point");

		expecting ( VOID );
		(void) unlink(my_node);

		expecting( ENOENT );
		umount(my_node);

	finis

	abstract("Unmounting a non block special device.");
	abstract("Expect a return value of -1 and errno of ENOTBLK or EINVAL.");
	start

		prep("Create a file which will be the non-block spec. device.");		
		start
			temp_creat("dummy_point", PROT);

			strcpy(my_node, temp_path());
			strcat(my_node, "dummy_point");
		finis

		testing("Issue the call to umount.");
		start
			expecting (-1);
			umount (my_node);
			if ((errno != ENOTBLK) && (errno != EINVAL))
				error ("errno = %s", err_lookup (errno));
		finis

		cleanup("Remove the file.");
		start
			expecting ( SUCCESS );
			unlink(my_node);
		finis

	finis

	abstract("Unmounting a device that is not mounted.");
	abstract("Expect a return value of -1 and errno of EINVAL.");
	start

		expecting ( VOID );
		(void) umount(SVVS_FS);		/* make sure it's not there */

		expecting( EINVAL );
		umount(SVVS_FS);

	finis

	abstract("Umount a device which has a file open and is busy.");
	abstract("To unmount the device that contains the root filesystem.");
	abstract("Expect a return value of -1 and errno of EBUSY.");
	start

		expecting( EBUSY );
		umount(SVVS_ROOT);

	finis

	done();
	/*NOTREACHED*/
}
