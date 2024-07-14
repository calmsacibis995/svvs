
#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)creat1.c	1.6"

# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777

/*
** creat
**	Test the creat function.
**	This file must be run as setuid root.
**
*/

main()
{
	char	path[PATH_MAX];

	int	mount();
	int	umount();
	int	close();
	int	creat();
	int	fdesc;

	setup("creat");

	funct("creat");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Creat an existing file which resides on a read only file");
	abstract("system and attempt to creat a nonexistent file on a read");
	abstract("only file system.");
	start
#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		testing("Invoke creat for a file residing on the read");
		testing("only file_sys.");
		start
			continue_on( ERROR );
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE);

			expecting( EROFS );
			creat(path, PROT);
		finis
		testing("Invoke creat for a file not residing on the read");
		testing("only file_sys.");
		start

			continue_on( ERROR );
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_NOFILE);

			expecting( EROFS );
			creat(path, PROT);
		finis

#ifdef SV_RFS
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis

	done();
/*
 *	NOTREACHED
 */
}
