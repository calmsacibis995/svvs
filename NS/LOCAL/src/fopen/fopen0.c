#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<fcntl.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fopen0.c	1.9"

# define FILENAME	"fopen.data"
# define BAD_PATH	"badpath.data/"
# define MISSING_DIR	"missing_dir/"
# define NO_SEARCH_DIR	"nosearch.dir/"
# define NO_WR_DIR	"nowrite.dir/"
# define DIR777		"fopen.dir/"
# define NO_READ_FILE	"noread.data"
# define NO_WR_FILE	"nowrite.data"


/*
** fopen0
**	Test the fopen function.
**
**	This test is dependent upon:
**		
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
	FILE		*ret_val;
	FILE		*r_val[OPEN_MAX];
	struct stat	st_buf1;
	struct stat	st_buf2;
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
**
**	First sequence of tests attempt to open existing files
**	for "r", "r+", "w", "w+", "a", "a+"
**/
	setup( "fopen" );

	funct("fopen");

	exit_on ( ERROR | SEVERR );

	abstract("Fopen existing file for read (r).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize file name and stat a data file.");
		start
			init_buf( buf1 );

			strcpy( path, data_path() );
			strcat( path, FILENAME );

			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

		finis

		testing("Call fopen.");
		start
			expecting ( NON_NULL );
			ret_val = fopen( path, "r" );

		finis


		testing("Fopen of existing file for read has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fopen of existing file for read has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
			{
				error("file contents differ");
			}

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


	abstract("Fopen of existing file for append (a).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize file name and stat a data file.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue the call to fopen.");
		start

			expecting ( NON_NULL );
			ret_val = fopen( path, "a" );
		finis


		testing("Fopen of existing file for append has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fopen of existing file for append has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
			{
				error("file contents differ");
			}

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


	abstract("Fopen of existing file for update (r+).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize file name and stat a data file.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );
		finis

		testing("Issue the call to fopen.");
		start
			expecting ( NON_NULL );
			ret_val = fopen( path, "r+" );
		finis


		testing("Fopen of existing file for update has not changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );
		finis


		testing("Fopen of existing file for update has not changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
			{
				error("file contents differ");
			}

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


	abstract("Fopen of existing file for append and update (a+).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize file name and stat a data file.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

		finis

		testing("Issue the call to fopen.");
		start
			expecting ( NON_NULL );
			ret_val = fopen( path, "a+" );


		finis


		testing("Fopen of existing file for append and update has not ");
		testing("changed status of file.");
		testing("Expecting identical stat buffers.");
		start

			expecting ( SUCCESS );
			stat( path, &st_buf2 );
			cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );

		finis


		testing("Fopen of existing file for append and update has not ");
		testing("changed contents of file.");
		testing("Expecting unchanged contents.");
		start

			get_file( path, buf2 );
			if (cmp_file( buf1, buf2 ) != FILESIZ)
			{
				error("file contents differ");
			}

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


	abstract("Fopen of existing file for write (w).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize file name and stat a data file.");
		prep("Wait a few seconds before performing the test.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

			expecting ( SUCCESS );
			t = time( (ttime *)0) + 3l;

			expecting ( SUCCESS );
			while (time( (ttime *)0) < t) ;

		finis

		testing("Issue the call to fopen.");
		start
			expecting ( NON_NULL );
			ret_val = fopen( path, "w" );
		finis


		testing("Fopen of existing file for write has changed status of file.");
		testing("length of file to be 0, and file mode and status change");
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


	abstract("Fopen of existing file for truncate and update (w+).");
	abstract("Expecting back a non-null FILE pointer.");
	start

		prep("Initialize file name and stat a data file.");
		prep("Wait a few seconds before performing the test.");
		start
			put_file( path, 0666, buf1 );

			expecting ( SUCCESS );
			stat( path, &st_buf1 );

			expecting ( SUCCESS );
			t = time( (ttime *)0) + 3l;

			expecting ( SUCCESS );
			while (time( (ttime *)0) < t)  ;
		finis

		testing("Issue the call to fopen.");
		start
			expecting ( NON_NULL );
			ret_val = fopen( path, "w+" );
		finis


		testing("Fopen of existing file for truncate and update has changed");
		testing("status of file.");
		testing("length of file to be 0, and file mode and status change");
		testing("times to be modified.");
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
	/*
**
**Next series of tests attempt to open new or
**non-existing file for "r", "r+", ... "a+"
**
*/

	abstract("Next series of tests attempt to open new or");
	abstract("non-existing file for 'r', 'r+', ... 'a+'.");
	start

		testing("Fopen non-existing file for read (r).");
		testing("FILE pointer of NULL and errno of ENOENT.");
		start

			expecting ( NULL );
			expecting ( ENOENT );
			ret_val = fopen( path, "r" );


		finis


		testing("Fopen of new file for append (a).");
		testing("Expecting back a non-null FILE pointer.");
		start

			expecting ( NON_NULL );
			ret_val = fopen( path, "a" );


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


		testing("Fopen of non-existing file for update (r+).");
		testing("FILE pointer of NULL and errno of ENOENT.");
		start

			expecting ( NULL );
			expecting ( ENOENT );
			ret_val = fopen( path, "r+" );
		finis

		testing("Fopen of new file for append and update (a+).");
		testing("Expecting back a non-null FILE pointer.");
		start

			expecting ( NON_NULL );
			ret_val = fopen( path, "a+" );
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


		testing("Fopen of new file for write (w).");
		testing("Expecting back a non-null FILE pointer.");
		start

			expecting ( NON_NULL );
			ret_val = fopen( path, "w" );
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


		testing("Fopen of new file for truncate and update (w+).");
		testing("Expecting back a non-null FILE pointer.");
		start

			expecting ( NON_NULL );
			ret_val = fopen( path, "w+" );
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
**Next series of tests attempt to generate error conditions.
**
*/

	abstract("The next series of tests attempt to generate error conditions.");
	start

		testing("Fopen of path with a non-directory component.");
		testing("Expecting a return of 0 and errno of ENOTDIR.");
		start

			strcpy( path, data_path() );
			strcat( path, BAD_PATH );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( ENOTDIR );
			ret_val = fopen( path, "r" );

		finis


		testing("Fopen of path with a missing directory component.");
		testing("Expecting a return of 0 and errno of ENOENT.");
		start

			strcpy( path, data_path() );
			strcat( path, MISSING_DIR );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( ENOENT );
			ret_val = fopen( path, "w" );

		finis


		testing("Fopen of path with a component which denies search permission.");
		testing("Expecting a return of NULL and errno of EACCES.");
		start

			strcpy( path, data_path() );
			strcat( path, NO_SEARCH_DIR );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = fopen( path, "r" );
		finis


		testing("Fopen of new file in path with a component which denies");
		testing("write permission.");
		testing("Expecting a return of NULL and errno of EACCES.");
		start

			strcpy( path, data_path() );
			strcat( path, NO_WR_DIR );
			strcat( path, FILENAME );

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = fopen( path, "w" );
		finis


		testing("Fopen of file which denies read permission.");
		testing("Expecting a return of NULL and errno of EACCES.");
		start

			strcpy( path, data_path() );
			strcat( path, NO_READ_FILE );

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = fopen( path, "r" );
		finis


		testing("Fopen of file which denies write permission.");
		testing("Expecting a return of NULL and errno of EACCES.");
		start

			strcpy( path, data_path() );
			strcat( path, NO_WR_FILE);

			expecting ( NULL );
			expecting ( EACCES );
			ret_val = fopen( path, "w" );
		finis


		testing("Fopen of directory for write (w).");
		testing("Expecting a return of NULL and errno of EISDIR.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = fopen( path, "w" );
		finis


		testing("Fopen of directory for append (a).");
		testing("Expecting a return of NULL and errno of EISDIR.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = fopen( path, "a" );
		finis


		testing("Fopen of directory for update (r+).");
		testing("Expecting a return of NULL and errno of EISDIR.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = fopen( path, "r+" );
		finis


		testing("Fopen of directory for truncate and update (w+).");
		testing("Expecting a return of NULL and errno of EISDIR.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = fopen( path, "w+" );
		finis


		testing("Fopen of directory for append and update (a+).");
		testing("Expecting a return of NULL and errno of EISDIR.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );

			expecting ( NULL );
			expecting ( EISDIR );
			ret_val = fopen( path, "a+" );
		finis



		testing("Fopen of OPEN_MAX+1 files for write (w).");
		testing("Expecting a return of NULL.");
		start

			strcpy( path, data_path() );
			strcat( path, DIR777 );
			strcat( path, FILENAME );

			prep("Consume OPEN_MAX-3 file descriptors.");
			start

				for( i = 0; i < (OPEN_MAX-3) ; i++ ) /* 0, 1, 2 taken */
				{
					expecting ( NON_NULL );
					r_val[i] = fopen( path, "w" );

					if ( r_val[i] == NULL )
						severr("NULL return value from fopen  before opening OPEN_MAX files");

				}
			finis

			/*
	**this used to return errno == EMFILE, but call may fail for 
	**one of two reasons, caught too many files open - gives EMFILE, 
	**or ran out of buffers, no errno.
	*/
			testing("Issue the call to fopen and look for the failure.");
			start
				expecting ( NULL );
				ret_val = fopen( path, "w" );
			finis


			cleanup("Release all those file descriptors.");

			start
				for( i = 0; i < (OPEN_MAX-3) ; i++ ) /* 0, 1, 2 taken */
				{
					expecting ( SUCCESS );
					fclose( r_val[i] );
				}

			finis
		finis
	finis
	done();
	/*NOTREACHED*/
}
