#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)utime0.c	1.9"

# define	PROT	0777

/*
** utime
**	Test the utime function.
**
*/

main()
{
	char	path[PATH_MAX];

	int	fd;
	ttime	seconds;
	int	stat();
	int	close();
	ttime	time();
	struct	stat	buf;
	struct	utimbuf {
		time_t	actime;		/* access time */
		time_t	modtime;	/* modification time */
	} times;

	setup("utime");

	funct("utime");

	exit_on ( ERROR );

	abstract("A legal invocation of times != NULL.");
	abstract("Expect a return value of zero.");
	start

		prep("Create a temporary file prepare a utime struct for it.");
		start
			fd = temp_creat("utime_1", PROT);

			expecting ( SUCCESS );
			close(fd);

			strcpy(path, temp_path());
			strcat(path, "utime_1");

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


		testing("Validate st_atime field.");
		start
			if ( buf.st_atime != times.actime )
			{
				error("stat failed to detect change in atime.\nThe expected time was %ld.\nThe acutal time was %ld.\n", times.actime, buf.st_atime);	/* ERROR */
			}
		finis

		testing("Validate st_mtime field.");
		start
			if ( buf.st_mtime != times.modtime )
			{
				error("stat failed to detect change in mtime.\nThe expected time was %ld.\nThe acutal time was %ld.\n", times.modtime, buf.st_mtime);	/* ERROR */
			}
		finis

	finis

	abstract("A legal invocation of times = NULL.");
	abstract("Expect a return value of zero.");
	start

		prep("Create a temporary file prepare a utime struct for it.");
		start

			fd = temp_creat("utime_2", PROT);

			expecting ( SUCCESS );
			close(fd);

			strcpy(path, temp_path());
			strcat(path, "utime_2");

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

		testing("Validate st_atime field.");
		start
			if ( buf.st_atime < (time_t) seconds)
			{
				error("stat failed to detect change in atime current time.\nThe expected time was %ld.\nThe acutal time was %ld.\n", buf.st_atime, seconds);	/* ERROR */
			}
		finis

		testing("Validate st_mtime field.");
		start
			if ( buf.st_mtime < (time_t) seconds)
			{
				error("stat failed to detect change in mtime to current time.\nThe expected time was %ld.\nThe acutal time was %ld.\n", buf.st_mtime, seconds);	/* ERROR */
			}
		finis
#endif
	finis


	testing("Utime for a file which does not exist.");
	testing("Expect a return value of -1 and errno of ENOENT.");
	start

		strcpy(path, temp_path());
		strcat(path, "_utime");

		expecting( ENOENT );
		utime(path, &times);

	finis

	testing("Utime for a file -- search permission along a path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm.dir");
		strcat(path, "/utime_tryme");

		expecting( EACCES );
		utime(path, &times);

	finis

	testing("Utime -- write permission along a path is denied.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm1.dir");
		strcat(path, "/utime_tryme");

		expecting( EACCES );
		utime(path, (struct utimbuf *)0);

	finis

	testing("Utime -- argument has a component that is not a directory.");
	testing("Expect a return value of -1 and errno of ENOTDIR.");
	start

		fd = temp_creat("utimebad_dir", PROT);

		strcpy(path, temp_path());
		strcat(path, "utime");
		strcat(path, "bad_dir");
		strcat(path, "/dummy_point");

		expecting( ENOTDIR );
		utime(path, &times);

	finis

	testing("Ustat a file not owned by svvs.");
	testing("Expect a return value of -1 and errno of EPERM.");
	start

		strcpy(path, data_path());
		strcat(path, "utime_root");

		expecting( EPERM );
		utime(path, &times);

	finis

	done();
	/*NOTREACHED*/
}
