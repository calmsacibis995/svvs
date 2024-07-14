#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fopen3.c	1.3"

/*
** fopen
**	Test the fopen function.
**
**	This test is optional. This is for machines which
**	support pure text segments.
*/

main()
{
	char		path[PATH_MAX];

	int		unlink();

	setup("fopen");

	funct("fopen");

	exit_on ( ERROR );

	abstract("Fopen for write (w) a pure text segment that is being executed.");
	abstract("Expecting a return value of NULL and errno of ETXTBSY.");
	start


		if ( PURETEXT ) {
			strcpy(path, SVVSPATH);
			strcat(path, OS_BIN);
			strcat(path, "/fopen3");

			testing("Issue the call to fopen.");
			start
				expecting ( NULL );
				expecting ( ETXTBSY );
				fopen(path, "w");

			finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported");
		}

	finis


	abstract("Fopen for append (a) a pure text segment that is being executed.");
	testing("Expecting a return value of NULL and errno of ETXTBSY.");
	start


		if ( PURETEXT ) {
			testing("Issue the call to fopen.");
			start
				expecting ( NULL );
				expecting ( ETXTBSY );
				fopen(path, "a");
			finis

		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported");
		}
	finis


	abstract("Fopen for update (r+) a pure text segment that is being executed.");
	abstract("Expecting a return value of NULL and errno of ETXTBSY.");
	start


		if ( PURETEXT ) {
			testing("Issue the call to fopen.");
			start
				expecting ( NULL );
				expecting ( ETXTBSY );
				fopen(path, "r+");
			finis

		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported");
		}

	finis


	abstract("Fopen for append and update (a+) a pure text segment that is being executed.");
	abstract("Expecting a return value of NULL and errno of ETXTBSY.");
	start


		if ( PURETEXT ) {
			testing("Issue the call to fopen.");
			start
				expecting ( NULL );
				expecting ( ETXTBSY );
				fopen(path, "a+");
			finis

		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported");
		}
	finis


	abstract("Fopen for truncate and update (w+) a pure text segment that is being executed.");
	abstract("Expecting a return value of NULL and errno of ETXTBSY.");
	start


		if ( PURETEXT ) {
			testing("Issue the call to fopen.");
			start
				expecting ( NULL );
				expecting ( ETXTBSY );
				fopen(path, "w+");
			finis
		}
		else
		{
			message ("Test case suppressed - pure text segments are not supported");
		}

	finis


	done();
	/*NOTREACHED*/
}
