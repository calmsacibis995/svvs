#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fopen1.c	1.5"


# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777
# define	PROT_BITS	02777

/*
** fopen
**	Test the fopen function.
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

	int		mount();
	int		umount();
	int		close();
	FILE	 	*fopen();
	int		stat();
	char		*temp_path();

	setup("fopen");

	funct("fopen");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );


	abstract("Verify fopen on a read only file system.");
	start

#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		abstract("Verify fopen of existing files.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE );

			testing("Call fopen for write (w).");
			start
				expecting ( EROFS );
				fopen(path, "w");
			finis

			testing("Call fopen for append (a).");
			start
				expecting ( EROFS );
				fopen(path, "a");
			finis

			testing("Call fopen for update (r+).");
			start
				expecting ( EROFS );
				fopen(path, "r+");
			finis

			testing("Call fopen for truncate and update (w+).");
			start
				expecting ( EROFS );
				fopen(path, "w+");
			finis

			testing("Call fopen for append and update (a+).");
			start
				expecting ( EROFS );
				fopen(path, "a+");
			finis
		finis

		abstract("Verify fopen of nonexistent files.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_NOFILE );

			testing("Call fopen for write (w).");
			start
				expecting ( EROFS );
				fopen(path, "w");
			finis

			testing("Call fopen for append (a).");
			start
				expecting ( EROFS );
				fopen(path, "a");
			finis

			testing("Call fopen for truncate and update (w+).");
			start
				expecting ( EROFS );
				fopen(path, "w+");
			finis

			testing("Call fopen for append and update (a+).");
			start
				expecting ( EROFS );
				fopen(path, "a+");
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
