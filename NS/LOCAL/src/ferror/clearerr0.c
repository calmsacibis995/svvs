#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)clearerr0.c	1.3"

# define FILENAME	"clearerr.data"

/*
** clearerr0
**	Test the clearerr function.
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
	long		oldulim;
	char		*data_path();
	long		ulimit();

	setup( "ferror" );

	funct("clearerr");

	exit_on ( ERROR | SEVERR );

	abstract("Fwrite to file with ulimit set to 2.");
	abstract("Expect a zero return value from ferror after clearerr.");
	start

		strcpy( path, data_path() );
		strcat( path, FILENAME );

		prep("Lower our ulimit");
		prep("Write a temporary file to its limit and then try to go over the boundary");
		start
			expecting ( SUCCESS );
			oldulim = ulimit( 1, 0L );

			expecting ( SUCCESS );
			ulimit( 2, 2L );

			expecting ( SUCCESS );
			fptr = fopen(path, "w");

			expecting ( VOID );
			for ( i = 0 ; i < 10000 ; i++ )
				if (fwrite(&ch, 1, 1, fptr) != 1)
					break;

			expecting ( SUCCESS );
			ulimit( 2, oldulim);
		finis

		if ( i == 10000 )
		{
			severr(" could not generate error condition ");
		}

		testing("Call ferror, clearerr, and ferror and look at the return values.");
		start
			expecting ( VOID );
			if (ferror( fptr ) == 0)
				error("ferror returned 0");

			(void) clearerr( fptr );

			if (ferror( fptr ) != 0)
				error("clearerr did not clear ferror");
		finis

		cleanup("Close the file.");
		start

			expecting ( SUCCESS );
			(void) fclose( fptr );

		finis
	finis

	done();
	/*NOTREACHED*/
}
