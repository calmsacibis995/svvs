#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<sys/times.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<errno.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


# define	EXIT0		0
# define	WAIT		5
# define	BIGBUFF		4096

FILE		*Write_Stream;
FILE		*Read_Stream;
char		PathPointer[STRLEN];
extern FILE	*temp_fopen();
extern FILE	*data_fopen();
int		length;

#ident	"@(#)fork0.c	1.11"


/*
** fork
**	Test the process fork function.
*/

int		Sig_Flag;

main(argc, argv, envp)
int	argc;
char	**argv;
char	**envp;
{
	int		ret_val;
	int		i;
	int		pid;
	int		child_id;
	int		wait_val;
	char		data2[STRLEN];
	int		data3;
	char		*data;
	char		envp_string[BIGBUFF];
	char		envp2_string[BIGBUFF];
	int		close_on_exec;
	int		close2_on_exec;
	int		arg = 0;
	int		parent_group;
	int		child_group;
	int		fildes;
	struct	stat	buf1, buf2;
	struct	stat	*child_buf = &buf2;
	struct	stat	*parent_buf = &buf1;
	int		parent_mask;
	int		child_mask;
	long		parent_limit;
	long		child_limit;
	long		long_var = 0l;
	int		parent_pid;
	int		child_pid;
	int		child_ppid;
	unsigned	time_left;
	int		child_process[CHILD_MAX];
	int		last_child;
	struct	tms	child_times_buf;
	struct	tms	*times_buf = &child_times_buf;
	FILE		*stream;

	setup("fork");

	funct("fork");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( SEVERR );

	abstract("Have a child process fork.");
	abstract("There should be  two processes.");
	abstract("Determine if the child ever existed by having it write to");
	abstract("a file and then have the parent look at the results.");
	start
		/* get parent pid for testing for proper forking in child process*/
		expecting ( VOID );
		parent_pid = getpid();

		prep("Fork the child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* verify that child process is different from parent process*/
			testing("Child checks that his pid is not his parents.");
			start
				expecting ( VOID );
				if ( getpid() == parent_pid)
				{
					severr("The child and parent process id's are the same");
				}


			finis

			prep("The child opens a temporary file and writes some data to it.");
			start
				/* CHILD process */
				expecting ( NON_NULL );
				Write_Stream = temp_fopen("fork", "w");

				data = "This is a test";

				expecting ( VOID );
				length = strlen(data);

				expecting ( SUCCESS );
				fwrite(data, 1, length, Write_Stream);

				expecting ( SUCCESS );
				fclose(Write_Stream);

			finis
			exit(0);
		}
		else
		{
			expecting ( VOID );
			if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */

			testing("The parent will now determine if the child did the actual work.");
			start
				expecting ( NON_NULL );
				Read_Stream = temp_fopen("fork", "r");

				data = "This is a test";

				expecting ( VOID );
				length = strlen ( data );

				expecting ( length );
				fread(data2, 1, length, Read_Stream);

				expecting ( VOID );
				length = strlen ( data );
				if ( strncmp(data2, data, length) != 0 )
					error("The child process and parent do not share the same file.");	/* ERROR */

				cleanup("Close the file.");
				start
					expecting ( SUCCESS );
					fclose(Read_Stream);
				finis
			finis
		}

	finis

	abstract("Do a legal fork.");
	abstract("Expect the child to have a copy of the parents open file descriptors.");
	start

		prep("Open a file.");
		start
			expecting ( NON_NULL );
			Write_Stream = temp_fopen("fork", "w");
		finis

		prep("Fork the child");
		start
			/* get parent pid for testing for proper forking in child process*/
			expecting ( VOID );
			parent_pid = getpid();

			expecting ( SUCCESS );
			pid = fork();
		finis
		if ( pid == 0 )
		{

			/* CHILD process */
			/* verify that child process is different from parent process*/

			testing("Child checks that his pid is not his parents.");
			start
				expecting ( VOID );
				if ( getpid() == parent_pid)
				{
					severr("The child and parent process id's are the same.");	/* HALT */
				}

			finis

			prep("Have the child write out to the file");
			start
				data = "This is a test";

				expecting ( VOID );
				length = strlen ( data );

				expecting ( VOID );
				if ( (ret_val = fwrite(data, 1, length, Write_Stream)) == 0 )
					error("Could not write to the tmpfile 'fork'.");	/* ERROR */
			finis
			exit(0);
		}
		else
		{
			expecting ( VOID );
			if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */

			testing("Now have the parent read what the child should have written");
			start
				expecting ( NON_NULL );
				Read_Stream = temp_fopen("fork", "r");

				data = "This is a test";

				expecting ( VOID );
				length = strlen ( data );

				expecting ( length );
				fread(data2, 1, length, Read_Stream);

				expecting ( VOID );
				if ( strncmp(data2, data, strlen(data)) != 0 )
					error("The child process wrote to a file but the parent can not find it.");	/* ERROR */

				cleanup("Close the file.");
				start
					expecting ( SUCCESS );
					fclose(Read_Stream);
				finis
			finis
		}

		expecting ( SUCCESS );
		fclose(Write_Stream);

	finis

	abstract("Do a legal fork.");
	abstract("Expect the return value from the fork to be the pid of the child.");
	start

		/* get parent pid for testing for proper forking in child process*/
		expecting ( SUCCESS );
		parent_pid = getpid();

		expecting ( SUCCESS );
		pid = fork();

		if ( pid == 0 )
		{
			/* CHILD process */

			/* verify that child process is different from parent process*/
			testing("Child checks that his pid is not his parents.");
			start
				expecting ( VOID );
				if ( getpid() == parent_pid)
				{
					severr("The child and parent process id's are the same.");	/* HALT */
				}
			finis

			prep("Have the child write out to the file");
			start
				expecting ( NON_NULL );
				Write_Stream = temp_fopen("fork", "w");

				expecting ( VOID );
				child_id = getpid();
				if ( (ret_val = fwrite((char *)&child_id, sizeof(size_t), 1, Write_Stream)) == 0 )
					warning("%s\nfwrite returned 0",ADVISORY);

				expecting ( SUCCESS );
				fclose(Write_Stream);
			finis

			exit(0);
		}
		else
		{

			expecting ( VOID );
			if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */

			testing("Now have the parent read what the child should have written");
			start
				expecting ( NON_NULL );
				Read_Stream = temp_fopen("fork", "r");

				expecting ( VOID );
				if ( (ret_val = fread(&data3, sizeof(size_t), 1, Read_Stream)) == 0 )
					error("The tmpfile 'fork' should have been written to in the child process.");	/* ERROR */

				if ( pid != data3 )
					warning("%s\nparent's pid does not match child's ppid",ADVISORY);
			finis

			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				fclose(Read_Stream);
			finis

		}

	finis

	abstract("Do a legal fork.");
	abstract("The child to inherit all the following: Environment, close_on_exec");
	abstract("flag, Signal handling settings, Set_user_id mode, Set_group_id mode, ");
	abstract("Process group, Current working directory, Root directory, Umask, ");
	abstract("and File size limit.");
	start

		/*
	** Parent environment.
	*/
		prep("Have the parent set up the environment.");
		start
			i = 0;
			envp_string[0] = '\0';
			while ( *(envp+i) != NULL )
			{
				strcat(envp_string, *(envp+i));
				i++;
			}
		finis


		/*
	** Parent close_on_exec flag
	*/

		prep("Have the parent set up the close on exec flag.");
		start
			expecting ( SUCCESS );
			fildes = temp_open("fork", O_RDWR , 0);

			expecting ( SUCCESS );
			close_on_exec = fcntl(fildes, F_GETFD, arg);

		finis

		/*
	** Signal handling setting
	*/

		prep("Have the parent set up some signals.");
		start
			expecting ( SUCCESS );
			abort_on(ERROR);
			signal(SIGHUP, SIG_DFL);

			expecting ( SUCCESS );
			signal(SIGUSR1, SIG_IGN);

			expecting ( SUCCESS );
			signal(SIGUSR2, SIG_DFL);
		finis

		/*
	** user_id mode bit, group_id mode bit, process group
	*/

		prep("Have the parent set up the user/group id, and process group.");
		start
			expecting ( VOID );
			parent_group = getpgrp();


			expecting ( SUCCESS );
			fstat(fildes, parent_buf);
		finis

	/*
	** Umask, file size limit
	*/

		prep("Have the parent set up a new umask.");
		start
			expecting ( VOID );
			parent_mask = umask(777);

			expecting ( VOID );
			(void) umask(parent_mask); /* restore the mask*/
		finis


		prep("Have the parent set up a new ulimit.");
		start
			expecting ( VOID );
			if ( (parent_limit = ulimit(1, long_var)) == -1 )
			{
				severr("Can't get file size limit");
			}

			/* get parent pid for testing for proper forking in child process*/
			expecting ( VOID );
			parent_pid = getpid();


		finis

		prep("Now fork off the child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* verify that child process is different from parent process*/
			expecting ( VOID );
			if ( getpid() == parent_pid)
			{
				severr("The child and parent process id's are the same ");
			}

			/* CHILD process */

			testing("The child will now determine if he has inherited everything.");
			/*
		** Environment.
		*/
			i = 0;
			envp2_string[0] = '\0';

			while ( *(envp+i) != NULL )
			{
				strcat(envp2_string, *(envp+i));
				i++;
			}

			if ( strcmp(envp_string, envp2_string) != 0 )
				error("The child did not inherit the parent environment. The parents environment was %s.\n The childs environment was %s\n", envp_string, envp2_string);	/* ERROR */

			/*
		** Close on exec flag
		*/
			expecting ( SUCCESS );
			close2_on_exec = fcntl(fildes, F_GETFD, arg);

			if ( (close_on_exec & 0x1) != (close2_on_exec & 0x1) )
				error("The child did not inherit the parent close_on_exec flag. The parents flag was %d.\n The childs flag was %d\n", (close_on_exec & 0x1), (close2_on_exec & 0x1));	/* ERROR */

			/*
		** Signal handling setting
		*/
			testing("Check the signal settings passed from the parent");
			start
				expecting ( SIG_DFL );
				signal(SIGHUP, SIG_IGN);

				expecting ( SIG_IGN );
				signal(SIGUSR1, SIG_DFL);

				expecting ( SUCCESS );
				(void)signal(SIGUSR2, SIG_DFL);
			finis
			/*
		** user_id mode bit, group_id mode bit, process group
		*/

			expecting ( VOID );
			if ( (child_group = getpgrp()) != parent_group )
				error("The child did not inherit the parent process group id. The parents process group was %d.\n The childs process group was %d.\n", parent_group, child_group);	/* ERROR */

			expecting ( SUCCESS );
			fstat(fildes, child_buf);


			if ( child_buf->st_mode != parent_buf->st_mode )
				error("The child did not inherit the parent set_user_id and set_group_id mode bits. The parents st_mode was %d.\n The childs st_mode was %d.\n", parent_buf->st_mode, child_buf->st_mode);	/* ERROR */

			/*
		** Umask, file size limit
		*/

			expecting ( parent_mask );
			child_mask = umask(777);

			expecting ( VOID );
			(void) umask(child_mask); /* restore the mask*/

			expecting ( parent_limit );
			ulimit(1, long_var);

			exit(0);
		}
		else
		{

			expecting ( VOID );
			if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */
		}

	finis

	abstract("Do a legal fork.");
	abstract("The child to have different values for all the following: ");
	abstract("Process id, Parent process id (same as the process id for the ");
	abstract("parent). Utime, stime, cutime, and cstime are all set to zero ");
	abstract("for the child and the alarm clock signal has been reset to zero.");
	start

		expecting ( VOID );
		parent_pid = getpid();

		prep("Set an alarm to go off.");
		prep("Chdir into  appropriate directory.");
		start
			expecting ( VOID );
			(void) alarm(10);

			/*
			** Check the current directory
			*/

			expecting ( NON_NULL );
			stream = data_fopen("fork", "w");

			expecting ( VOID );
			strcpy(PathPointer, data_path());
			PathPointer[strlen(PathPointer)-1] = 0;

			expecting ( SUCCESS );
			chdir(PathPointer);

			expecting ( SUCCESS );
			fclose(stream);
		finis

		/*
		** Now create a child.
		*/

		/* get parent pid for testing for proper forking in child process*/

		expecting ( VOID );
		parent_pid = getpid();

		prep("Fork off a child.");
		start
			expecting ( SUCCESS );
			pid = fork();
		finis

		if ( pid == 0 )
		{
			/* CHILD process */

			testing("Child looks at process times.");
			start
				expecting ( SUCCESS );
				times(times_buf);

				if ( times_buf->tms_utime != 0 )
				{
					warning(
"The utime was not set to zero in the child process.\n\
actual value was %d", times_buf->tms_utime);	/* WARNING */
				}
				if ( times_buf->tms_stime != 0 )
				{
					warning(
"The stime was not set to zero in the child process.\n\
actual value was %d", times_buf->tms_stime);	/* WARNING */
				}
				if ( times_buf->tms_cutime != 0 )
				{
					warning(
"The cutime was not set to zero in the child process.\n\
actual value was %d", times_buf->tms_cutime);	/* WARNING */
				}
				if ( times_buf->tms_cstime != 0 )
				{
					warning(
"The cstime was not set to zero in the child process.\n\
actual value was %d", times_buf->tms_cstime);	/* WARNING */
				}

			finis

			testing("Child looks at time left on alarm clock.");
			start

				expecting ( VOID );
				if ( (time_left = alarm(10)) != 0 )
				{
					warning(
"The amount of time left on the previously set alarm did not get reset to zero.\n\
%d was returned", time_left);	/* WARNING */
				}
			finis

			testing("Child examines process info.");
			start
				/* verify that child process is different
				 * from parent process
				 */
				expecting ( VOID );
				if ( (child_pid = getpid()) == parent_pid )
				{
					severr(
"The child and parent process ids are the same, both being %d", parent_pid);
/*
 *  This is a severe error since system test claims that a dummy
 *  fork routine that returns non-zero should fail
 */
				}


				expecting ( VOID );
				if ( (child_ppid = getppid()) != parent_pid )
					warning(
"%s\ngetppid returned %d, which does not match the parent pid of %d",
ADVISORY, child_ppid, parent_pid);
			finis

			/*
			** Try to access the data file just created above.
			*/
			testing("Child tries to open datafile.");
			start
				expecting ( NON_NULL );
				stream = fopen("datafork", "w");
			finis

			cleanup("Child closes file.");
			start
				expecting ( SUCCESS );
				fclose(stream);
			finis

			exit(0);
		}
		else
		{

			expecting ( VOID );
			if ( (wait_val = wait_for(pid, WAIT)) )
					error("Unexpected return value from child process wait. The expected wait value was %d\n. The found return value was %d.\n", 0, wait_val);	/* ERROR */
		}

	finis

	abstract("Attempt more than CHILD_MAX forks");
	abstract("Not to exceed CHILD_MAX forks, and");
	abstract("the return value from the fork be -1 and errno to be EAGAIN.");
	start

		prep("Start by consuming what should be all available process slots.");
		prep("Remember their names (pid's) for later cleanup.");
		start
			for ( i = 0; i <= CHILD_MAX; i++ )
			{
				/* get parent pid for testing for proper forking
						 in child process*/
				expecting ( VOID );
				parent_pid = getpid();

				expecting ( VOID );
				pid = fork();
				if ( pid == -1 )
				{
					break;
				}
				if ( pid == 0 )
				{
					/* verify that child process is different f
						rom parent process*/

					expecting ( VOID );
					if ( getpid() == parent_pid)
					{
						severr("The child and parent process id's are the same");
					}

					/* CHILD process */
					pause();
					exit(0);
				}
				child_process[i] = pid;
			}

		finis
		last_child = i - 1;
		if ( i >= CHILD_MAX )
			error("Forked CHILD_MAX or more times.");
		cleanup("Kill the child processes.");
		start
			for (i = 0; i <= last_child; i++)
			{
				expecting ( SUCCESS );
				kill(child_process[i], SIGKILL);
			}

		finis
	finis
	done();
	/*NOTREACHED*/
}
