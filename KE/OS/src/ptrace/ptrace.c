#include	<sv_base.h> /*replacement macros */
#ifndef	SYSVR_2
#include	<sys/types.h>
#endif
#include	<signal.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	REQ0	0
# define	REQ1	1
# define	REQ2	2
# define	REQ3	3
# define	REQ4	4
# define	REQ5	5
# define	REQ6	6
# define	REQ7	7
# define	REQ8	8
# define	REQ9	9

# define	DATA1		12345
# define	DATA2		0x1723	/* a most random number */
# define	CHILD_STOPPED	0177

# define	MAXNUM		10
# define	KNOWNFUNC	signal_func
# define	WAIT	5

int		signal_func();
int		signal2_func();

#ident	"@(#)ptrace.c	1.8"


/*
** ptrace
**	Test the process trace function.
*/
/*
** In the following the CHILD process will
** perform a ptrace with request = 0 to
** allow the parent process to trace this
** child. The last 3 arguments passed to
** ptrace are ignored.  No return value is
** defined for this call.
*/

int		Sig_Flag;

main()
{
# ifdef PTRACE
	int		ret_val1;
	int		ret_val2;
	int		ret_val3;
	int		pid;
	int		wait_val;
	int		word = DATA1;
	char		*address;
	extern	siggy	(*signal())();

setup("ptrace");
funct("ptrace");

testing("Use ptrace with a request of 1 and a known function address.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		kill(getpid(), SIGUSR1);
		exit(0);
/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");


		ptrace(REQ1, pid, (int *)KNOWNFUNC, 0);

		expecting (SUCCESS);
		kill(pid, SIGKILL);

		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally");
	}
finis


testing("Use ptrace with a request of 2 and a known address.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		kill(getpid(), SIGUSR1);
		exit(0);
		/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);  /*  give explicit errors  */
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");
		address = (char *)&word;

		if ( (ret_val2 = ptrace(REQ2, pid, (int *)address, 0)) == -1 )
			error("Ptrace(2) to a known address failed");

		if ( kill(pid, SIGKILL) == -1 )
			error ("can not kill the child process.");
		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally, returning 0%o",
				wait_val);
	}
	if (ret_val2 != DATA1) 
		error("Ptrace(2) returned %d, %d was expected",
			ret_val2, DATA1);
finis

			
testing("Use ptrace with a request of 3 to read the USER area.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		kill(getpid(), SIGUSR1);
		exit(0);
		/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");

		address = (char *)0;

		expecting (VOID);
		ptrace(REQ3, pid, (int *)address, 0);

		expecting (SUCCESS);
		kill(pid, SIGKILL);

		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally");
	}
finis


if (PURETEXT)
{
testing("Using ptrace with a request of 4 to write to a known pure text");
testing("address. This address is in an active procedure space.");
testing("The call should fail since the pure procedure address is shared");
testing("by both the parent and the child process.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		kill(getpid(), SIGUSR1);
		exit(0);
	}
	else
	{
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");

		expecting (VOID);
		ptrace(REQ4, pid, (int *)KNOWNFUNC, DATA1);

		expecting (SUCCESS);
		kill(pid, SIGKILL);

		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally");
	}
finis

}  /*  end of PURETEXT test  */


testing("Use ptrace with a request of 5 to write to a known address.");
testing("Then use ptrace(2) to see if this address was changed.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		kill(getpid(), SIGUSR1);
		exit(0);
		/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");

		address = (char *)&word;
		if ( (ret_val2 = ptrace(REQ5, pid, (int *)address, DATA2)) != DATA2 )
			error("%s\n%d was returned\n%d was expected",
	"Ptrace with a request of 5 to write to a known address failed",
	ret_val2, DATA2);

		if ( (ret_val3 = ptrace(REQ2, pid, (int *)address, 0)) == -1 )
			error("%s\n%d was returned\n-1 was expected",
	"Ptrace(2) to a known func address failed",
	ret_val3);
		if ( ret_val3 != DATA2 )
			error("%s\n%d was written\n%d was read back",
	"Data written to an address is not the same as what is read back.",
	DATA2, ret_val3);

		expecting (SUCCESS);
		kill(pid, SIGKILL);

		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally");
	}
finis


testing("Use ptrace with a request of 6 to write the USER area.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		kill(getpid(), SIGUSR1);
		exit(0);
		/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");

		address = (char *)0;
		ptrace(REQ6, pid, (int *)address, 0);

		expecting (SUCCESS);
		kill(pid, SIGKILL);

		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally");
	}
finis


testing("Using ptrace(7) with a data of 0");
testing("should cause the child process to restart without any signals");
testing("being sent and a return value of 0.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		signal(SIGUSR1, signal_func);
		kill(getpid(), SIGUSR1);
		exit(0);
		/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");
		if ( (ret_val1 = ptrace(REQ7, pid, (int *)1, 0)) != 0 )
			error("%s\n%d was returned, 0 was expected",
			 "Ptrace(7) with a data of 0 returned the wrong value.",
			 ret_val1);
		if ( (wait_val = wait_for(pid, WAIT)) )
			error ("Child terminated abnormally");
	}
finis


testing("Using ptrace(7) with a data of SIGINT");
testing("should return SIGINT and the child should start");
testing("running again and receive the signal SIGINT.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		/* CHILD process */
		expecting (VOID);
		Sig_Flag = 0;
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		signal(SIGINT, signal2_func);
		kill(getpid(), SIGUSR1);
		if ( Sig_Flag != 1 )
		{
			error("Must have received the signal %d", Sig_Flag);
			exit(1);
			/*NOTREACHED*/
		}
		else
			exit(0);
			/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");
		if ( (ret_val1 = ptrace(REQ7, pid, (int *)1, SIGINT)) != SIGINT )
			error("%s\nreturned %d, %d was expected",
				"Ptrace(7) with a data of SIGINT",
				ret_val1, SIGINT);
		if ( (wait_val = wait_for(pid, WAIT)) != 0 )
		{
			if ( (wait_val & CHILD_STOPPED) != 0 )
				error ("child exited abnormally");
			/*****  no longer used  *****
			 *  else
			 *	set_exit(ERROR);
			 *****/
		}
	}
finis


testing("ptrace(8) should cause the child to terminate.");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("Could not fork from ptrace()");

	if ( pid == 0 )
	{
		ttime t;
		ttime time();

		/* CHILD process */
		expecting (VOID);
		ptrace(REQ0, 0,(int *)0,0);

		expecting (SUCCESS);
		signal(SIGUSR1, signal_func);
		kill(getpid(), SIGUSR1);
		t = time((ttime *)0) + 15 ;
		while( t > time((ttime *)0));	/* sleep(15) */
		error ("Child was not terminated by parent process");
		exit(1);
		/*NOTREACHED*/
	}
	else
	{
		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");
		ptrace(REQ8, pid, (int *)0, 0);

		if (wait_val = wait_for(pid, WAIT))
		{
			if ( (wait_val & CHILD_STOPPED) == CHILD_STOPPED )
			{
				error("%s\nwait_val = %o",
		"Ptrace(8) causes the child to stop rather than terminate.",
		wait_val);
				expecting (VOID);
				kill(pid, SIGKILL);
			}
		}
	}
finis


testing("Using ptrace to do an exec(OS)");
testing("should cause ptrace to be in effect across an exec(OS).");
start
	expecting (SUCCESS);
	pid = fork();
	if ( pid == -1 )
		error ("can not fork");

	if ( pid == 0 )
	{
		execl("./ptrace1","ptrace1",0);
		error ("can not find program ptrace1");
	}
	else
	{
		if ( ((wait_val = wait_for(pid, WAIT)) & CHILD_STOPPED) != CHILD_STOPPED )
			error ("The child process did not stop.");
		expecting (VOID);
		ptrace(REQ1, pid, (char *)&word, 0);

		expecting (SUCCESS);
		kill(pid, SIGKILL);

		expecting (VOID);
		if ( ((wait_val = wait_for(pid, WAIT)) & SIGKILL) != SIGKILL )
			error ("Child terminated abnormally");
	}
finis


done();
/*NOTREACHED*/

}
/*
**
*/
signal_func()
{
	error("Ptrace failed -- This function should never be called");
}
/*
**
*/
signal2_func()
{
	Sig_Flag = 1;
#else
	void exit();
	
	unsupported("Process tracing is not supported.");
	exit(0);


#endif
}
