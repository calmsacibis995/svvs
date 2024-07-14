#include	<sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	CHKEXIST	0
# define	MAXLEN		512


#ident	"@(#)tmpfile.c	1.5"

char *temp_path();

/*
** tmpfile
**	Test the tmpfile function.
*/
main()
{
	char		*file_name;
	FILE		*file_pointer;
	int		ret_val;
	int		fd[2];
	int		pid;
	int		wait_val;
	char		file2_name[MAXLEN];
	int		i;
	char		pathname[STRLEN];
	extern	char	Tempdir[];
	int		dup_stderr;

	extern	char	*tmpnam();
	extern	FILE	*tmpfile();

setup("tmpfile");
funct("tmpfile");
testing ("Call tmpnam with a NULL string to get the tmpfile name back.");
testing ("This call is necessary so that we can check the file later on to");
testing ("see if it is being written to or it is closed or it is deleted.");
start

	/*
	** First call tmpnam with a NULL string to get the
	** tmpfile name back.  This call is necessary so
	** that we can check the file later on to see if it
	** is being written to or it is closed or it is 
	** deleted.
	*/

	expecting (SUCCESS);
	file_name = tmpnam(NULL);
finis


testing("A temp file created using tmpfile should be writable.");
testing("Data written to the file should actually go there.");
start
	expecting (SUCCESS);
	file_pointer = tmpfile();

	expecting ('a');
	fputc('a', file_pointer);

	expecting (0L);
	fseek(file_pointer,0L,SEEK_SET);

	expecting ('a');
	fgetc(file_pointer);
finis

		

testing("When a temp file is closed it should be removed when it is closed.");
start
	expecting (SUCCESS);
	fclose(file_pointer);

	expecting (VOID);  /*  give a more meaningful error  */
	if ( (ret_val = access(file_name, CHKEXIST)) != -1 )
		error("The access call should indicate that\
the temp file was removed when closed");
finis



abstract ("When the process that opens the file exits, the");
abstract ("file is actually removed.");
abstract ("The file must be removed when the process that opened it");
abstract ("exits.  This is tested as follows:");
abstract ("A pipe is opened and a child is forked.");
abstract ("The child opens then closes a temp_file,");
abstract ("passes its name back to the parent via the pipe, and exits.");
abstract ("The parent checks that the temp file was deleted since ");
abstract ("it was passed the temp_file name.");
start
	/*
	** Create an interprocess channel
	*/
	expecting (SUCCESS);
	pipe(fd);

	/*
	** Have a child process open the temp file and exit
	** check to see if the file gets removed
	*/
	expecting (SUCCESS);
	pid = fork();

	if ( pid != 0 )
		wait_val = wait_for(pid,5);
	else
	{
		/*
		** CHILD PROCESS
		*/

		/*
		** First call tmpnam with a NULL string to get the
		** tmpfile name back.  This call is necessary so
		** that we can later on check the file to see if it
		** is being written to or it is closed or it is 
		** deleted.
		*/

		expecting (SUCCESS);
		file_name = tmpnam(NULL);
		
		file_pointer = tmpfile();
	
		/*
		** Close the read file descriptor for the 
		** child process and write the file name to
		** the read file for the parent process.
		*/
		close(fd[0]);

		write(fd[1], file_name,strlen(file_name));

		close(fd[1]);
		exit(0);
	}
	if ( wait_val )
		error ("Can not creat a temporary file using tmpfile");

	/*
	** Close the write file for the parent process
	*/
	expecting (SUCCESS);
	close(fd[1]);

	read(fd[0], file2_name, MAXLEN);

	close(fd[0]);

	expecting (VOID);  /*  give a more meaningful error  */
	if ( (ret_val = access(file2_name, CHKEXIST)) != -1 )
		error("The access call should indicate that\
the temp file was removed when closed");
finis


testing("tmpfile should return NULL if it cannot open any more files.");
start
	expecting (SUCCESS);
	file_name = tmpnam(NULL);
# ifdef SYSVR_1
/*
**	Redirects stderr to a file, but saves a copy
**	of the file descriptor attached to stderr so we can
**	restore it later.
*/
	expecting (SUCCESS);
	dup_stderr = dup(fileno(stderr));

	expecting (VOID);
	strcpy (pathname,temp_path());
	strcat (pathname,"err_out");

	expecting (VOID);
	if ( !freopen(pathname,"w",stderr) )
	{
		fclose(stderr);
		fdopen(dup_stderr,"w");
		error ("could not freopen(stderr)");
	}
	for (i = 4; i < OPEN_MAX; i++ ) /* would be loop from 3 but have */
					/* stderr saved in dup_stderr	 */
# else
	for (i = 3; i < OPEN_MAX; i++ )	/*Not release 1, stderr not */
					/*duped */
# endif
	{
		expecting (VOID);
		if ( (file_pointer = tmpfile()) == NULL )
			error ("Can not creat a temporary file using tmpfile, \
i = %d", i);
	}
		expecting (VOID);
		if ( (file_pointer = tmpfile()) != NULL )
			error ("Tmpfile did not fail when there are \
no more files to be opened");
# ifdef SYSVR_1
/*
**	Reset stderr back to what it used to point to.
*/
	expecting (SUCCESS);
	fclose(stderr);

	expecting (VOID);
	if ( dup(dup_stderr) != 2 )
		error ("dup of stderr back to 2 failed");
	if ( fdopen(2,"w") == NULL )
		error ("couldn't fdopen stderr");

	expecting (SUCCESS);
	close(dup_stderr);
# endif

finis
done();
/*NOTREACHED*/

}
