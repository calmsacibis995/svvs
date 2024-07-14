#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	CHARDEV		0020000


#ident	"@(#)ctermid.c	1.3"



/*
** ctermid
**	Test the ctermid function.
*/
main()
{
	char		ret_buf1[L_ctermid];
	char		ret_buf2[L_ctermid];
	char		ret_buf3[L_ctermid];
	char		ret_buf4[L_ctermid];
	char		ret_buf5[L_ctermid];
	char		ret_buf6[L_ctermid];
	char		*buf_ptr1;
	char		*buf_ptr2;
	struct	stat	buf;

	extern	char	*ctermid();

setup("ctermid");
funct("ctermid");

testing("L_ctermid should be a positive value.");
start
	continue_on (ERROR);

	if ( L_ctermid < 0 )
		error ("Negative value for L_ctermid = %d", L_ctermid);
finis


testing("ctermid(NULL) should return a reasonable return string.");
start
	expecting (SUCCESS);

	if ( (buf_ptr1 = ctermid(NULL)) != NULL )
	{
		/* save the buffer value for the next call */
		strcpy(ret_buf1, buf_ptr1);
		if ( stat(ret_buf1, &buf) == 0 )
		{
			if ( (buf.st_mode & CHARDEV) != CHARDEV )
				error ("%s%s%d",
				"Ctermid returned not a character device ",
				"with a NULL pointer.\nThe mode = ",
				buf.st_mode);
		}
		else
			error ("%s\n%s%s",
			"Ctermid returned the wrong string with a NULL pointer.",
			"The string = ",
			ret_buf1);

	}
	else
		error ("Ctermid(NULL) returned a NULL string");
finis


testing("ctermid(NULL) should return");
testing("a reasonable return string same as the previous call");
testing("with the physical location as before.");
start

	if ( (buf_ptr2 = ctermid(NULL)) != NULL )
	{
		/*
		** check that this string is the same as the
		** one in the previous call.
		*/
		strcpy(ret_buf2, buf_ptr2);
		expecting (VOID);

		if ( strcmp(ret_buf1, ret_buf2) != 0 )
			error ("%s%s%s%s%s%s",
			"A second call to ctermid with a NULL did not produce ",
			"the same string as the first call",
			"\nThe first string = ", ret_buf1,
			"\nThe second string = ", ret_buf2);

		expecting (SUCCESS);

		/*
		** see if the physical location of the two pointers
		** returned by the two consecutive calls to ctermid
		** with NULL are the same.
		*/
		if ( buf_ptr1 != buf_ptr2 )
			error ("%s%s%s%ld%s%ld",
			"The addresses of the two pointers returned by two ",
			"consecutive calls to ctermid with NULL are not the same",
			"\nFirst pointer = ", (long)buf_ptr1,
			"\nSecond pointer - ", (long)buf_ptr2);

		/*
		** see if st_mode returned by stat() is
		** valid -- ie character device.
		*/
		if ( stat(ret_buf2, &buf) == 0 )
		{
			if ( (buf.st_mode & CHARDEV) != CHARDEV )
				error ("Ctermid returned not a character\
device with a NULL pointer.\nThe mode returned = %d", buf.st_mode);
		}
		else
			error ("Ctermid returned the wrong string with\
 a NULL pointer.\nThe mode returned = %d", buf.st_mode);
	}
	else
		error ("Ctermid(NULL) returned a NULL string\n%s%s",
		"buf_ptr1 points to :", buf_ptr1);
finis


testing("Passing a string of length L_ctermid");
testing("should return the same string as before.");
start

	if ( (buf_ptr1 = ctermid(ret_buf3)) != NULL )
	{
		/*
		** check to see if the returned pointer is
		** the same as the one passed to ctermid.
		*/
		if ( buf_ptr1 != ret_buf3 )
			error ("ctermid called with a pointer did not return\
 the same pointer address");
		
		/*
		** check that the string returned is the same as 
		** one from the previous call.
		*/
		expecting (VOID);

		if ( strcmp(ret_buf1, ret_buf3) != 0 )
			error ("ctermid called with a pointer returned\n\
a different string from the call with a NULL pointer");

		expecting (SUCCESS);

	}
	else
		error ("Ctermid(pointer) returned a NULL string");
finis



testing("Passing a different string of length L_ctermid to ctermid");
testing("should return the same string as before.");
start

	if ( (buf_ptr1 = ctermid(ret_buf4)) != NULL )
	{
		/*
		** check to see if the returned pointer is
		** the same as the one passed to ctermid.
		*/
		if ( buf_ptr1 != ret_buf4 )
			error ("ctermid called with a pointer did not\n\
 return the same pointer address");
		
		/*
		** check that the string returned is the same as 
		** one from the previous call.
		*/
		expecting (VOID);

		if ( strcmp(ret_buf3, ret_buf4) != 0 )
			error ("ctermid called with a pointer returned\n\
a different string from the call with a different pointer");

		expecting (SUCCESS);

	}
	else
		error ("Ctermid(pointer) returned a NULL string");
finis


testing("ctermid(NULL) with the terminal closed and /dev/null open");
testing("should return a reasonable string.");
start
	expecting (SUCCESS);

	/*
	** close the dev with descriptor 0(zero) and open
	** /dev/null for the following two tests.
	*/
	close(0);

	open("/dev/null", O_RDWR);

	/*
	** now proceed with the test
	*/
	if ( (buf_ptr1 = ctermid(NULL)) != NULL )
	{
		/* save the buffer value for the next call */
		strcpy(ret_buf5, buf_ptr1);

		expecting (VOID);

		if ( strcmp(ret_buf5, "/dev/null") == 0 )
			error("Ctermid returned '/dev/null' with a NULL pointer.");

		expecting (SUCCESS);

		if ( stat(ret_buf5, &buf) == 0 )
		{
			if ( (buf.st_mode & CHARDEV) != CHARDEV )
				error("Ctermid returned not a char dev with a NULL pointer.");
		}
		else
			error("Ctermid returned the wrong string with a NULL pointer.");
	}
	else
		error ("Ctermid(NULL) returned a NULL string");
finis


testing("Passing a string of length L_ctermid to ctermid with");
testing("the terminal closed and /dev/null open should return");
testing("a reasonable string.");
start
	if ( (buf_ptr1 = ctermid(ret_buf6)) != NULL )
	{
		/*
		** check to see if the returned pointer is
		** the same as the one passed to ctermid.
		*/
		if ( buf_ptr1 != ret_buf6 )
			error ("ctermid called with a pointer did not return\
 the same pointer address");
		
		/*
		** check that the string returned is the same as 
		** one from the previous call.
		*/
		expecting (VOID);

		if ( strcmp(ret_buf1, ret_buf6) != 0 )
			error ("ctermid called with a pointer returned\n\
a different string from the call with a NULL pointer\n\
The string returned = %s", ret_buf1);

		expecting (SUCCESS);

	}
	else
		error ("Ctermid(pointer) returned a NULL string");

finis


done();
/*NOTREACHED*/

}
