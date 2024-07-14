#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fstat.c	1.7"

# define	PROT	777

#ifndef SYSVR_2
# define 	MODE	( S_IXOTH | S_IWOTH | S_IROTH | S_IXGRP | S_IWGRP | S_IRGRP | S_IEXEC | S_IWRITE | S_IREAD )
# else
# define	MODE	0777
# endif

/*
** fstat
**	Test the fstat function.
**
*/
main()
{
	int	fd;
	char	path[PATH_MAX];

	struct	stat	buf;
	void	check_fstat();

	setup("stat");

	funct("fstat");

	exit_on ( ERROR );

	abstract("Expect a legal invocation fstat on an ordinary file.");
	abstract("Expect a return value of zero.");
	start

		prep("Open the file.");
		start
			strcpy(path, data_path());
			strcat(path, "fstat_1");

			expecting ( SUCCESS );
			fd = open(path, O_RDONLY);
		finis

		testing("Issue the call to fstat and check the results.");
		start
			expecting ( SUCCESS );
			fstat(fd, &buf);

			check_fstat(&buf);
		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			close(fd);
		finis

	finis

	abstract("Expect a legal invocation fstat on an character special file.");
	abstract("Expect a return value of zero.");
	start
		prep("Open the file.");
		start
			strcpy(path, data_path());
			strcat(path, "fstat_2");

			expecting ( SUCCESS );
			fd = open(path, O_RDONLY);
		finis

		testing("Issue the call to fstat and check the results.");
		start
			expecting ( SUCCESS );
			fstat(fd, &buf);

			check_fstat(&buf);
		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			close(fd);
		finis

	finis

	abstract("Expect a legal invocation fstat on an block special file.");
	abstract("Expect a return value of zero.");
	start
		prep("Open the file.");
		start
			strcpy(path, data_path());
			strcat(path, "fstat_3");

			expecting ( SUCCESS );
			fd = open(path, O_RDONLY);
		finis

		testing("Issue the call to fstat and check the results.");
		start
			expecting ( SUCCESS );
			fstat(fd, &buf);

			check_fstat(&buf);
		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			close(fd);
		finis

	finis

	abstract("Expect a legal invocation fstat on an pipe special file.");
	abstract("Expect a return value of zero.");
	start

		prep("Open the file.");
		start
			strcpy(path, data_path());
			strcat(path, "fstat_4");

			expecting ( SUCCESS );
			fd = open(path, O_RDONLY|O_NDELAY);
		finis

		testing("Issue the call to fstat and check the results.");
		start
			expecting ( SUCCESS );
			fstat(fd, &buf);

			check_fstat(&buf);
		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			close(fd);
		finis

	finis

	abstract("Fstat -- fd is an invalid open file descriptor.");
	abstract("Expect a return value of -1 and errno of EBADF.");
	start

		strcpy(path, data_path());
		strcat(path, "fstat_1");

		expecting ( SUCCESS );
		fd = open(path, O_RDONLY);

		expecting( EBADF );
		fstat(BAD_FILDES, &buf);

		expecting ( SUCCESS );
		close(fd);

	finis

	done();
}

void
check_fstat(buf)

struct	stat	*buf;

{
	if ( ( buf->st_mode & MODE ) != (ushort)MODE )
	{
		error("st_mode in error?.\nThe expected value was %d.\nThe actual value was %d.", MODE, (int)(buf->st_mode & MODE));	/* ERROR */
	}

	if ( buf->st_ino <= (ino_t)0 )
	{
		error("st_ino in error?.\nThe expected value was 0.\nThe actual value was %d.", (int)buf->st_ino);	/* ERROR */
	}

	if ( buf->st_nlink != (short)1 )
	{
		error("st_nlink in error?.\nThe expected value was 1.\nThe actual value was %d.",  (int)buf->st_nlink);	/* ERROR */
	}

	if ( buf->st_uid != (ushort)UID )
	{
		error("st_uid in error?.\nThe expected value was %d.\nThe actual value was %d.", UID, (int)buf->st_uid);	/* ERROR */
	}

	if ( buf->st_gid != (ushort)GID )
	{
		error("st_gid in error?.\nThe expected value was %d.\nThe actual value was %d.", GID, (int)buf->st_gid);	/* ERROR */
	}

	if ( buf->st_size != (off_t)0 )
	{
		error("st_size in error?.\nThe expected value was 0.\nThe actual value was %d.", (int)buf->st_size);	/* ERROR */
	}

	if ( buf->st_atime <= (time_t)0 )
	{
		error("st_atime in error?.\nThe expected value was 0.\nThe actual value was %ld.", (long)buf->st_atime);	/* ERROR */
	}

	if ( buf->st_mtime <= (time_t)0 )
	{
		error("st_mtime in error?.\nThe expected value was 0.\nThe actual value was %ld.", (long)buf->st_mtime);	/* ERROR */
	}

	if ( buf->st_ctime <= (time_t)0 )
	{
		error("st_ctime in error?.\nThe expected value was 0.\nThe actual value was %ld.", (long)buf->st_ctime);	/* ERROR */
	}

}
