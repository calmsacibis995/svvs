#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)stat0.c	1.7"

# define	PROT	0777


#ifndef SYSVR_2
# define 	MODE	( S_IXOTH | S_IWOTH | S_IROTH | S_IXGRP | S_IWGRP | S_IRGRP | S_IEXEC | S_IWRITE | S_IREAD )
# else
# define	MODE	0777
# endif
/*
** stat
**	Test the stat function.
**
**	Future work could verify and exercise each of the fields
**	in the stat structure.
**
*/

main()
{
	char	path[PATH_MAX];

	struct	stat	buf;
	void	check_stat();
	int	stat();

	setup("stat");

	funct("stat");

	exit_on ( ERROR );

	testing("Expect a legal invocation stat on an ordinary file.");
	testing("Expect a return value of zero.");
	start

		strcpy(path, data_path());
		strcat(path, "stat_1");

		expecting ( SUCCESS );
		stat(path, &buf);

		check_stat(&buf);

	finis

	testing("Expect a legal invocation stat on an character special file.");
	testing("Expect a return value of zero.");
	start

		strcpy(path, data_path());
		strcat(path, "stat_2");

		expecting ( SUCCESS );
		stat(path, &buf);

		check_stat(&buf);

	finis

	testing("Expect a legal invocation stat on an block special file.");
	testing("Expect a return value of zero.");
	start


		strcpy(path, data_path());
		strcat(path, "stat_3");

		expecting ( SUCCESS );
		stat(path, &buf);

		check_stat(&buf);

	finis

	testing("Expect a legal invocation stat on an pipe special file.");
	testing("Expect a return value of zero.");
	start


		strcpy(path, data_path());
		strcat(path, "stat_4");

		expecting ( SUCCESS );
		stat(path, &buf);

		check_stat(&buf);

	finis

	testing("Stat -- argument has a component that is not a directory.");
	testing("Expect a return value of -1 and errno of ENOTDIR");
	start

		temp_creat("statbad_dir", PROT);

		strcpy(path, temp_path());
		strcat(path, "stat");
		strcat(path, "bad_dir");
		strcat(path, "/dummy_point");

		expecting( ENOTDIR );
		stat(path, &buf);
	finis

	testing("Stat a file which does not exist.");
	testing("Expect a return value of -1 and errno of ENOENT.");
	start

		strcpy(path, temp_path());
		strcat(path, "_stat");

		expecting( ENOENT );
		stat(path, &buf);

	finis

	testing("Stat a file where search permission to the directory is denied to all.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "noperm.dir");
		strcat(path, "/stat_tryme");

		expecting( EACCES );
		stat(path, &buf);

	finis



	testing("Stat a file where search permission to the directory is denied to owner.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "nosearchowner");
		strcat(path, "/stat_tryme");

		expecting( EACCES );
		stat(path, &buf);

	finis



	testing("Stat a file where search permission to the directory is denied to group.");
	testing("The directory mode is 767 but owned by bin user.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "nosearchgroup");
		strcat(path, "/stat_tryme");

		expecting( EACCES );
		stat(path, &buf);

	finis



	testing("Stat a file where search permission to the directory is denied to all.");
	testing("The directory mode is 776 but owned by bin user and group.");
	testing("Expect a return value of -1 and errno of EACCES.");
	start

		strcpy(path, data_path());
		strcat(path, "nosearchall");
		strcat(path, "/stat_tryme");

		expecting( EACCES );
		stat(path, &buf);

	finis



	done();
	/*NOTREACHED*/
}

void
check_stat(buf)

struct	stat	*buf;

{
	if ( ( buf->st_mode & MODE ) != (ushort)MODE )
	{
error("st_mode in error?.\nThe expected value was %d.\nThe actual value was %d.", MODE, (int)buf->st_mode & MODE);	/* ERROR */
	}

	if ( buf->st_ino <= (ino_t)0 )
	{
error("st_ino in error?.\nThe expected value was 0.\nThe actual value was %d.",  (int)buf->st_ino);	/* ERROR */
	}


	if ( buf->st_nlink != (short)1 )
	{
error("st_nlink in error?.\nThe expected value was 1.\nThe actual value was %d.", (int)buf->st_nlink);	/* ERROR */
	}

	if ( buf->st_uid != (ushort)UID )
	{
error("st_uid in error?.\nThe expected value was %d.\nThe actual value was %d.", UID,  (int)buf->st_uid);	/* ERROR */
	}

	if ( buf->st_gid != (ushort)GID )
	{
error("st_gid in error?.\nThe expected value was %d.\nThe actual value was %d.", GID,  (int)buf->st_gid);	/* ERROR */
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
