#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)freopen1.c	1.6"

# define	STARTFILE	"startfile.data"
# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777
# define	PROT_BITS	02777

FILE *temp_fopen();
/*
** freopen
**	Test the freopen function.
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
	char		start_path[PATH_MAX];
	char		buf1[FILESIZ];
	FILE		*fptr;
	int		fdesc;

	FILE		*fopen();
	int		mount();
	int		umount();
	int		close();
	FILE	 	*freopen();
	int		stat();
	void		put_file();
	void		init_buf();
	char		*data_path();
	char		*temp_path();

	setup("fopen");
	funct("freopen");

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
		abstract("Verify freopen of existing files.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE );

			testing("Call freopen for write (w).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "w", fptr);
			finis

			testing("Call freopen for append (a).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "a", fptr);
			finis

			testing("Call freopen for update (r+).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "r+", fptr);
			finis

			testing("Call freopen for truncate and update (w+).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "w+", fptr);
			finis

			testing("Call freopen for append and update (a+).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "a+", fptr);
			finis
		finis

		abstract("Verify freopen of nonexistent files.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_NOFILE );

			testing("Call freopen for write (w).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "w", fptr);
			finis

			testing("Call freopen for append (a).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "a", fptr);
			finis

			testing("Call freopen for truncate and update (w+).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "w+", fptr);
			finis

			testing("Call freopen for append and update (a+).");
			start
				fptr = temp_fopen("freopen.tmp","w");
				expecting ( EROFS );
				freopen(path, "a+", fptr);
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
