#include	<sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#include	<fcntl.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)getc.c	1.5"



extern	FILE	*data_fopen();
extern	FILE	*temp_fopen();

long lseek(), ltemp;  

/*
** variations of this are in the 'known' files that we read for testing purposes
*/
char	Data[] = "abcdefghijklmnopqrstuvwxyz";

# define	MAXLINES	26	/* Number of lines in the file */
# define	NUMINTS		25	/* Number of integers in getw temp file */

char	String[STRLEN];			/* Temp string for error messages */

/*
** getc
**	Test getc, getchar, fgetc and getw
*/
main()
{
setup("getc");

funct ("getc");
start
	t_getc();
finis

funct ("getchar");
start
	t_getchar();
finis

funct ("fgetc");
start
	t_fgetc();
finis

funct ("getw");
start
	t_getw();
finis

done();
/*NOTREACHED*/

}

/*
** t_getc
**	Test the getc routine.
*/
t_getc()
{
	FILE	*fp;			/* FILE for getc */
	int	character;
	int	length;
	int	numlines;
	int	totalchars;
	char	*sp;

testing("File read using getc should return");
testing ("the contents of the file to be read.");
start
	expecting (SUCCESS);
	fp = data_fopen("datagetc","r");

	totalchars = 0;
	for ( numlines = 0 ; numlines < MAXLINES ; numlines++ )
	{
		sp = &Data[numlines];
		expecting (VOID);
		for ( length = 0 ; length < strlen(Data) ; length++ )
		{
			if ( *sp == '\0' )
				sp = Data;
			character = getc(fp);
			if ( *sp != character )
			{ 
				error ("After reading %d characters, getc returned the wrong character.",totalchars);
				return;
			}
			totalchars++;
			sp++;
		}
		if ( (character = getc(fp)) != '\n' )
		{
			error ("After reading %d characters, getc returned the wrong character.",totalchars);
			return;
		}
		totalchars++;
	}
finis


testing("Verify getc at EOF returns EOF.");
start
	expecting (VOID);
	if ( (character = getc(fp)) != EOF )
	{
		error("getc returned '%c', EOF was expected", character);
		return;
	}
finis


testing("fseek and getc - seeking to start should cause a read to get an 'a'.");
start
	expecting (0);
	fseek(fp,0L,SEEK_SET);

	expecting (VOID);
	if ( (character = getc(fp)) != 'a' )
	{
		error("getc returned '%c' after the fseek", character);
		return;
	}
finis


testing("fseek and getc - seeking to the end of the first line");
testing("should cause a read to get a carriage return.");
start
	expecting (VOID);
	ltemp = (long)strlen(Data);

	expecting (0);
	fseek(fp, ltemp,SEEK_SET);

	expecting (VOID);
	if ( (character = getc(fp)) != '\n' )
	{
		error ("get returned '%c' instead of a '\n'", character);
		return;
	}
finis


testing("fseek and getc -seeking to the beginning of the second line.");
testing("should cause a read to get a 'b'.");
start
	expecting (0);
	fseek(fp,0L,SEEK_CUR);

	expecting (VOID);
	if ( (character = getc(fp)) != 'b' )
	{
		error("getc returned '%c'",character);
		return;
	}
finis


testing("fseek and getc -- seeking to the beginning of the fourth line.");
testing("should read in a 'd'.");
start
	expecting (VOID);
	ltemp = (long) (3 * strlen(Data) + 3);

	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	if ( (character = getc(fp)) != 'd' )
	{
		error("getc returned '%c'",character);
		return;
	}
finis


testing("fseek and getc - seeking to the middle of the ninth line.");
testing("should read in a 'v'.");
start
	expecting (VOID);
	ltemp = (long) (8 * strlen(Data) + 8);

	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	expecting (0);
	fseek(fp,13L,SEEK_CUR);

	expecting (VOID);
	if ( (character = getc(fp)) != 'v' )
	{
		error("getc returned '%c'",character);
		return;
	}

	expecting (0);
	fclose(fp);
finis

}

/*
** t_fgetc
**	Test the fgetc routine.
*/
t_fgetc()
{
	FILE	*fp;			/* FILE for fgetc */
	int	character;
	int	length;
	int	numlines;
	int	totalchars;
	char	*sp;

testing("file read using fgetc should return the contents of the file to be read.");
start
	expecting (SUCCESS);
	fp = data_fopen("datagetc","r");

	totalchars = 0;
	for ( numlines = 0 ; numlines < MAXLINES ; numlines++ )
	{
		sp = &Data[numlines];
		expecting (VOID);
		for ( length = 0 ; length < strlen(Data) ; length++ )
		{
			if ( *sp == '\0' )
				sp = Data;

			expecting (VOID);
			character = fgetc(fp);
			if ( *sp != character )
			{ 
				error ("After reading %d characters, fgetc returned the wrong character.",totalchars);
				return;
			}
			totalchars++;
			sp++;
		}
		expecting (VOID);
		if ( (character = fgetc(fp)) != '\n' )
		{
			error ("After reading %d characters, fgetc returned the wrong character.",totalchars);
			return;
		}
		totalchars++;
	}
finis


testing("Verify fgetc at EOF returns EOF.");
start
	expecting (EOF);
	fgetc(fp);
finis


testing("fseek and fgetc - seeking to start of file");
testing("should read in an 'a'.");
start
	expecting (0);
	fseek(fp,0L,SEEK_SET);

	expecting ('a');
	fgetc(fp);
finis


testing("fseek and fgetc - seeking to the end of the first line");
testing("should read in a carriage return.");
start
	expecting (VOID);
	ltemp = (long)strlen(Data);

	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	expecting ('\n');
	fgetc(fp);
finis


testing("fseek and fgetc - seeking to the beginning of the second line");
testing("should read in a 'b'.");
start
	expecting (0);
	fseek(fp,0L,SEEK_CUR);

	expecting ('b');
	fgetc(fp);
finis


testing("fseek and fgetc - seeking to the beginning of the fourth line");
testing("should read in a 'd'.");
start
	expecting (VOID);
	ltemp = (long) (3 * strlen(Data) + 3);
	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	expecting ('d');
	fgetc(fp);
finis


testing("fseek and fgetc - seeking to the middle of the ninth line");
testing("should read in a 'v'.");
start
	expecting (VOID);
	ltemp = (long) (8 * strlen(Data) + 8);
	expecting (0);
	fseek(fp,ltemp, SEEK_SET);

	expecting (0);
	fseek(fp,13L,SEEK_CUR);

	expecting ('v');
	fgetc(fp);

	expecting (SUCCESS);
	fclose(fp);
finis

}

/*
** t_getchar
**	Test the getchar routine.
**
** Assumptions
**	If we close stdin, and then fopen a file, 
**	we get the stdin FILE.
*/
t_getchar()
{
	FILE	*fp;			/* FILE for getchar */
	int	character;
	int	length;
	int	numlines;
	int	totalchars;
	char	*sp;

testing("Reading the file using getchar should return the contents of the file.");
start
	expecting (SUCCESS);
	fclose(stdin);

	expecting (SUCCESS);
	fp = data_fopen("datagetc","r");

	totalchars = 0;
	for ( numlines = 0 ; numlines < MAXLINES ; numlines++ )
	{
		sp = &Data[numlines];
		expecting (VOID);
		for ( length = 0 ; length < strlen(Data) ; length++ )
		{
			if ( *sp == '\0' )
				sp = Data;

			expecting (SUCCESS);
			character = getchar();
			if ( *sp != character )
			{ 
				error ("After reading %d characters, getchar returned the wrong character.",totalchars);
				return;
			}
			totalchars++;
			sp++;
		}
		if ( (character = getchar()) != '\n' )
		{
			error ("After reading %d characters, getchar returned the wrong character.",totalchars);
			return;
		}
		totalchars++;
	}
finis


testing("Verify getchar at EOF returns EOF.");
start
	expecting (EOF);
	getchar();
finis


testing("fseek and getchar - seeking to the start of file");
testing("should read in an 'a'.");
start
	expecting (0);
	fseek(fp,0L,SEEK_SET);

	expecting ('a');
	getchar();
finis


testing("fseek and getchar - seeking to the end of the first line");
testing("should read in a carriage return.");
start
	expecting (VOID);
	ltemp = (long)strlen(Data);
	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	expecting ('\n');
	getchar();
finis


testing("fseek and getchar - seeking to the beginning of the second line");
testing("should read in a 'b'.");
start
	expecting (0);
	fseek(fp,0L,SEEK_CUR);

	expecting ('b');
	getchar();
finis


testing("fseek and getchar - seeking to the beginning of the fourth line");
testing("should read in a 'd'.");
start
	expecting (VOID);
	ltemp = (long) (3 * strlen(Data) + 3);
	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	expecting ('d');
	getchar();
finis


testing("fseek and getchar - seeking to the middle of the ninth line");
testing("should read in a 'v'.");
start
	expecting (VOID);
	ltemp = (long) (8 * strlen(Data) + 8);
	expecting (0);
	fseek(fp, ltemp, SEEK_SET);

	expecting (0);
	fseek(fp,13L,SEEK_CUR);

	expecting ('v');
	getchar();

	expecting (SUCCESS);
	fclose(fp);
finis

}

/*
** t_getw
**	Test the getw routine.
**
** Notes:
**	We need to write a file of integers before we can do much of
**	anything else.
*/
t_getw()
{
	FILE	*fp;			/* FILE for getw */
	int	fd;			/* File descriptor for write */
	int	value;			/* The value to write */
	int	i;
	long	locations[NUMINTS];


testing("Using getw, read from a file");
testing("the series of integers back, and EOF at the end of the file.");
start
	expecting (SUCCESS);
	fd = temp_open("tmpgetw",(O_CREAT | O_WRONLY | O_TRUNC),0666);
	
	for ( value = 0 ; value < NUMINTS ; value++ )
		{
		expecting (SUCCESS);
		if ( write(fd,&value,sizeof (value)) != sizeof (value) )
			error ("Write on getw temp file failed");
		}

	expecting (SUCCESS);
	close(fd);

	expecting (SUCCESS);
	fp = temp_fopen("tmpgetw","r");

	i = 0;

	expecting (VOID);
	while ( (value = getw(fp)) != EOF )
	{
		if ( i >= NUMINTS )
		{
			error ("getw returned more numbers than written in the file\n%d numbers were read, but only %d were written",
			    i, NUMINTS);
			return;
		}
		if ( value != i )
		{
			error("getw did not read a number from a file correctly\nthe number %d should be %d",
			   value, i);
			return;
		}
		i++;
	}

	if ( i != NUMINTS )
	{
		if ( ferror(fp) )
			error("Getw claims to have an error reading integers from a file");
		else
			if ( feof(fp) )
				error("Getw returned an early EOF");
			else
				error("Getw returned EOF unexpectedly, ferror and feof are both 'FALSE'");
		return;
	}

	if ( ferror(fp) )
	{
		error("Ferror is 'TRUE' after reading EOF with getw");
		return;
	}

	if ( !feof(fp) )
	{
		error("Feof is 'FALSE' after getting back an EOF from getw");
		return;
	}

	if ( fclose(fp) )
		error ("fclose returned non-zero closing temp file");
finis


testing("Reading integers from any alignment");
testing("should return a group of integers back, and an EOF.");
start

	expecting (SUCCESS);
	fd = temp_open("tmpgetw",(O_CREAT | O_WRONLY | O_TRUNC),0666);
	
	for ( value = 0 ; value < NUMINTS ; value++ )
	{
		expecting (SUCCESS);
		if ( (locations[value] = lseek(fd,(long) value, SEEK_END))  == -1)
		{
			error ("lseek(fd,%d,SEEK_END) returned -1",value);
		}

		expecting (SUCCESS);
		write(fd,&value,sizeof (value));
	}

	expecting (SUCCESS);
	close(fd);

	expecting (SUCCESS);
	fp = temp_fopen("tmpgetw","r");

	i = 0;

	for(;;)
	{
		expecting (SUCCESS);
		if ( i < NUMINTS && fseek(fp,locations[i],SEEK_SET) )
		{
			error ("fseek(fp,%d,SEEK_SET) returned non-zero",locations[i]);
		}

		expecting (VOID);
		if ( (value = getw(fp)) == EOF )
			break;

		if ( i >= NUMINTS )
		{
			error("getw returned more numbers then written in the file\n%d numbers were read, %d were written",
			    i,NUMINTS);
			return;
		}
		if ( value != i )
		{
			error("getw did not read a number from a file correctly\n%d was read instead of the expected %d",
			    value,i);
			return;
		}
		i++;
	}

	if ( i != NUMINTS )
	{
		expecting (VOID);
		if ( ferror(fp) )
			error("Getw claims to have an error reading integers from a file");
		else
			if ( feof(fp) )
				error("Getw returned an early EOF");
			else
				error("Getw returned EOF unexpectedly, ferror and feof are both 'FALSE'");
		return;
	}
	if ( ferror(fp) )
	{
		error("Ferror is 'TRUE' after reading EOF with getw");
		return;
	}

	if ( !feof(fp) )
	{
		error("Feof is 'FALSE' after getting back an EOF from getw");
		return;
	}


	if ( fclose(fp) )
		error ("close returned non-zero closing temp file");
finis
}
