#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)ferror0.c	1.3"

# define FILENAME	"ferror.data"

/*
** ferror0
**	Test the ferror function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fopen()
**		fwrite()
**		fclose()
*/

main()
{
	int		i;
	char		ch;
	char		path[PATH_MAX];
	FILE		*fptr;
	char		*data_path();
	long		oldulim;

	setup( "ferror" );

	funct("ferror");

	exit_on ( ERROR );

	abstract("Fwrite to file with ulimit set to 2.");
	abstract("Expect a non-zero return value from ferror.");
	start

		prep("Lower our ulimit");
		prep("Write a temporary file to its limit and then try to go over the boundary");
		start
			strcpy( path, data_path() );
			strcat( path, FILENAME );

			expecting ( SUCCESS );
			oldulim = ulimit( 1, 0L ); /* get current ulimit */

			expecting ( SUCCESS );
			ulimit( 2, 2L ); /* set new limit */

			expecting ( SUCCESS );
			fptr = fopen(path, "w");

			expecting ( VOID );
			for ( i = 0 ; i < 10000 ; i++ )
				if (fwrite(&ch, 1, 1, fptr) != 1)
					break;

			expecting ( SUCCESS );
			ulimit( 2, oldulim);

			if ( i == 10000 )
			{
				severr(" could not generate error condition ");
			}
		finis

		testing("Issue the call to ferror twice.");
		testing("The return value from both calls should not be zero.");
		start
			expecting ( VOID );
			if (ferror( fptr ) == 0)
				error("ferror returned 0");

			expecting ( VOID );
			if (ferror( fptr ) == 0)
				error("ferror cleared error after first call to ferror");
		finis

		cleanup("Close the file.");
		start

			expecting ( VOID );
			(void) fclose( fptr );

		finis
	finis

	done();
	/*NOTREACHED*/
}
