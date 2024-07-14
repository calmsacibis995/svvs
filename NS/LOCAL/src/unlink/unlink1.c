#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)unlink1.c	1.5"

# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777

/*
** unlink
**	Test the unlink function.
**	This file must be run as setuid root.
**
*/

main()
{
	char	path[PATH_MAX];

	int	fdesc;
	int	mount();
	int	umount();
	int	close();
	int	unlink();

	setup("unlink");

	funct("unlink");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Unlink a mount point for a mounted file system.");
	abstract("Expect a return value of -1 and errno of EBUSY.");
	start
#ifdef SV_RFS
		prep("Invoke 'umount -d' to ensure '%s' is not mounted.",SVVS_RFS);
		start
			expecting( VOID );
			sv_cmd( "umount", "-d", SVVS_RFS, (char *)0 );
		finis
		prep("Invoke 'mount -d' to mount '%s' on '%s'.",
		  MOUNT_POINT, SVVS_RFS);
		start
			sv_cmd("mount", "-d", SVVS_RFS, MOUNT_POINT , (char *)0 );
		finis
#else
		prep("Unmount SVVS_FS if it's mounted.");
		prep("Mount SVVS_FS.");
		start
			expecting ( VOID );
			(void) umount(SVVS_FS); /* insurance */

			expecting ( SUCCESS );
			mount(SVVS_FS, MOUNT_POINT, READ_WRITE);

		finis
#endif
		testing("Issue the call to unlink.");
		start
			expecting( EBUSY );
			unlink(MOUNT_POINT);
		finis

#ifdef SV_RFS
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis

	testing("Unlink a file which is part of a read only file system.");
	testing("Expect a return value of -1 and errno of EROFS.");
	start

#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif	

		testing("Issue the call to unlink.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/");
			strcat(path, EROFS_FILE);
			expecting( EROFS );
			unlink(path);
		finis

#ifdef SV_RFS
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis

	done();
	/*NOTREACHED*/
}
