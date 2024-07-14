#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<string.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fileno0.c	1.4"

/*
** fileno0
**	Test the fileno function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fopen()
**		fclose()
*/

int	catch_sig();

main()
{
	FILE		*fptr;
	int		fd;

	setup( "ferror" );

	funct("fileno");

	exit_on ( ERROR );

	testing("A fileno greater than or equal to 0 and less than OPEN_MAX.");
	start

		expecting ( SUCCESS );
		fptr = fopen("/dev/null", "r+");

		expecting ( VOID );
		fd = fileno( fptr );


		if (fd < 0 || fd >= OPEN_MAX )
			error("fileno return value out of range");

		expecting ( SUCCESS );
		fclose( fptr );

	finis

	done();
}
