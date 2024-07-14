#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#endif

#include	<signal.h>
#include	<stdio.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	EXIT0		0
# define	EXIT1		256
# define	EXIT2		512
# define	WAIT		5

FILE		*Write_Stream;
FILE		*Read_Stream;
extern FILE	*temp_fopen();

#ident	"@(#)exit.c	1.6"

/*
** exit
**	Test the process exit function.
*/

main()
{
	int		pid;
	int		oldppid;
	int		newppid;
	int		wait_val;
	char		*data;
	char		data2[100];
	ttime		t;
	ttime		time();

	setup("exit");

	funct("exit");

	exit_on ( ERROR );

	abstract("Have a child process exit.");
	abstract("The process dies normally.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			exit(0);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */
			finis

		}

	finis


	abstract("Have a child write to a tmp file and exit without closing it.");
	abstract("Upon exit the file should have been properly closed.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			prep("Child opens a temp file and initializes data.");
			start
				expecting ( SUCCESS );
				Write_Stream = temp_fopen("exit","w");
				data = "This is a test";
			finis

			testing("Child now writes to the file.");
			start
				expecting ( VOID );
				fwrite(data, 1, strlen(data), Write_Stream);
			finis

			exit(0);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			testing("And by examining the child's temp file.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */

				expecting ( SUCCESS );
				Read_Stream = temp_fopen("exit", "r");
				data = "This is a test";

				expecting ( strlen(data) );
				fread(data2, 1, strlen(data), Read_Stream);

				expecting( SUCCESS );
				fclose(Read_Stream);
			finis
		}

	finis


	abstract("Have several children exit with different values.");
	abstract("The parent should receive all the different exit values.");
	start

		prep("Fork off the children");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			exit(0);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT0 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */

			finis
		}

		expecting ( SUCCESS );
		pid = fork();

		if ( pid == 0 )
		{
			/* CHILD process */
			exit(1);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			testing("This is for the second child.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT1 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */
			finis

		}

		expecting ( SUCCESS );
		pid = fork();

		if ( pid == 0 )
		{
			/* CHILD process */
			exit(2);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			testing("This is for the third child.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT2 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */
			finis

		}

	finis


	abstract("Have a child spawn more children. Then have the first child exit.");
	abstract("The grand child should now have a different parent-id.");
	start

		continue_on ( ERROR );

		prep("Fork off child");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */

			prep("Fork off grandchild");
			start
				expecting ( SUCCESS );
				pid = fork();
			finis

			if ( pid == 0 )
			{
				testing("Grandchild obtains and examines ppid's.");
				start
					oldppid = getppid();
					/* GRAND CHILD process */

					expecting ( SUCCESS );
					t = time((ttime *)0) + 15l;

					expecting ( SUCCESS );
					while( time((ttime *)0) < t) ;
					newppid = getppid();

					if ( newppid == oldppid )
						error("The parent id of this child should have changed when the parent terminated. The old parent id was %d\n. The new parent id was%d\n", newppid, oldppid);

				finis
				exit(1);
			}
			else
			{
				expecting ( SUCCESS );
				t = time((ttime *)0) + 5l;

				expecting ( SUCCESS );
				while( time((ttime *)0) < t) ;
				exit(0);
			}
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT0 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */

				expecting ( SUCCESS );
				t = time((ttime *)0) + 10l;

				expecting ( SUCCESS );
				while( time((ttime *)0) < t) ;
			finis
		}

	finis


	funct("_exit");

	abstract("Have a child process call _exit.");
	abstract("The process hopefully dies.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			_exit(0);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);/* ERROR */

			finis
		}

	finis


	abstract("Have a child write to a tmp file and call _exit without closing it.");
	abstract("Upon calling _exit the file should not have been flushed with fclose.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis
		if ( pid == 0 )
		{
			/* CHILD process */

			prep("Child opens a temp file and initializes data.");
			prep("And writes data into the temporary file.");
			start
				expecting ( SUCCESS );
				Write_Stream = temp_fopen("_exit","w");
				data = "This is a test";

				expecting ( VOID );
				fwrite(data, 1, strlen(data), Write_Stream);
			finis
			_exit(0);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			testing("And by examining the child's temp file.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);/* ERROR */

				expecting ( NON_NULL );
				Read_Stream = temp_fopen("_exit", "r");

				data = "This is a test";

				expecting( VOID );
				if ( ( fread(data2, 1, strlen(data), Read_Stream)) == strlen(data) )
					/*return value*/
					error("The tmpfile '_exit' should have not been flushed with fclose when the child process terminated. An fread of the file returned %d\n. The fread should have returned %d\n", 0, strlen(data));

			finis

			cleanup("Close the temporary file.");
			start
				expecting ( SUCCESS );
				fclose(Read_Stream);
			finis
		}

	finis

	abstract("Have several children call _exit with different values.");
	abstract("Verify the parent receives all the different exit values.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			_exit(0);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT0 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);/* ERROR */

			finis
		}

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			_exit(1);
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT1 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);/* ERROR */

			finis
		}

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */
			_exit(2);
		}
		else
		{
			if ( (wait_val = wait_for(pid, WAIT)) != EXIT2 )
				error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);/* ERROR */

		}

	finis


	abstract("Have a child spawn more children. Then have the first child");
	abstract("call _exit.  Verify the grandchild has a different parent id.");
	start

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{

			/* CHILD process */
			prep("Fork off a grandchild");
			start
				expecting ( SUCCESS );
				pid = fork();
			finis

			if ( pid == 0 )
			{
				testing("Grandchild obtains and examines ppid's.");
				start
					oldppid = getppid();
					/* GRAND CHILD process */

					expecting ( SUCCESS );
					t = time((ttime *)0) + 15l;

					expecting ( SUCCESS );
					while( time((long *)0) < t) ;
					newppid = getppid();
					if ( newppid == oldppid )
						error("The parent id of this child should have changed when the parent terminated. The old parent id was %d\n. The new parent id was%d\n", newppid, oldppid);
				finis
				_exit(1);
			}
			else
			{
				expecting ( SUCCESS );
				t = time((ttime *)0) + 5l;

				expecting ( SUCCESS );
				while( time((ttime *)0) < t) ;
				_exit(0);
			}
		}
		else
		{
			testing("Determine success by looking at return value from wait.");
			start
				if ( (wait_val = wait_for(pid, WAIT)) != EXIT0 )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);/* ERROR */

				expecting ( SUCCESS );
				t = time((ttime *)0) + 10l;

				expecting ( SUCCESS );
				while( time((ttime *)0) < t) ;
			finis

		}
	finis
	done();
	/*NOTREACHED*/
}
