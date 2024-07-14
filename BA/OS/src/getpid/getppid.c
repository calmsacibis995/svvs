#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)getppid.c	1.4"

/*
** getppid
**	Test the getppid function.
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
	int	getppid();
	void	exit();

	setup("getpid");
	funct("getppid");

	exit_on ( ERROR );

	abstract("Child will check that his parent ID is correct.");
	abstract("Getppid to return the correct parent process.");
	start

		testing("Get pid and make sure it's in range.");
		start
			expecting ( VOID );
			expected = 1;
			ret_val = getpid();
			parent_id = ret_val;

			if ((ret_val <= expected) || (ret_val > PID_MAX))
			{
				error("Incorrect return value from getpid.\nExpected range of values between 1 and PID_MAX.\nThe returned value was %d\n", ret_val);	/* ERROR */
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
			testing("Child determines that his pid is in range and not that");
			testing("of his parent.");
			start
				expecting ( VOID );
				expected = 1;
				ret_val = getppid();

				if ((ret_val <= expected) || (ret_val > PID_MAX))
				{
					error("Incorrect return value from getpid.\nExpected range of values between 1 and PID_MAX.\nThe returned value was %d\n", ret_val);	/* ERROR */
				}

				if ( parent_id != ret_val )
				{
					error("Child's parent ID does not match version given to him.\nHis expected ID was %d.\nHis actual ID was %d.\n", parent_id, ret_val);	/* ERROR */
				}

			finis
			exit(0);
		}
	finis

	done();
}
