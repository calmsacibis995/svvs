#include	<sv_base.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)chmod0.c	1.3"

# define	PROT		0777
# define	BITS_PROT	06777

/*
 * chmod0.c	- a test of normal cases chmod(BA_OS).
 *
 *	Eventually the magic numbers 04000 and 02000
 *	should be turned into S_ISUID and S_ISGID
 *
 *	For SVVS 3.0 the magic numbers are interpreted by
 *	the appropriate z_funct.
 */

main() {
	char	*path;
	int		mode;
	struct	stat	buf;
	char	*malloc();

	setup("chmod");

#ifndef SYSVR_2
	map_modes(1);	/* turn on SVR2 to SVR3 mode mapping */
#endif

	exit_on ( ERROR );
	prep("Allocate a path buffer.");
	start
		path = malloc( PATH_MAX );
	finis
	abstract("Call chmod with all possible values of mode.  Verify");
	abstract("the requested mode was set with stat.");
	start
		prep("Creat a temp file and initialize a path name.");
		start
			temp_creat("chmod_1", PROT);

			strcpy(path, temp_path());
			strcat(path, "chmod_1");
		finis
		testing("Call chmod for all possible modes.");
		start
			for ( mode = 00; mode <= 07777; mode++){

				chmod(path, mode);
				stat(path, &buf);
				if (( buf.st_mode & 07777 ) != ( mode & 06777 ) )
					error("%s\n%s%d\n%s%d\n",
					  "stat reported unexpected mode",
					  "expected:	",
					  mode,
					  "actual:	",
					  (int) buf.st_mode );
			}
		finis
	finis

	abstract("Call chmod of a file that has setgid bits set");
	abstract("and is not a member of the svvs group.");
	abstract("Call stat to verify the setgid bit is cleared.");
	start
		prep("Initialize a path name.");
		start
			strcpy(path, data_path());
			strcat(path, "chmod_setgid");
		finis
		testing("Call chmod.");
		start
			chmod(path, PROT);
		finis
		testing("Call stat to verify the setgid bit was cleared.");
		start
			stat(path, &buf);
		
			if (( buf.st_mode & 0777 ) != (ushort) PROT )
				error("%s\n%s%d\n%s%d\n",
				  "stat reported unexpected mode",
				  "expected:	",
				  PROT,
				  "actual:	",
				  (int) buf.st_mode );
		finis
	finis

	abstract("Call chmod for a file which does not exist.");
	abstract("Verify failure with errno set to ENOENT.");
	start
		prep("Initialize a path name.");
		start
			strcpy(path, temp_path());
			strcat(path, "_chmod");
		finis
		testing("Call chmod.");
		start
			expecting( ENOENT );
			chmod(path, PROT);
		finis
	finis

	abstract("Call chmod for a file where search permission along");
	abstract("the path is denied.  Verify failure with errno set");
	abstract("to EACCES.");
	start
		prep("Initialize a path name.");
		start
			strcpy(path, data_path());
			strcat(path, "noperm.dir");
			strcat(path, "/chmod_tryme");
		finis
		testing("Call chmod.");
		start
			expecting( EACCES );
			chmod(path, PROT);
		finis
	finis

	testing("Call chmod of a path with a non-directory component.");
	testing("Verify failure with errno set to ENOTDIR.");
	start
		prep("Creat a temp file for use as a non-directory component.");
		prep("Initialize a path name.");
		start
			temp_creat("chmodbad_dir", PROT);

			strcpy(path, temp_path());
			strcat(path, "chmod");
			strcat(path, "bad_dir");
			strcat(path, "/dummy_point");
		finis
		testing("Call chmod.");
		start
			expecting( ENOTDIR );
			chmod(path, PROT);
		finis
	finis


	abstract("Call chmod of a file not owned by svvs.");
	abstract("Verify failure with errno set to EPERM.");
	start
		prep("Initialize a pathname.");
		start
			strcpy(path, data_path());
			strcat(path, "chmod_root");
		finis
		testing("Call chmod.");
		start
			expecting( EPERM );
			chmod(path, PROT);
		finis
	finis
	cleanup("Free that path buffer.");
	start
		free( path );
	finis
	done();
	/*NOTREACHED*/
}

