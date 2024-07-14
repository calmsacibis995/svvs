#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getpid0.c	1.3"

/*
** getpid
**	Test the getpid function.
**
** Testing this function is extremely difficult since an ID is a dynamically
** assigned variable issued by the kernel.
**
** The only sane test is to make sure the call was issued and returned
** with a sane value.
**
*/

main()
{
	int	ret_val;
	int	expected;

	int	getpid();

	setup("getpid");
	funct("getpid");

	exit_on ( ERROR );

	abstract("Getpid returns a valid process ID.");
	abstract("A return value of >1 and less than PID_MAX.");
	start


		expecting ( VOID );
		ret_val = getpid();

		if ((ret_val <= expected) || (ret_val > PID_MAX))
		{
			error("Incorrect return value from getpid.\nExpected range is >1 and less than PID_MAX.\nThe returned value was %d.\n",  ret_val);	/* ERROR */
		}

	finis


	done();
	/*NOTREACHED*/
}
