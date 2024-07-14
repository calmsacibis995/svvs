#include	<sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<fcntl.h>
#include	<stdio.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)fflush0.c	1.6"

# define	FILENAME	"fflush.data"
# define	ULIM		(long)1024*16

/*
** fflush0
**	Test the fflush function.
**
**	This test is dependent upon:
**		<stdio.h>
**		<fcntl.h>
**		fopen()
**		fwrite()
**		open()
**		creat()
**		write()
**		read()
**		close()
**		unlink()
**		
*/
main() {
	char		ch;
	int 		fd;
	int		ret_val;
	FILE 		*fptr;
	FILE		*fopen();
	char		path[PATH_MAX];
	char		buf[BUFSIZ+1];
	void		init_buf();
	void		put_file();
	long		ulimit();
	long		l;

	char		*temp_path();

	setup("fclose");

	exit_on ( ERROR );

	abstract("Fflush of file opened for append");
	abstract("should return 0, flushing the data.");
	start

		prep("Close the temporary file from the previous case.");
		prep("Re-open it and write some data to it.");

		start
			strcpy(path, temp_path());
			strcat(path, FILENAME );

			expecting ( VOID );
			(void) unlink( path );

			expecting ( SUCCESS );
			fptr=fopen(path, "a" );

			expecting ( 1 );
			fwrite("Z", 1, 1, fptr);
		finis

		testing("Issue the call to fflush.");
		start
			expecting ( SUCCESS );
			fflush( fptr );
		finis

		cleanup("Examine the contents of the file after the close.");
		cleanup("And finally close the temporary file.");

		start
			expecting ( SUCCESS );
			fclose( fptr );

			expecting ( SUCCESS );
			fd=open(path,O_RDONLY);

			expecting ( VOID );
			if (read(fd,&ch,1) != 1)
			{
				severr("read failed - data may not have been flushed");
			}

			if (ch != 'Z')
			{
				error("data read does not match data written\n\
'Z' was written, but '%c' was read back", ch);
			}

			expecting ( SUCCESS );
			(void) close(fd);
		finis
	finis


	abstract("Fflush of file opened for write,");
	abstract("should return 0, flushing the data.");
	start

		prep("Close the temporary file from the previous case.");
		prep("Re-open it and write some data to it.");

		start
			expecting ( SUCCESS );
			(void) unlink( path );

			expecting ( SUCCESS );
			fptr=fopen(path, "w" );

			expecting ( 1 );
			fwrite("Z", 1, 1, fptr);
		finis

		testing("Issue the call to fflush.");
		start
			expecting ( SUCCESS );
			fflush( fptr );
		finis

		cleanup("Examine the contents of the file after the close.");
		cleanup("And finally close the temporary file.");

		start
			expecting ( SUCCESS );
			fclose( fptr );

			expecting ( SUCCESS );
			fd=open(path,O_RDONLY);

			expecting ( VOID );
			if (read(fd,&ch,1) != 1)
			{
				severr("read failed - data may not have been flushed");
			}

			if (ch != 'Z')
			{
				error("data read does not match data written\n\
'Z' was written, but '%c' was read back", ch);
			}

			expecting ( SUCCESS );
			(void) close(fd);
		finis
	finis


	abstract("Fflush of file opened for update,");
	abstract("should return 0, flushing the data.");
	start

		prep("Close the temporary file from the previous case.");
		prep("Re-open it and write some data to it.");

		start
			init_buf( buf );
			put_file( path, 0666, buf );

			expecting ( SUCCESS );
			fptr=fopen(path, "r+" );

			expecting ( 1 );
			fwrite("Z", 1, 1, fptr);

		finis

		testing("Issue the call to fflush.");
		start
			expecting ( SUCCESS );
			fflush( fptr );
		finis

		cleanup("Examine the contents of the file after the close.");
		cleanup("And finally close the temporary file.");

		start
			expecting ( SUCCESS );
			fclose( fptr );

			expecting ( SUCCESS );
			fd=open(path,O_RDONLY);

			expecting ( VOID );
			if (read(fd,&ch,1) != 1)
			{
				severr("read failed - data may not have been flushed");
			}

			if (ch != 'Z')
			{
				error("data read does not match data written\n\
'Z' was written, but '%c' was read back", ch);
			}

			expecting ( SUCCESS );
			(void) close(fd);

		finis
	finis


	abstract("Fflush of file opened for update and append,");
	abstract("should return 0, flushing the data.");
	start

		prep("Close the temporary file from the previous case.");
		prep("Re-open it and write some data to it.");

		start
			expecting ( SUCCESS );
			(void) unlink( path );

			expecting ( SUCCESS );
			fptr=fopen(path, "a+" );

			expecting ( 1 );
			fwrite("Z", 1, 1, fptr);

			expecting ( SUCCESS );
			fflush( fptr );
		finis

		cleanup("Examine the contents of the file after the close.");
		cleanup("And finally close the temporary file.");

		start
			expecting ( SUCCESS );
			fclose( fptr );

			expecting ( SUCCESS );
			fd=open(path,O_RDONLY);

			expecting ( VOID );
			if (read(fd,&ch,1) != 1)
			{
				severr("read failed - data may not have been flushed");
			}

			if (ch != 'Z')
			{
				error("data read does not match data written\n\
'Z' was written, but '%c' was read back", ch);
			}

			expecting ( SUCCESS );
			(void) close(fd);
		finis
	finis


	abstract("Fflush of file update and write.");
	abstract("should return 0, flushing the data.");
	start

		prep("Close the temporary file from the previous case.");
		prep("Re-open it and write some data to it.");

		start
			expecting ( VOID );
			(void) unlink( path );

			expecting ( SUCCESS );
			fptr=fopen(path, "w+" );

			expecting ( 1 );
			fwrite("Z", 1, 1, fptr);
		finis


		testing("Issue the call to fflush.");
		start
			expecting ( SUCCESS );
			fflush( fptr );
		finis

		cleanup("Examine the contents of the file after the close.");
		cleanup("And finally close the temporary file.");

		start

			expecting ( SUCCESS );
			fclose( fptr );

			expecting ( SUCCESS );
			fd=open(path,O_RDONLY);

			expecting ( VOID );
			if (read(fd,&ch,1) != 1)
			{
				severr("read failed - data may not have been flushed");
			}

			if (ch != 'Z')
			{
				error("data read does not match data written\n\
'Z' was written, but '%c' was read back", ch);
			}

			expecting ( SUCCESS );
			(void) close(fd);

		finis
	finis


	abstract("Fflush of file causing write error.");
	abstract("should return 0, flushing the data.");
	start

		prep("Close the temporary file from the previous case.");
		prep("Lower ulimit.  Re-open it and write some data to it.");

		start
			expecting ( SUCCESS );
			(void) unlink( path );

			expecting ( SUCCESS );
			ulimit( 2, ULIM/512 );

			expecting ( SUCCESS );
			fptr=fopen(path, "w" );

			exit_on (ERROR);

			for( l=(long)BUFSIZ; l <= ULIM; l += (long)BUFSIZ )
			{
				expecting ( BUFSIZ );
				fwrite(buf, 1, BUFSIZ, fptr);
			}

			expecting (SUCCESS);
			fflush (fptr);

			expecting ( VOID );
			if( fwrite(buf, 1, BUFSIZ, fptr) != (int)( ULIM%(long)BUFSIZ )	)
			{
				if (fflush (fptr) != EOF)
				severr("fwrite did not fail as expected at filesize limit");
			}
		finis

		testing("Issue the call to fflush.");
		start
			expecting ( EOF );
			fflush( fptr );

			expecting ( EOF );
			fclose( fptr );
		finis

	finis

	done();
	/* NOTREACHED */
}
