#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<string.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)feof0.c	1.5"

/*
** feof0
**	Test the feof function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fopen()
**		fread()
**		fclose()
*/

int	catch_sig();

main()
{
	char		ch;
	FILE		*fptr;

	setup( "feof" );

	funct("feof");

	exit_on ( ERROR );

	abstract("Fread of /dev/null.");
	abstract("Expect a non-zero return value from feof.");
	start

		prep("Open /dev/null and try to read from it.");
		prep("This should generate an error condition.");
		start
			expecting ( SUCCESS );
			fptr = fopen("/dev/null", "r+");

			expecting ( FAILURE );
			fread(&ch, 1, 1, fptr);
		finis

		testing("Call feof twice");
		testing("The return value from both calls should not be zero.");
		start
			expecting ( VOID );
			if (feof( fptr ) == 0)
				error("feof returned 0");

			expecting ( VOID );
			if (feof( fptr ) == 0)
				error("feof cleared error after first call to feof");
		finis


		cleanup("Close the open file.");
		start
			expecting ( SUCCESS );
			fclose( fptr );

		finis
	finis

	done();
}
