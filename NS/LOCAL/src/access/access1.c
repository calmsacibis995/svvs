#include	<sv_base.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)access1.c	1.6"

# define	W_ACC		02

/*
 * access
 *	Test the access function on a read only file system.
 *	This file must be run as setuid root so that it can mount/umount.
 */
main()
{
	char	path[PATH_MAX];
	int	fdesc;

	setup("access");

	funct("access");
	
# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	abstract("Test access of a file which resides");
	abstract("on a read only file system.");
	start
#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		testing("Invoke access for the file residing on the read");
		testing("only file sys.");
		start
			continue_on( ERROR );
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE );

			expecting( EROFS );
			access(path, W_ACC);
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
