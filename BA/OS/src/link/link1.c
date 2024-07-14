#include	<sv_base.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)link1.c	1.8"

# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777

/*
** link
**	Test the link function.
**	This file must be run as setuid root.
**
*/

main()
{
	char	path1[PATH_MAX];
	char	path2[PATH_MAX];
	int	link();
	int	mount();
	int	umount();

	setup("link");
	funct("link");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Link a file onto a read only file system.");
	abstract("A return value of -1 and errno of EROFS.");
	start
#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		testing("Call link.");
		start
			strcpy(path2, MOUNT_POINT);
			strcat(path2, "/nogood");

			strcpy(path1, MOUNT_POINT);
			strcat(path1, "/" );
			strcat(path1, EROFS_FILE );

			expecting ( EROFS );
			link(path1, path2);
		finis

#ifdef SV_RFS
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis

	abstract("Link a file across file systems.");
	abstract("A return value of -1 and errno of EXDEV.");
	start
#ifdef SV_RFS
		strcpy(path1, SVVSPATH);	/* need file from local system	*/
		strcat(path1, "/tmp/");		/* for EXDEV across rfs			*/
#else
		strcpy(path1, temp_path());
#endif
		strcat(path1, "link_exdev");

		prep("Create a temporary file.");
		start
			close( creat( path1, PROT) );
		finis

#ifndef SV_RFS
		prep("Mount '%s' read_write.", SVVS_FS);
		start
			expecting ( SUCCESS );
			mount(SVVS_FS, MOUNT_POINT, READ_WRITE);

		finis
#else
		prep("Invoke 'mount -d' to mount '%s' on '%s' as read/write.",
		  MOUNT_POINT, SVVS_RFS);
		start
			sv_cmd("mount", "-d", SVVS_RFS, MOUNT_POINT , (char *)0 );
		finis
#endif
		testing("Unlink '%s/link.tmp' to ensure it does not exist.",
		  MOUNT_POINT);
		start
			strcpy(path2, MOUNT_POINT);
			strcat(path2, "/link.tmp");

			expecting ( VOID );
			(void) unlink(path2);
		finis
		testing("Call link, expecting failure with errno set to EXDEV.");
		start
			expecting ( EXDEV );
			link(path1, path2);
		finis

#ifdef SV_RFS
		unlink(path1);
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis
	done();
	/*NOTREACHED*/
}
