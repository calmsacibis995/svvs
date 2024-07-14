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

#ident	"@(#)freopen0.c	1.8"

# define FILENAME	"fopen.data"
# define STARTFILE	"startfile.data"
# define BAD_PATH	"badpath.data/"
# define MISSING_DIR	"missing_dir/"
# define NO_SEARCH_DIR	"nosearch.dir/"
# define NO_WR_DIR	"nowrite.dir/"
# define DIR777		"fopen.dir/"
# define NO_READ_FILE	"noread.data"
# define NO_WR_FILE	"nowrite.data"


/*
** freopen
**	Test the freopen function.
**
**	This test is dependent upon:
**		
**		fopen(OS)
**		<stdio.h>
**		fclose(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
*/
main()
{
	int		i;
	ttime		t;
	char		path[PATH_MAX];
	char		start_path[PATH_MAX];
	FILE		*fptr;
	FILE		*ret_val;
	FILE		*r_val[OPEN_MAX];
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

	setup("fopen");
	funct("freopen");
/*
**
**	First sequence of tests attempt to open existing files
**	for "r", "r+", "w", "w+", "a", "a+"
**/
	funct("FREOPEN, open/creat streams.");

	exit_on ( ERROR );

	abstract("Freopen existing file for read (r).");
	abstract("The FILE pointer should match the current stream.");
	start

		prep("Initialize file names, open and stat data files.");
		start
			init_buf( buf1 );

			strcpy( start_path, data_path() );
			strcat( start_path, STARTFILE );

			put_file( start_path, 0666, buf1 );

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, FILENAME );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue the call to freopen.");
		start

			expecting ( NON_NULL );
			ret_val = freopen( path, "r", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		testing("Freopen of existing file for read has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Freopen of existing file for read has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		cleanup("Fclose existing file opened for read.");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis


	abstract("Freopen of existing file for append (a).");
	abstract("The FILE pointer should match the current stream.");
	start

		prep("Initialize file names, open and stat data files.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NON_NULL );
			ret_val = freopen( path, "a", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		testing("Freopen of existing file for append has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Freopen of existing file for append has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		cleanup("Fclose existing file opened for append.");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis


	abstract("Freopen of existing file for update (r+).");
	abstract("The FILE pointer should match the current stream.");
	start

		prep("Initialize file names, open and stat data files.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NON_NULL );
			ret_val = freopen( path, "r+", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		testing("Freopen of existing file for update has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start
			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );
		finis


		testing("Freopen of existing file for update has not changed contents of file.");
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

	abstract("Freopen of existing file for append and update (a+).");
	abstract("The FILE pointer should match the current stream.");
	start
		prep("Initialize file names, open and stat data files.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NON_NULL );
			ret_val = freopen( path, "a+", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		testing("Freopen of existing file for append and update has not ");
		testing("changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Freopen of existing file for append and update has not ");
		testing("changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
				error("file contents differ");

		finis


		cleanup("Fclose existing file opened for append and update");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis
	finis

	abstract("Freopen of existing file for write (w).");
	abstract("The FILE pointer should match the current stream.");
	start

		prep("Initialize file names, open and stat data files.");
		prep("Wait 3 seconds before calling freopen.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

			expecting ( SUCCESS );
			t = time( (ttime *)0) + 3l;

			expecting ( SUCCESS );
			while (time( (ttime *)0) < t) ;
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NON_NULL );
			ret_val = freopen( path, "w", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		testing("Freopen of existing file for write has changed status of file.");
		testing("length of file to be 0, and file modify and status change");
		testing("times to be modified.");
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

	abstract("Freopen of existing file for truncate and update (w+).");
	testing("The FILE pointer should match the current stream.");
	start

		prep("Initialize file names, open and stat data files.");
		prep("Wait a few seconds before performing the test.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

			expecting ( SUCCESS );
			t = time( (ttime *)0) + 3l;

			expecting ( SUCCESS );
			while (time( (ttime *)0) < t) ;
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NON_NULL );
			ret_val = freopen( path, "w+", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		testing("Freopen of existing file for truncate and update has changed");
		testing("status of file.");
		testing("length of file to be 0, and file modify and status change");
		testing("times to be modified.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, TRUNC );

		finis


		cleanup("Fclose existing file opened for truncate and update.");
		cleanup("And unlink the file as well.");
		testing("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );
		finis
	finis

	/*
**
**	Next series of tests attempt to open new or
**	non-existing file for "r", "r+", ... "a+"
**
*/


	abstract("Next series of tests attempt to open new or");
	abstract("'non-existing file for 'r', 'r+', ... 'a+'");
	start

		testing("Freopen a non-existing file for read (r)");
		testing("should return a FILE pointer of NULL with errno = ENOENT.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			expecting ( NULL );
			expecting ( ENOENT );
			ret_val = freopen( path, "r", fptr );
		finis


		testing("Freopen of new file for append (a)");
		testing("should return a FILE pointer that matches the current stream.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			expecting ( NON_NULL );
			ret_val = freopen( path, "a", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		cleanup("Fclose new file opened for append.");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );

		finis


		testing("Freopen of a non-existing file for update (r+)");
		testing("should return a FILE pointer of NULL with errno = ENOENT.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			expecting ( NULL );
			expecting ( ENOENT );
			ret_val = freopen( path, "r+", fptr );

		finis


		testing("Freopen of new file for append and update (a+).");
		testing("The FILE pointer should match the current stream.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			expecting ( NON_NULL );
			ret_val = freopen( path, "a+", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		cleanup("Fclose new file opened for append and update");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );
		finis


		testing("Freopen of new file for write (w).");
		testing("The FILE pointer should match the current stream.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			expecting ( NON_NULL );
			ret_val = freopen( path, "w", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		cleanup("Fclose new file opened for write");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );
		finis


		testing("Freopen of new file for truncate and update (w+).");
		testing("The FILE pointer should match the current stream.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			expecting ( NON_NULL );
			ret_val = freopen( path, "w+", fptr );

			if ( ret_val != fptr )
			{
				severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */
			}

		finis


		cleanup("Fclose new file opened for truncate and update.");
		cleanup("And unlink the file as well.");
		cleanup("Expecting a return of 0.");
		start

			expecting ( SUCCESS );
			fclose( ret_val );

			expecting ( SUCCESS );
			unlink( path );
		finis
	finis

	/*
**
**	Next series of tests attempt to generate error conditions.
**
*/


	abstract("Next series of tests attempt to generate error conditions.");
	start

		testing("Freopen of path with a non-directory component.");
		testing("Expecting a return of 0 and errno of ENOTDIR.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, BAD_PATH );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( ENOTDIR );
			ret_val = freopen( path, "r", fptr );
		finis


		testing("Freopen of path with a missing directory component.");
		testing("Expecting a return of 0 and errno of ENOENT.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, MISSING_DIR );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( ENOENT );
			ret_val = freopen( path, "w", fptr );

		finis

		testing("Freopen of path with a component which denies search permission.");
		testing("Expect to see a return of NULL and errno of EACCES.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, NO_SEARCH_DIR );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = freopen( path, "r", fptr );

		finis


		testing("Freopen of new file in path with a component which denies");
		testing("write permission.");
		testing("Expect to see a return of NULL and errno of EACCES.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, NO_WR_DIR );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = freopen( path, "w", fptr );
		finis


		testing("Freopen of file which denies read permission.");
		testing("Expect to see a return of NULL and errno of EACCES.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, NO_READ_FILE );

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = freopen( path, "r", fptr );
		finis


		testing("Freopen of file which denies write permission.");
		testing("Expect to see a return of NULL and errno of EACCES.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, NO_WR_FILE);

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = freopen( path, "w", fptr );
		finis


		testing("Freopen of directory for write (w).");
		testing("Expect to see a return of NULL and errno of EISDIR.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = freopen( path, "w", fptr );
		finis


		testing("Freopen of directory for append (a).");
		testing("Expect to see a return of NULL and errno of EISDIR.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = freopen( path, "a", fptr );
		finis


		testing("Freopen of directory for update (r+).");
		testing("Expect to see a return of NULL and errno of EISDIR.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = freopen( path, "r+", fptr );

		finis

		testing("Freopen of directory for truncate and update (w+).");
		testing("Expect to see a return of NULL and errno of EISDIR.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = freopen( path, "w+", fptr );

		finis

		testing("Freopen of directory for append and update (a+).");
		testing("Expect to see a return of NULL and errno of EISDIR.");
		start

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = freopen( path, "a+", fptr );
		finis

		testing("Freopen of OPEN_MAX files for write (w).");
		testing("Expect to see a return of stream.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );
			strcat( path, FILENAME );


			for( i = 0; i < (OPEN_MAX-3) ; i++ ) /* 0, 1, 2 taken */
			{
				expecting ( NON_NULL );
				fptr = fopen( start_path, "r" );

				expecting ( NON_NULL );
				r_val[i] = freopen( path, "w", fptr );

				if ( r_val[i] != fptr )
					severr("Return value from freopen does not match stream or errno set.\nExpected value from freopen should have been %x.\nThe acutal value returned was %x.\n", fptr, ret_val);	/* HALT */

			}

			for( i = 0; i < (OPEN_MAX-3) ; i++ ) /* 0, 1, 2 taken */
			{
				expecting ( SUCCESS );
				fclose( r_val[i] );
			}

		finis
	finis

	done();
}
