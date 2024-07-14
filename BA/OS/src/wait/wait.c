#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	CHD_STP		0177 	/* Child stopped */
# define	WAIT		1

#ident	"@(#)wait.c	1.7"

/*
** WAIT
**	Test the wait function
*/
main()
{
	int	pid;
	int	ret_val;
	int	statloc;

	setup("wait");

	funct("wait");

	exit_on ( ERROR );
	testing("Do a wait when no unwaited child process exists.");
	testing("Expect a return value of -1 and an errno of ECHILD.");
	start

		expecting ( ECHILD );
		ret_val = wait(&statloc);
	finis


	abstract("Create a child and have the child exit normally.");
	abstract("Expect a return value equal to the pid of the child and");
	abstract("the low order 8 bits of statloc equal to zero. The");
	abstract("high order 8 bits of statloc should be equal to what the");
	abstract("child passed to exit.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			testing("The child immediately calls exit(1).");
			start
				exit(1);
			finis
		}
		else
		{
			testing("The parent waits for the child.");
			testing("The parent also examines the exit status.");
			start
				expecting ( pid );
				wait(&statloc);

				if ( (statloc & 0xFF) != 0 )
				{
					error("Wait set the low order 8 bits of statloc incorrectly.\nThe expected value of statloc was %d.\nThe actual value of statloc was %d.\n", 0, (0xFF & statloc));	/* ERROR */
				}
				if ( (statloc & 0xFF00) != 0x100 )
				{
					error("Wait set the high order 8 bits of statloc incorrectly.\nThe expected value of statloc was %d.\nThe actual value of statloc was %d.\n", 0x100, (0xFF00 & statloc));	/* ERROR */
				}
			finis

		}

	finis

	abstract("Create a child & have the child exit normally.");
	abstract("Expect a return value equal to the pid of the child.");
	start
		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			testing("The child immediately calls exit(0).");
			start
				exit(0);
			finis
		}
		else
		{
			testing("The parent waits and expects to see pid.");
			start
				expecting ( pid );
				wait((int *)0);
			finis
		}

	finis


	abstract("Wait of child process killed with SIGKILL.");
	abstract("the return value is equal to the child pid and");
	abstract("statloc is equal to SIGKILL.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			prep("The child sleeps and waits to  be killed.");
			start
				sleep(60);
				exit(0);
			finis
		}
		else
		{

			testing("Parent kills the child, waits for its exit, and looks at the return value.");
			start
				expecting ( SUCCESS );
				kill(pid, SIGKILL);

				expecting ( pid );
				wait(&statloc);

				if ( statloc != SIGKILL )
				{
					error("Wait set statloc incorrectly.\nThe expected value of statloc was %d.\nThe actual value of statloc was %d.\n", SIGKILL, statloc);	/* ERROR */
				}
			finis
		}

	finis


	done();
	/*NOTREACHED*/

}
