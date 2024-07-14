#include	<sv_base.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)chmod1.c	1.7"

# define	READ_ONLY	1
# define	READ_WRITE	0
# define	PROT		0777
# define	PROT_BITS	02777

/*
 * chmod1.c - a test of setuid root cases of chmod(BA_OS).
 *
 */
main() {
	struct	stat	buf;
	char	*path;
	char	*malloc();

	setup("chmod");

#ifndef SYSVR_2
	map_modes(1);	/* turn on SVR2, SVR3 mode mapping */
#endif

	exit_on ( ERROR );

	prep("Allocate a path buffer.");
	start
		path = malloc( PATH_MAX );
	finis

	abstract("Call chmod for a file which resides on a read only file");
	abstract("system.  Verify failure with errno set to EROFS.");
	start
#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		testing("Call chmod.");
		start
			continue_on( ERROR );
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE);
#ifdef	SV_RFS
			expecting( VOID );
			if( !chmod( path, PROT ) )
				warning( "%s\n%s", ADVISORY,
				  "chmod succeeded unexpectedly");
			else {
				expecting( EROFS );
				chmod( path, PROT );
			}
#else
			expecting( EROFS );
			chmod(path, PROT);
#endif
		finis
#ifdef SV_RFS
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis

	abstract("While executing as root, call chmod to set the setgid bit");
	abstract("of a file which is not in root's group.  Call stat to");
	abstract("verify that the setgid bit is not cleared.");
	start
		prep("Initialize a path name.");
		start
			strcpy(path, data_path());
			strcat(path, "chmod_setgid");
		finis
		testing("Call chmod.");
		start
			chmod(path, PROT_BITS);
		finis
		testing("Call stat to verify the setgid bit was not cleared.");
		start
			stat(path, &buf);
			if (( buf.st_mode & 07777 ) != PROT_BITS )
					error("%s\n%s%d\n%s%d\n",
					  "stat reported unexpected mode",
					  "expected:	",
					  PROT_BITS,
					  "actual:	",
					  (int) buf.st_mode );
		finis
	finis
	cleanup("Free the path buffer.");
	start
		free( path );
	finis
	done();
	/*NOTREACHED*/
}
