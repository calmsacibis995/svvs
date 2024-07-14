#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<fcntl.h>	/*  defines O_RDONLY  */
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#define	_IOBAD	_IOLBF+_IOFBF+_IONBF


#ident	"@(#)setbuf.c	1.7"


char	ret_buf[BUFSIZ+2];	/*  used by read_data  */
char	data_ptr[BUFSIZ+2];	/*  filled with the data to be written  */
char	file_buf[BUFSIZ];	/*  the substitute buffer  */
FILE	*write_stream;

extern		FILE	*temp_fopen();
extern		int	fclose();

main()
{

	int		i,r;

setup("setbuf");
funct("setbuf");
start

testing("Using setbuf, set buf'ing to NULL & write a char to file,");
testing("should yield the char in the file.");
start
	strcpy(data_ptr , "a");
	do_it (NULL);
finis


testing("Using setbuf, set buf'ing to NULL & write BUFSIZ/2 chars to file");
testing("should yield BUFSIZ/2 chars in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ/2); i++)
		strcat (data_ptr, "a");

	do_it (NULL);
finis



testing("Using setbuf, set buf'ing to NULL & write BUFSIZ chars to file");
testing("should yield BUFSIZ chars in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ); i++)
		strcat (data_ptr, "a");

	do_it (NULL);
finis



testing("Using setbuf, set buf'ing to BUFSIZ & write a char to file");
testing("should yield the char in the file.");
start
	strcpy(data_ptr , "a");
	do_it (file_buf);
finis


testing("Using setbuf, set buf'ing to BUFSIZ & writing BUFSIZ/2 chars to file,");
testing("should yield BUFSIZ/2 chars in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ/2); i++)
		strcat (data_ptr, "a");
	do_it (file_buf);
finis



testing("Using setbuf, set buf'ing to BUFSIZ & write BUFSIZ chars to file");
testing("should yield BUFSIZ chars in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ); i++)
		strcat (data_ptr, "a");

	do_it (file_buf);
finis



testing("Using setbuf, set buf'ing to BUFSIZ & write BUFSIZ-1 chars to file");
testing("should yield BUFSIZ-1 characters in the file");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ-1); i++)
		strcat (data_ptr, "a");

	do_it (file_buf);
finis


testing("Using setbuf, set buf'ing to BUFSIZ & write BUFSIZ+1 chars to file.");
testing("BUFSIZ chars should be in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ+1); i++)
		strcat (data_ptr, "a");

	do_it (file_buf);
finis

finis	/*  end of funct block  */

# ifndef	SYSVR_1

funct("setvbuf");
start
testing("Using setvbuf, set buf'ing to _IONBF & write one char to file.");
testing("The char should be in the file.");
start
	expecting (VOID);
	strcpy(data_ptr , "a");

	v_do_it (_IONBF);
finis



testing("Using setvbuf, set buf'ing to _IONBF & write BUFSIZ/2 chars to file,");
testing("should place BUFSIZ/2 chars in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ/2); i++)
		strcat (data_ptr, "a");

	v_do_it (_IONBF);
finis



testing("Using setvbuf, set buf'ing to _IONBF & write BUFSIZ chars to file.");
testing("BUFSIZ chars should be in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ); i++)
		strcat (data_ptr, "a");

	v_do_it (_IONBF);
finis


testing("Using setvbuf, set buf'ing to _IOFBF & write one char to file.");
testing("One char should be in the file.");
start
	expecting (VOID);
	strcpy(data_ptr , "a");

	v_do_it (_IOFBF);
finis



testing("Using setvbuf, set buf'ing to _IOFBF & write BUFSIZ chars to file.");
testing("BUFSIZ chars should be in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ); i++)
		strcat (data_ptr, "a");

	v_do_it (_IOFBF);
finis



testing("Using setvbuf, set buf'ing to _IOFBF & write BUFSIZ/2 chars to file.");
testing("BUFSIZ/2 chars should be in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ/2); i++)
		strcat (data_ptr, "a");

	v_do_it (_IOFBF);
finis



testing("Using setvbuf,set buf'ing to _IOFBF & write BUFSIZ+1 chars to file.");
testing("BUFSIZ chars should be in the file.");
start
	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ+1); i++)
		strcat (data_ptr, "a");

	v_do_it (_IOFBF);
finis



testing("Using setvbuf, set buf'ing to _IOLBF & write a line with");
testing("a newline character in it.");
testing("The entire line should be in the file.");
start
	strcpy(data_ptr, "A line with a newline\n");

	v_do_it (_IOLBF);
finis


testing("Using setvbuf, set buf'ing to _IOLBF & write a newline.");
testing("The newline should be in the file.");
start
	expecting (VOID);
	strcpy(data_ptr, "\n");

	v_do_it (_IOLBF);
finis



testing("Using setvbuf, set buf'ing to _IOLBF & write a character.");
testing("Nothing should be in the file.");
start
/*  v_do_it won't handle this case  */
	expecting (SUCCESS);
	write_stream = temp_fopen("tmpsetbuf", "w");

	expecting (VOID);
	if ( (r = do_setvbuf(write_stream, file_buf, _IOLBF, BUFSIZ)) != 0  )
		error("setvbuf returned nonzero on legal arguments");

	strcpy(data_ptr, "a");

	expecting (1);
	fwrite(data_ptr, 1, 1, write_stream);
	
	expecting (VOID);
	if ( read_file(0, ret_buf, data_ptr) != 0 )
	{
		warning ("The character was written to the file");
	}

	expecting (SUCCESS);
	fclose(write_stream);
finis



testing("Using setvbuf, set buf'ing to _IOLBF & write BUFSIZ+1");
testing("chars with out a newline to file.");
testing("BUFSIZ chars should be in the file.");
start
/*  v_do_it won't handle this case  */
	expecting (SUCCESS);
	write_stream = temp_fopen("tmpsetbuf", "w");

	expecting (VOID);
	if ( (r = do_setvbuf(write_stream, file_buf, _IOLBF, BUFSIZ)) != 0  )
		error("setvbuf returned nonzero on legal arguments");

	data_ptr[0] = '\0';
	for (i = 0; i < (BUFSIZ+1); i++)
		strcat (data_ptr, "a");


	expecting ( BUFSIZ + 1 );
	fwrite(data_ptr, 1, (BUFSIZ + 1), write_stream);
	
	expecting (VOID);
	if ( read_file(BUFSIZ, ret_buf, data_ptr) != 0 )
	{
		error ("setvbuf did not set buf'ing on file to BUFSIZ with \
type _IOLBF");
	}

	expecting (SUCCESS);
	fclose(write_stream);
finis



testing("Using setvbuf, using an illegal value for type,");
testing("should return a nonzero return value from setvbuf.");
start
	expecting (SUCCESS);
	write_stream = temp_fopen("tmpsetbuf", "w");

	expecting (VOID);
	if ( setvbuf(write_stream, file_buf, _IOBAD, BUFSIZ) == 0  )
		if ( setvbuf(write_stream, _IOBAD, file_buf, BUFSIZ) == 0  )
			error("setvbuf did not return nonzero on illegal arguments");
		else
			warning ("%s\nsetvbuf (stream, file_buf, _IOBAD, BIFSIZ)\n\
was successful, but setvbuf (stream, _IOBAD, file_buf, BUFSIZE) failed",
				ADVISORY);

	expecting (SUCCESS);
	fclose(write_stream);
finis


testing("Using setvbuf, using an illegal value for size,");
testing("should return nonzero from setvbuf.");
start
	expecting (SUCCESS);
	write_stream = temp_fopen("tmpsetbuf", "w");

	expecting (VOID);
	if ( setvbuf(write_stream, file_buf, _IOLBF, -30000) == 0  )
		if (setvbuf(write_stream, _IOLBF, file_buf,-30000) == 0  )
			error("setvbuf did not return nonzero on illegal arguments");
		else
			warning ("%s\n\
setvbuf(stream, file_buf, _IOLBF, -30000) was successful,\n\
setvbuf(stream, _IOLBF, file_buf,-30000) failed",
ADVISORY);

	expecting (SUCCESS);
	fclose(write_stream);
finis

finis	/*  end of funct block  */

# endif	/* Not SYSVR_1 */
done();
/*NOTREACHED*/

}

/*
** read_file(nbyte, ret_buf, data_ptr)
**
**	Read nbytes from the temp file 'setbuf' and compare the
**	data to data_ptr. Return 0 if data_ptr == ret_buf
**                               1 if data_ptr != ret_buf.
**
*/
read_file(nbyte, ret_buf, data_ptr)
int		nbyte;
char		*ret_buf;
char		*data_ptr;
{
	int		flag;
	int		read_stream;
	int		cnt;
	char		*ptr;

	ret_buf[0] = '\0';
	if ( (read_stream = temp_open("tmpsetbuf", O_RDONLY)) == (-1) )
		error ("Could not open the tempfile 'setbuf' for read");
	if ( (cnt = read(read_stream, ret_buf, nbyte )) != (-1) )
	{
		for (ptr=ret_buf ; ptr<(ret_buf + cnt -1) ; ptr++)
			if (*ptr == '\n')
				break;
		*(++ptr) = '\0';
		if ( strncmp(ret_buf,data_ptr,mystrlen(ret_buf)) != 0 )
			flag = 1;
		else
			flag = 0;
	}
	else
		flag = 1;

	if ( close(read_stream) )
		error ("close(tempfile = 'setbuf') failed");
	return(flag);
}
# ifndef	SYSVR_1
do_setvbuf(stream, buf, typ, siz)
FILE *stream;
char *buf;
int typ, siz;
{
	int r,r1;

	expecting (VOID);
	r = setvbuf(stream, buf, typ, siz);
	if (!r) return(0);
	r1 = setvbuf(stream, typ, buf, siz);
	if (!r1) {
		warning ("%s\nsetvbuf (stream, typ, buf, siz) succeeded",
			ADVISORY);
		return(0);
	}
	r = setvbuf(stream, buf, typ, siz);
	return(r);
}



v_do_it (arg)
int 	arg;
{
int	len;
int	r;

expecting (SUCCESS);
write_stream = temp_fopen("tmpsetbuf", "w");

if ( (r = do_setvbuf(write_stream, file_buf, arg, BUFSIZ)) != 0  )
	error("setvbuf returned %d (nonzero) on legal arguments", r);

len = mystrlen(data_ptr);

expecting (len);
fwrite(data_ptr, 1, len, write_stream);

/*
 *	Flush the buffer if buffering was requested.
 *	_IONBF specifies no buffering (so no flushing is necessary)
 *	and _IOLBF specifies that the buffer is flushed for each newline.
 */
if (arg == _IOFBF)
	{
	expecting (SUCCESS);
	fflush (write_stream);
	}

len++;

expecting (VOID);
if ( read_file(len, ret_buf, data_ptr) != 0 )
	error ("file readback failed");

expecting (SUCCESS);
fclose(write_stream);
}

#endif

/*
 *  mystrlen - same as strlen but without the svvs error checking
 */
mystrlen (ptr)
register char *ptr;
{
register char *str;

str = ptr;
while (*ptr++)
	;
return (ptr - str - 1);
}



do_it (arg)
char 	*arg;
{
int	len;

expecting (SUCCESS);
write_stream = temp_fopen("tmpsetbuf", "w");

expecting (VOID);
setbuf(write_stream, arg);

len = mystrlen(data_ptr);

expecting (len);
fwrite(data_ptr, 1, len, write_stream);

/*
 *  If there is any buffering, then
 *  	flush the buffer
 */
if (arg != NULL)
	{
	expecting (SUCCESS);
	fflush (write_stream);
	}

len++;

expecting (VOID);
if ( read_file(len, ret_buf, data_ptr) != 0 )
	error ("file readback failed");

expecting (SUCCESS);
fclose(write_stream);
}
