#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)mknod0.c	1.7"


#ifndef SYSVR_2
# define 	WORLD	S_IXOTH | S_IWOTH | S_IROTH | S_IXGRP | S_IWGRP | S_IRGRP | S_IEXEC | S_IWRITE | S_IREAD | S_ISVTX | S_ISGID | S_ISUID
# else
# define	WORLD	07777
# endif

# define	PROT	07777
# define	NODEV	0
# define	READ_ONLY	1

/*
** mknod
**	Test the mknod function.
**
*/

main()
{
	char	path[PATH_MAX];
	char	path1[PATH_MAX];

	int	mode;
	struct	stat buf;
	int	mknod();
	int	fstat();


	setup("mknod");
	funct("mknod");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Call mknod with every  possible value of mode.");
	abstract("Expect a return value of zero on every call.");
	start

/*
**	Set umask explicitly to zero since driver may not do so....
**	Call unlink on the fly in order not to leave around lots and lots
**	of empty files....
*/

		prep("Setting umask.");
		start
			expecting ( VOID );
			umask(0);

			expecting ( 0 );
			umask(0);
		finis

		testing("Issue the calls to mknod.");
		start
			for ( mode = 00; mode <= 07777; mode++){

				expecting ( SUCCESS );
				sprintf(path, "%s%d", temp_path(), mode);

				expecting ( SUCCESS );
				mknod(path, mode, NODEV);


				expecting ( SUCCESS );
				stat(path, &buf);

				/* mknod explains all */
				if (( buf.st_mode & WORLD ) !=   ( mode & WORLD ) )
				{
					error("Stat reports mode in error?.\n The expected st_mode was %d.\nThe actual st_mode was %d.\n", mode, (int) buf.st_mode);	/* ERROR */
					break;
				}
				expecting ( SUCCESS );
				unlink(path);

			}

		finis
	finis

	abstract("Mknod a file that already exists.");
	abstract("Expect a return value of -1 and errno of EEXIST.");
	start

		prep("Create a temporary file.");
		start
			temp_creat("mknod_eexist", PROT);

			strcpy(path1, temp_path());
			strcat(path1, "mknod_eexist");
		finis

		testing("Issue the calls to mknod.");
		start
			expecting ( SUCCESS );
			mknod(path, PROT, NODEV);

			expecting( EEXIST );
			mknod(path, PROT, NODEV);
		finis

	finis

	testing("Mknod for a file along a path which does not exist.");
	testing("Expect a return value of -1 and errno of ENOENT.");
	start

		strcpy(path, temp_path());
		strcat(path, "_mknod/_mknod");

		expecting( ENOENT );
		mknod(path, PROT, NODEV);
	finis

	testing("Mknod -- argument has a component that is not a directory.");
	testing("Expect a return value of -1 and errno of ENOTDIR");
	start

		temp_creat("mknodbad_dir", PROT);

		strcpy(path, temp_path());
		strcat(path, "mknod");
		strcat(path, "bad_dir");
		strcat(path, "/dummy_point");

		expecting( ENOTDIR );
		mknod(path, PROT, NODEV);

	finis

	abstract("Mknod a file which is on a read only file system.");
	abstract("Expect a return value of -1 and errno of EROFS.");
	start
#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif

		testing("Issue the call to mknod.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_NOFILE );

			expecting( EROFS );
			mknod(path, PROT, NODEV);
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
