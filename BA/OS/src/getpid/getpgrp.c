#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getpgrp.c	1.4"

/*
** getpgrp
**	Test the getpgrp function.
**
** Testing this function is extremely difficult since an ID is a dynamically
** assigned variable issued by the kernel.
**
** The only sane test is to make sure the call was issued and returned
** with a sane value.
** Note: It is assumed that the group id is greater than 1.
**
** Error should not return 1 but a range in case of failure.
**
*/

main()
{
	int	ret_val;
	int	expected;

	int	parent_id;
	int	pid;
	int	fork();
	void	exit();

	setup("getpid");
	funct("getpgrp");

	exit_on ( ERROR );

	abstract("Child will check that his process group is the same as his parent.");
	abstract("Getpgrp to return the correct process group.");
	start

		testing("Get pgrp and determine that it's in range.");
		start
			expecting ( VOID );
			expected = 1;
			ret_val = getpgrp();
			parent_id = ret_val;

			if ( ret_val <= expected )
			{
				error("Incorrect return value from getpid. Id should be greater than 1. The returned value was %d.\n", ret_val);	/* ERROR */
			}
		finis

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if (pid > 0) 	/* parent */
			(void) wait_for(pid, 2);

		else	/* child */
		{
			testing("The child determines that his pgrp is in range and");
			testing("is the same as his parents.");
			start
				expecting ( VOID );
				expected = 1;
				ret_val = getpgrp();

				if (ret_val <= expected)
				{
					error("Incorrect return value from getpid. Id should be greater than 1.\nThe returned value was %d.\n", ret_val);	/* ERROR */
				}

				if ( parent_id != ret_val )
					error("Child's parent ID does not match version given to him.\nHis expected ID was %d.\nHis actual ID was %d.\n", parent_id, ret_val);

			finis
			exit(0);
		}
	finis

	done();
}
