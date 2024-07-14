#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)freopen3.c	1.3"

# define STARTFILE	"startfile.data"

/*
** freopen
**	Test the freopen function.
**
**	This test is optional. This is for machines which
**	support pure text segments.
*/

main()
{
	char		path[PATH_MAX];
	char		start_path[PATH_MAX];
	char		buf1[FILESIZ];

	FILE		*fopen();
	FILE		*fptr;
	int		unlink();
	void		init_buf();
	void		put_file();
	char		*data_path();

	setup("fopen");
	funct("freopen");

	exit_on ( ERROR );

	abstract("Freopen for write (w) a pure text segment that is being executed.");
	abstract("Expect a value of NULL and errno of ETXTBSY.");
	start

		if ( PURETEXT ) {
		prep("Initialize buffers file names and data file.");
		start
			init_buf( buf1 );

			strcpy( start_path, data_path() );
			strcat( start_path, STARTFILE );

			put_file( start_path, 0666, buf1 );

			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/freopen3");
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NULL );
			expecting ( ETXTBSY );
			freopen(path, "w", fptr);

		finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported.");
		}

	finis


	abstract("Freopen for append (a) a pure text segment that is being executed.");
	abstract("Expect a value of NULL and errno of ETXTBSY.");
	start

		if ( PURETEXT ) {

		prep("Open the text file.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );
			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/freopen3");
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NULL );
			expecting ( ETXTBSY );
			freopen(path, "a", fptr);
		finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported.");
		}

	finis

	abstract("Freopen for update (r+) a pure text segment that is being executed.");
	abstract("Expect a value of NULL and errno of ETXTBSY.");
	start

		if ( PURETEXT ) {

		prep("Open the text file.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/freopen3");
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NULL );
			expecting ( ETXTBSY );
			freopen(path, "r+", fptr);
		finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported.");
		}

	finis


	abstract("Freopen for append and update (a+) a pure");
	abstract("text segment that is being executed.");
	abstract("Expect a value of NULL and errno of ETXTBSY.");
	start

		if ( PURETEXT ) {

		prep("Open the text file.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/freopen3");
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NULL );
			expecting ( ETXTBSY );
			freopen(path, "a+", fptr);
		finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported.");
		}

	finis


	abstract("Freopen for truncate and update (w+) a pure");
	abstract("text segment that is being executed.");
	abstract("Expect a value of NULL and errno of ETXTBSY.");
	start

		if ( PURETEXT ) {

		prep("Open the text file.");
		start
			expecting ( NON_NULL );
			fptr = fopen( start_path, "r" );

			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/freopen3");
		finis

		testing("Issue the call to freopen.");
		start
			expecting ( NULL );
			expecting ( ETXTBSY );
			freopen(path, "w+", fptr);
		finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported.");
		}

	finis

	done();
}
