#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)open1.c	1.4"

# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777
# define	PROT_BITS	02777

/*
** open
**	Test the open function.
**	This file must be run as setuid root.
**
**	This test is dependent upon:
**		mount
**		umount
**		close
**		stat
**		<stdio.h>
*/

main()
{
	char		path[PATH_MAX];
	int		fdesc;
	int		sts;
	int		expct;

	int		mount();
	int		umount();
	int		close();
	int		open();
	int		stat();
	char		*temp_path();

	setup("open");
	funct("open");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Verify open on a read only file system.");
	start

#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		abstract("Verify open of existing files.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE );

			testing("Call open for O_RDONLY.");
			start
				int fd;
				fd = open(path, O_RDONLY);
				close( fd );
			finis

			testing("Call open for O_WRONLY.");
			start
				expecting ( EROFS );
				open(path, O_WRONLY);
			finis

			testing("Call open for O_RDWR.");
			start
				expecting ( EROFS );
				open(path, O_RDWR);
			finis

		finis

		abstract("Verify open of nonexistent files.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_NOFILE );

			testing("Call open for O_WRONLY | O_CREAT.");
			start
				expecting ( EROFS );
				open(path, O_WRONLY | O_CREAT, 0666 );
			finis

			testing("Call open for O_RDWR | O_CREAT.");
			start
				expecting ( EROFS );
				open(path, O_RDWR | O_CREAT, 0666 );
			finis
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
