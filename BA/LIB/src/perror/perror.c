#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	STDERR_FD	2
# define	MAXSYSNERR	20000


#ident	"@(#)perror.c	1.3"



extern	FILE	*temp_fopen();

/*
** perror
**	Test the perror function.
*/
main()
{
	FILE		*stream;
	int		num_error;
	char		ret_buf[STRLEN];
	int		nbyte;
	char		*sp;
	char		*bp;

	extern	char	*sys_errlist[];
	extern	int	errno;
	extern	int	sys_nerr;


setup("perror");
funct("perror");

prep ("Close the STDERR file, replacing it with a temp file");
prep ("so that error messages may be read in for comparison.");
start
	expecting (SUCCESS);
	fclose(stderr);
	expecting (VOID);
	close(STDERR_FD);  /*  just to be sure  */

	expecting (SUCCESS);
	stream = temp_fopen("tmpperror", "w");
finis


testing("sys_nerr should be a reasonable value.");
start
	if ( sys_nerr < 0 || sys_nerr > MAXSYSNERR )
		error("unacceptable size for sys_nerr = %d", sys_nerr);
finis


testing("There are strings in sys_errlist[]");
testing("for each possible errno.");
testing("A string should be printed on stderr.");
start
	nbyte = STRLEN;
	for (num_error = 1; num_error < sys_nerr; num_error++)
	{
		errno = num_error;
		expecting (SUCCESS);
		perror("Testing perror");
		fclose(stream);
		stream = temp_fopen("tmpperror", "r");
		ret_buf[0] = '\0';
		expecting (VOID);
		if ( fgets(ret_buf, nbyte, stream) != NULL )
		{
			sp = "Testing perror: ";
			expecting (VOID);
			if ( strncmp(ret_buf,sp,strlen(sp)) )
			{
				ret_buf[strlen(sp)] = '\0';
				error ("perror did not prepend the correct \
string,\nstring = '%s', should begin with '%s'", ret_buf, sp );
			}
			if ( strncmp(&ret_buf[strlen(sp)],
				sys_errlist[num_error],
				strlen(sys_errlist[num_error]))
				!= 0)
			{
				error ("wrong string for this errno = '%s',\n\
'%s' was expected", &ret_buf[strlen(sp)], sys_errlist[num_error]);
			}
			bp = &ret_buf[strlen(sp) +
				strlen(sys_errlist[num_error])];
			if ( *bp != '\n' )
			{
				error ("Perror did not put a new line at the \
end of the string");
			}
			if ( bp[1] != '\0' )
			{
				error ("Perror did not end the string at the \
carriage return");
			}
		}
		else
		{
			error ("perror did not write anything onto stderr");
		}
		expecting (SUCCESS);
		fclose(stream);
		stream = temp_fopen("tmpperror","w");
	}
finis

		

testing("perror of all the errno's");
testing ("should print the associated string on stderr.");
start
	nbyte = STRLEN;
	for (num_error = 1; num_error < sys_nerr; num_error++)
	{
		errno = num_error;
		expecting (SUCCESS);
		perror("Testing perror");
		fclose(stream);
		stream = temp_fopen("tmpperror", "r");
		ret_buf[0] = '\0';

		expecting (VOID);
		if ( fgets(ret_buf, nbyte, stream) != NULL )
		{
			sp = "Testing perror: ";
			if ( strncmp(ret_buf,sp,strlen(sp)) )
			{
				ret_buf[strlen(sp)] = '\0';
				error ("perror did not prepend the correct \
string\n'%s' was returned, '%s' was expected", ret_buf, sp);
			}
			if ( strncmp(&ret_buf[strlen(sp)],
				sys_errlist[num_error],
				strlen(sys_errlist[num_error]))
				!= 0)
			{
				error ("Incorrect string '%s'\n\
was written to the file perror\n'%s' was expected",
				&ret_buf[strlen(sp)],
				sys_errlist[num_error]);
			}
			bp = &ret_buf[strlen(sp) +
				strlen(sys_errlist[num_error])];
			if ( *bp != '\n' )
			{
				error ("Perror did not put a new line at the \
end of the string");
			}
			if ( bp[1] != '\0' )
			{
				error ("Perror did not end the string at the \
carriage return");
			}
		}
		else
		{
			error ("perror did not write anything onto stderr");
		}
		expecting (SUCCESS);
		fclose(stream);
		stream = temp_fopen("tmpperror","w");
	}
finis

		

testing("perror of a NULL string should yield a string printed on stderr.");
start
	nbyte = STRLEN;
	num_error = errno = 1;

	expecting (SUCCESS);
	perror("");
	fclose(stream);
	stream = temp_fopen("tmpperror", "r");
	ret_buf[0] = '\0';

	expecting (VOID);
	if ( fgets(ret_buf, nbyte, stream) != NULL )
	{
		if ( strncmp(ret_buf, sys_errlist[num_error],
			strlen(sys_errlist[num_error]))
			!= 0 )
		{
			error ("Incorrect string '%s' was written \
to the error file",ret_buf);
		}

		expecting (SUCCESS);
		fclose(stream);
	}
finis

done();
/*NOTREACHED*/
}
