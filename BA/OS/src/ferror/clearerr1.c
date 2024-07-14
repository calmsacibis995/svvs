#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<string.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)clearerr1.c	1.4"

/*
** clearerr1
**	Test the clearerr function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fopen()
**		fread()
**		fclose()
*/

main()
{
	char		ch;
	FILE		*fptr;

	setup( "ferror" );

	funct("clearerr");

	exit_on ( ERROR | SEVERR );

	abstract("Fread of /dev/null.");
	abstract("Expect a zero return value from feof after clearerr.");
	start


	prep("Open /dev/null and try to read from it.");
	prep("This should fail.");
	start
		expecting ( SUCCESS );
		fptr = fopen("/dev/null", "r+");

		expecting ( FAILURE );
		fread(&ch, 1, 1, fptr);

	finis

	testing("Call feof, and then clearerr to clear the error condition.");
	start
		expecting ( VOID );
		if (feof( fptr ) == 0)
			error("feof returned 0");

		expecting ( VOID );
		clearerr( fptr );

		if (feof( fptr ) != 0)
			error("feof not cleared after call to clearerr");

	finis

	cleanup("Close the open file.");
	start
		expecting ( SUCCESS );
		fclose( fptr );
	finis

	finis

	done();
	/*NOTREACHED*/
}
