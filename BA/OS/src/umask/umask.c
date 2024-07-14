#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)umask.c	1.3"

# define	MAX_MASK	256	/* 2^9 possible combinations */

/*
** umask
**	Test the umask function.
**
**	Cycle through all the values that umask may have.
**
**	Several initial calls to umask are required since we
**	don't know for sure what the processes default
**	umask is.
*/
main()
{
	int	prev_mask;
	int	mask;
	int	umask();


	setup("umask");

	funct("umask");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	abstract("All possible values of umask/cmask.");
	abstract("Cycle through all possible combinations of umask and");
	abstract("have the previous umask value returned.");
	abstract("Expect return values to be >0, and errnos of NOERR.");
	start

		prep("Set umask to an initial value.");
		start
			expecting ( VOID );
			prev_mask = umask(0);

			expecting ( VOID );
			prev_mask = umask(0);
		finis

		testing("Cycle through the masks.");
		start
			for(mask= 1; mask != MAX_MASK; mask++)
			{
				expecting( prev_mask );
				umask(mask);

				prev_mask = mask;
			}
		finis
	finis

	done();
	/*NOTREACHED*/
}
