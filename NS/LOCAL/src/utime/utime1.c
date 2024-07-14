#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)utime1.c	1.12"

# define	PROT	0777
# define	READ_WRITE	0
# define	READ_ONLY	1

/*
** utime
**	Test the utime function.
**
**	This test must be run as setuid root.
**
*/

main()
{
	char	path[PATH_MAX];

	int	stat();
	int	close();
	ttime	time();
	int	mount();
	int	umount();
	int	fd;
	ttime	seconds;
	struct	stat	buf;
	struct	utimbuf{
		time_t	actime;		/* access time */
		time_t	modtime;	/* modification time */
	} times;

	setup("utime");

	funct("utime");


# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("Utime a file which is part of a read only file system.");
	abstract("Expect a  return value of -1 and errno of EROFS.");
	start

#ifdef SV_RFS
		dmnt_erofs();
#else
		mnt_erofs();
#endif
		testing("Issue the call to utime.");
		start
			strcpy(path, MOUNT_POINT);
			strcat(path, "/" );
			strcat(path, EROFS_FILE);
#ifdef SV_RFS
			expecting( VOID );
			if( !utime(path, &times) )
				warning( "%s\nutime succeeded unexpectedly",ADVISORY );
			else {
				expecting( EROFS );
				utime(path, &times);
			}
#else
			expecting( EROFS );
			utime(path, &times);
#endif
		finis
#ifdef SV_RFS
		dumnt_erofs();
#else
		umnt_erofs();
#endif
	finis

	abstract("A legal invocation of times != NULL: file owned by svvs: effective uid of super-user.");
	abstract("Expect a  return value of zero.");

	start

		prep("Prepare a utime structure.");
		start
			strcpy(path, data_path());
			strcat(path, "utime1_root");

			times.actime = (time_t)0;
			times.modtime = (time_t)0;
		finis

		testing("Issue the call to utime.");
		start
			expecting ( SUCCESS );
			utime(path, &times);
		finis

		testing("Stat the temporary file.");
		start
			expecting ( SUCCESS );
			stat(path, &buf);
		finis

		testing("Validate sysname field.");
		start
			if ( buf.st_atime != times.actime )
			{
				error("stat failed to detect change in atime.\nThe expected time was %l.\nThe actual time was %l.\n", times.actime, buf.st_atime);	/* ERROR */
			}
		finis

		testing("Validate sysname field.");
		start
			if ( buf.st_mtime != times.modtime )
			{
				error("stat failed to detect change in mtime.\nThe expected time was %l.\nThe actual time was %l.\n", times.modtime, buf.st_mtime);	/* ERROR */
			}
		finis

	finis

	abstract("A legal invocation of times = NULL: file owned by svvs: effective uid of super-user.");
	abstract("Expect a  return value of zero.");
	start

		prep("Prepare a utime structure.");
		start
			strcpy(path, data_path());
			strcat(path, "utime1_root");

			expecting ( SUCCESS );
			seconds = time ((ttime *) 0);
		finis

		testing("Issue the call to utime.");
		start
			expecting ( SUCCESS );
			utime(path, (struct utimbuf *)0);
		finis
#ifndef SV_RFS
		testing("Stat the temporary file.");
		start
			expecting ( SUCCESS );
			stat(path, &buf);
		finis

		testing("Validate sysname field.");
		start
			if ( buf.st_atime < (time_t) seconds)
			{
				error("stat failed to detect change in atime current time.\nThe expected time was %l.\nThe actual time was %l.\n", buf.st_atime, seconds);	/* ERROR */
			}
		finis

		testing("Validate sysname field.");
		start
			if ( buf.st_mtime < (time_t) seconds)
			{
				error("stat failed to detect change in mtime to current time.\nThe expected time was %l.\nThe actual time was %l.\n", buf.st_mtime, seconds);	/* ERROR */
			}
		finis
#endif
	finis

	done();
	/*NOTREACHED*/
}
