#include <sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<errno.h>
#include	<signal.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)pipe.c	1.11"

# ifndef	SIG_ERR
# define	SIG_ERR	(int(*)())-1
# endif

/*
** pipe
**	Test the pipe function.
**
**	This test has dependencies on:
**		read
**		write
**
*/

main()
{
	int	expected;
	int	ret_val;

	siggy	(*func)();
	int	fildes[2];
	int	files[OPEN_MAX*2];
	int	pid;
	char	buf[PIPE_MAX];
	int	i, j;
	int	pipe();
	int	read();
	int	write();
	int	catch_sig();
	void	exit();
	unsigned alarm();

	setup("pipe");

	funct("pipe");

	exit_on ( ERROR );

	abstract("Create a pipe.");
	abstract("Expect a  return value of zero.");
	start

		testing("Issue the call to pipe.");
		start
			expecting ( SUCCESS );
			pipe(fildes);
		finis

		cleanup("Close the open file descriptors.");
		start
			expecting ( SUCCESS );
			close(fildes[0]);

			expecting ( SUCCESS );
			close(fildes[1]);
		finis

	finis


	abstract("Pipe called repetitively to consume OPEN_MAX+1 file descriptors.");
	abstract("Expect a  return value of -1 and errno of EMFILE.");
	start
		prep("Consume all available file descriptors.");
		start
			for(i = 0, j = 0; i < (( OPEN_MAX -3 ) / 2 ); i++, j+=2)
			{


				expecting ( SUCCESS );
				pipe(&files[j]);

			}

		finis

		testing("The next call to pipe to generate an error.");
		start
			expecting( EMFILE );
			pipe(fildes);

		finis

		cleanup("Release all file descriptors.");
		start
			for(i = 0; i < (( OPEN_MAX -3 ) / 2); i++)
			{
				expecting ( SUCCESS );
				close(files[i]);
			}
		finis

	finis


	abstract("Reads and writes with all values of atomic write up to PIPE_MAX.");
	abstract("Read and write to return the number of bytes read and written.");
	abstract("We also try and make the pipe overflow by trying to write more than PIPE_MAX.");
	start

		testing("Open a pipe.");
		start
			expecting ( SUCCESS );
			pipe(fildes);
		finis

		testing("Issue reads and writes to the pipe.");
		start
			for (i = 1; i < PIPE_MAX; i++)
			{

				expecting( i );
				write(fildes[1], buf, (unsigned)i);

				expecting( i );
				read(fildes[0], buf, (unsigned)i);
			}
		finis


		testing("Read PIPE_MAX from a pipe that is PIPE_MAX/3 full;");
		testing("Read and write to return PIPE_MAX/3.");
		start
			expecting( PIPE_MAX/3 );
			write(fildes[1], buf, PIPE_MAX/3);

			expecting( PIPE_MAX/3 );
			read(fildes[0], buf, PIPE_MAX);
		finis


		testing("Write more than PIPE_MAX into a pipe.");
		testing("Hoping that the (child) process will block and not do the write.");
		start
			prep("Fork off a child.");
			start
				expecting ( SUCCESS );
				pid = fork();
			finis

			if (pid > 0) 	/* parent */
			{
				ttime t;
				ttime time();

				testing("The parent reads 1 byte from the pipe and sleeps.");
				start
					expecting ( 1 );
					read(fildes[0], buf, 1);

					t = time((ttime *)0) + 30;	/* sleep(30); */
					while(t>time((ttime *)0));
				finis
			}
/*
** The child will hopefully block when he tries to write into the pipe.
** If he does so then the parent will eventually die from old age.
** The child must terminate itself...it does so by setting an alarm clock
** and exiting. If he does not block his errors are reported and he exits.
*/
		else	/* child */
		{
			prep("The child sets up an alarm to go off in case of failure.");
			start
				expecting ( VOID );
				func = (siggy (*)())signal(SIGALRM, catch_sig);
		
				if ( func == (siggy (*)())SIG_ERR )
				{
					severr("pipe: Could not set sigalrm");
				}
		
				expecting ( VOID );
				(void) alarm((unsigned) 15);
			finis
		
			testing("The child writes one byte for the parent.");
			start
				expecting( 1 );
				write(fildes[1], buf, 1);
			finis
		
			testing("The child now tries to overflow the pipe.");
			testing("Hopefully he will block.");
			start
				expected = PIPE_MAX+1;
			finis

			expecting( PIPE_MAX+1 );
			ret_val = write(fildes[1], buf, PIPE_MAX+1);
		
		/*
		** Should never get past this point...but if it does
		*/
			error("Write did not block when PIPE_MAX+1 bytes were written?");
			error("The pipe overflowed when the data was written into the pipe?.\nWe expected to write %d bytes.\nWe actually wrote %d bytes.\n", PIPE_MAX+1, ret_val);	/* ERROR */
			done();
		}
		
		cleanup("Close the open file descriptors.");
		start
			expecting ( SUCCESS );
			close(fildes[0]);
		
			expecting ( SUCCESS );
			close(fildes[1]);
		finis
		
		finis
	finis
	done();
	/*NOTREACHED*/
}

catch_sig()
{
	exit(0);
}

