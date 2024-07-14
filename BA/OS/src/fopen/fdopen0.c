#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<fcntl.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fdopen0.c	1.5"

# define FILENAME	"fdopen.data"

# define NO_TRUNC	0
# define TRUNC		1

/*
** fdopen
**	Test the fdopen function.
**
**	This test is dependent upon:
**		
**		time()
**		<stdio.h>
**		fclose(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
**		<fcntl.h>
*/
main()
{
	int		fd;
	ttime		t;
	char		path[PATH_MAX];
	FILE		*ret_val;
	struct	stat	st_buf1;
	struct	stat	st_buf2;
	char		buf1[FILESIZ];
	char		buf2[FILESIZ];

	ttime		time();
	void		init_buf();
	void		put_file();
	void		get_file();
	int		cmp_file();
	void		cmp_stat();
	char		*data_path();

/*
**	attempt to fdopen() for "r", "r+", "w", "w+", "a", "a+"
*/


	setup("fopen");

	funct("fdopen");

	exit_on ( ERROR );

	abstract("Fdopen for read (r).");
	abstract("Expecting a non-null FILE pointer.");
	start

		prep("Initialize file name, and stat a data file.");
		start
			init_buf( buf1 );

			strcpy( path, data_path() );
			strcat( path, FILENAME );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue a call to open and fdopen.");
		start

			expecting ( SUCCESS );
			fd = open( path, O_RDONLY );

			expecting ( NON_NULL );
			ret_val = fdopen( fd, "r" );
		finis



		testing("Fdopen of existing file for read has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fdopen of existing file for read has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		cleanup("Fclose existing file opened for read,");
		cleanup("and unlink the file.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis


	abstract("Fdopen of existing file for append (a).");
	abstract("Expect back a non-null FILE pointer.");
	start

		prep("Initialize and stat the file.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

		finis

		testing("Call open and fdopen.");
		start
			expecting ( SUCCESS );
			fd = open( path, O_APPEND );

			expecting ( NON_NULL );
			fdopen( fd, "a" );
		finis


		testing("Fdopen of existing file for append has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fdopen of existing file for append has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		abstract("Fclose existing file opened for append.");
		cleanup("And unlink the file as well.");
		abstract("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis


	abstract("Fdopen of existing file for update (r+).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize and stat the file.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

		finis

		testing("Call open and fdopen.");
		start
			expecting ( SUCCESS );
			fd = open( path, O_RDWR );

			expecting ( NON_NULL );
			fdopen( fd, "r+" );
		finis


		testing("Fdopen of existing file for update has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fdopen of existing file for update has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		cleanup("Fclose existing file opened for update.");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );
		finis
	finis


	testing("Fdopen of existing file for append and update (a+).");
	testing("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize and stat the file.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

		finis

		testing("Call open and fdopen.");
		start
			expecting ( SUCCESS );
			fd = open( path, O_APPEND | O_RDWR );

			expecting ( NON_NULL );
			fdopen( fd, "a+" );
		finis


		testing("Fdopen of existing file for append and update has not ");
		testing("changed status of file.");
		testing("Expecting to see identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fdopen of existing file for append and update has not ");
		testing("changed contents of file.");
		testing("Expecting to see unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		cleanup("Fclose existing file opened for append and update");
		cleanup("And unlink the file as well.");
		cleanup("Expecting to see return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis


	abstract("Fdopen of existing file for write (w).");
	abstract("Expecting to see back a non-null FILE pointer.");
	start

		prep("Initialize and stat the file.  Wait a few seconds");
		prep("before performing the test.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

			expecting ( VOID );
			t = time( (ttime *)0) + 3l;

			expecting ( VOID );
			while (time( (ttime *)0) < t) ;

		finis

		testing("Call open and fdopen.");
		start
			expecting ( SUCCESS );
			fd = open( path, O_WRONLY | O_TRUNC );

			expecting ( NON_NULL );
			fdopen( fd, "w" );
		finis


		testing("Fdopen of existing file for write has changed status of file.");
		testing("Afterwards, the length of file to be 0, file modify and status change");
		testing("and times to be modified.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, TRUNC );

		finis


		cleanup("Fclose existing file opened for write");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis


	abstract("Fdopen of existing file for truncate and update (w+).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize and stat the file.  Wait a few seconds");
		prep("before performing the test.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

			expecting ( VOID );
			t = time( (ttime *)0) + 3l;

			expecting ( VOID );
			while (time( (ttime *)0) < t) ;

		finis

		testing("Call open and fdopen.");
		start
			expecting ( SUCCESS );
			fd = open( path, O_RDWR | O_TRUNC);

			expecting ( NON_NULL );
			fdopen( fd, "w+" );
		finis


		testing("Fdopen of existing file for truncate and update has changed");
		testing("status of file.");
		testing("Afterwards, length of file to be 0, file modify and status change");
		testing("and times to be modified.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, TRUNC );

		finis


		cleanup("Fclose existing file opened for truncate and update.");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis

	done();
	/*NOTREACHED*/
}
