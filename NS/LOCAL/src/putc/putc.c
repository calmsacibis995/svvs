#include	<sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)putc.c	1.3"



# define	NUMINTS	35

extern	FILE	*temp_fopen();

/*
** putc
**	Check the putc routines.
*/
main()
{
setup("putc");

funct ("putc");
start
	t_putc();
finis

funct ("fputc");
start
	t_fputc();
finis

funct ("putw");
start
	t_putw();
finis

funct ("putchar");
start
	t_putchar();
finis
done();
/*NOTREACHED*/
}

/*
** t_putc
**	Test the putc routine.
*/
t_putc()
{
	int	ch;
	int	ret_val;
	FILE	*fp;
	int	i;


testing("Writing a file with putc should");
testing("return the values of the characters written,");
testing("and a file to be written.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpputc","w");

	for ( ch = 'a' ; ch <= 'z' ; ch++ )
	{
		expecting (VOID);
		if ( (ret_val = putc(ch,fp)) != ch )
		{
			if ( ret_val == EOF )
			{
				error("putc returned EOF while writing the \
valid character '%c'", ch);
			}
			error ("puts returned '%c', '%c' was expected",
				ret_val, ch);
		}
	}

	expecting (SUCCESS);
	fclose(fp);

	fp = temp_fopen("tmpputc","r");

	i = 'a';
	expecting (VOID);
	while ( (ret_val = getc(fp)) != EOF )
	{
		if ( i > 'z' )
		{
			error("putc wrote more characters to the file \
then it was supposed to");
		}
		if ( ret_val != i )
		{
			error ("putc wrote '%c', '%c' was exepected",
				ret_val, i);
		}
		i++;
	}

	if ( i <= 'z' )
	{
		error("putc did not write all the characters out to the file");
	}
	expecting (SUCCESS);
	fclose(fp);
finis


testing("putc to a file not open for writing should return EOF.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpputc","r");
	
	expecting (VOID);
	if ( (ret_val = putc('a',fp)) != EOF )
	{
		error("putc returned %d, EOF was expected", ret_val);
	}
	expecting (SUCCESS);
	fclose(fp);
finis


testing("a series of fseek/putcs should result in a changed file.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpputc","w");

	for ( ch = 'a' ; ch <= 'z' ; ch++ )
	{
		expecting (VOID);
		if ( (ret_val = putc(ch,fp)) != ch )
		{
			if ( ret_val == EOF )
			{
				error("putc returned EOF while writing the \
valid character '%c'", ch);
			}
			error ("puts returned '%c', '%c' was expected",
				ret_val, ch);
		}
	}

	for ( i = 0 ; i <= 'z' - 'a' ; i++ )
	{
		expecting (SUCCESS);
		fseek(fp,(long)i,SEEK_SET);

		ch = 'z' - i;
		expecting (VOID);
		if ( (ret_val = putc(ch,fp)) != ch )
		{
			if ( ret_val == EOF )
			{
				error("putc returned EOF, '%c' was expected",
					ch);
			}
			error("putc returned '%c', '%c' was expected",
				ret_val, ch);
		}
	}

	expecting (SUCCESS);
	fclose(fp);

	fp = temp_fopen("tmpputc","r");

	i = 'z';
	expecting (VOID);
	while ( (ret_val = getc(fp)) != EOF )
	{
		if ( i < 'a' )
		{
			error("putc wrote more characters to the file \
then it was supposed to");
		}
		if ( ret_val != i )
		{
			error("putc returned '%c', '%c' was expected",
				ret_val, i);
		}
		i--;
	}

	if ( i >= 'a' )
	{
		error("putc did not write all the characters out to the file");
	}
	expecting (SUCCESS);
	fclose(fp);
finis
}

/*
** t_fputc
**	Test the fputc routine.
*/
t_fputc()
{
	int	ch;
	int	ret_val;
	FILE	*fp;
	int	i;


testing("Writing a file with fputc should");
testing("return values of the characters written,");
testing("and a file to be written.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpfputc","w");

	for ( ch = 'a' ; ch <= 'z' ; ch++ )
	{
		expecting (VOID);
		if ( (ret_val = fputc(ch,fp)) != ch )
		{
			if ( ret_val == EOF )
			{
				error("fputc returned EOF while writing a \
valid character");
			}
			error("fputc returned '%c', '%c' was exepcted",
				ret_val, ch);
		}
	}

	expecting (SUCCESS);
	fclose(fp);

	fp = temp_fopen("tmpfputc","r");

	i = 'a';
	expecting (VOID);
	while ( (ret_val = getc(fp)) != EOF )
	{
		if ( i > 'z' )
		{
			error("fputc wrote more characters to the file \
than it was supposed to");
		}
		if ( ret_val != i )
		{
			error("fputc wrote '%c', '%c' was expected",
				ret_val, i);
		}
		i++;
	}

	if ( i <= 'z' )
	{
		error("fputc did not write all the characters out to the file");
	}
	expecting (SUCCESS);
	fclose(fp);
finis


testing("fputc to a file not open for writing should return EOF.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpfputc","r");
	
	expecting (EOF);
	fputc('a',fp);
	expecting (SUCCESS);
	fclose(fp);
finis



testing("a series of fseek/fputcs should result in a changed file.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpfputc","w");

	for ( ch = 'a' ; ch <= 'z' ; ch++ )
	{
		expecting (VOID);
		if ( (ret_val = fputc(ch,fp)) != ch )
		{
			if ( ret_val == EOF )
			{
				error("fputc returned EOF while writing '%c'",
					ch);
			}
			error("fputc returned '%c', '%c' was expected",
				ret_val, ch);
		}
	}

	for ( i = 0 ; i <= 'z' - 'a' ; i++ )
	{
		expecting (0L);
		fseek(fp,(long)i,SEEK_SET);
		ch = 'z' - i;
		expecting (VOID);
		if ( (ret_val = fputc(ch,fp)) != ch )
		{
			if ( ret_val == EOF )
			{
				error("fputc returned EOF, expecting '%c'",
					ch);
			}
			error("fputc returned '%c', '%c' was expected",
				ret_val, ch);
		}
	}

	expecting (SUCCESS);
	fclose(fp);

	fp = temp_fopen("tmpfputc","r");

	i = 'z';
	expecting (VOID);
	while ( (ret_val = getc(fp)) != EOF )
	{
		if ( i < 'a' )
		{
			error("fputc wrote more characters to the file \
than it was supposed to");
		}
		if ( ret_val != i )
		{
			error("fputc wrote '%c', '%c' was expected",
				ret_val, i);
		}
		i--;
	}

	if ( i >= 'a' )
	{
		error("fputc did not write all the characters out to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis
}

/*
** t_putchar
**	Test the putchar routine.
*/
t_putchar()
{
	int	ch;
	int	ret_val;
	FILE	*fp;
	int	i;


testing("Writing a file with putchar");
testing("should return the values of the characters written,");
testing("and a file to be written.");
start
	push_stdout("tmpputchar","w");

	for ( ch = 'a' ; ch <= 'z' ; ch++ )
	{
		expecting (VOID);
		if ( (ret_val = putchar(ch)) != ch )
		{
			if ( ret_val == EOF )
			{
				pop_stdout();
				error("putchar returned EOF while writing '%c'",
					ch);
			}
			else
			{
				pop_stdout();
				error("putchar returned '%c', '%c' was expected",
					ret_val, ch);
			}
		}
	}

	expecting (VOID);
	if ( fclose(stdout) )
	{
		pop_stdout();
		error ("fclose of the temp file 'putchar' returned non-zero");
	}
	pop_stdout();

	expecting (SUCCESS);
	fp = temp_fopen("tmpputchar","r");

	expecting (0L);
	fseek(fp,(long)0,SEEK_SET);
	i = 'a';

	expecting (VOID);
	while ( (ret_val = getc(fp)) != EOF )
	{
		if ( i > 'z' )
		{
			error("putchar wrote more characters to the file \
than it was supposed to");
		}
		if ( ret_val != i )
		{
			error("putchar returned '%c', '%c' was exepected",
				ret_val, i);
		}
		i++;
	}

	if ( i <= 'z' )
	{
		error("putchar did not write all the characters out \
to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis



testing("putchar to stdout when not open for writing should return EOF.");
start
	push_stdout("tmpputchar","r");
	
	expecting (EOF);
	putchar('a');

	expecting (SUCCESS);
	fclose(stdout);

	pop_stdout();
finis



testing("a series of fseek/putchars should result in a changed file.");
start
	push_stdout("tmpputchar","w");

	for ( ch = 'a' ; ch <= 'z' ; ch++ )
	{
		expecting (VOID);
		if ( (ret_val = putchar(ch)) != ch )
		{
			if ( ret_val == EOF )
			{
				pop_stdout();
				error("putchar returned EOF while writing '%c'",
					ch);
			}
			pop_stdout();
			error("putchar returned '%c', '%c' was expected",
				ret_val, ch);
		}
	}

	for ( i = 0 ; i <= 'z' - 'a' ; i++ )
	{
		expecting (0L);
		fseek(stdout,(long)i,SEEK_SET);

		ch = 'z' - i;
		expecting (VOID);
		if ( (ret_val = putchar(ch)) != ch )
		{
			if ( ret_val == EOF )
			{
				pop_stdout();
				error("putchar returned EOF, '%c' was expected",
					ch);
			}
			else
			{
				pop_stdout();
				error("putchar returned '%c', '%c' was expected",
					ret_val, ch);
			}
		}
	}

	expecting (SUCCESS);
	fclose(stdout);

	pop_stdout();
	expecting (SUCCESS);
	fp = temp_fopen("tmpputchar","r");

	i = 'z';
	expecting (VOID);
	while ( (ret_val = getc(fp)) != EOF )
	{
		if ( i < 'a' )
		{
			error("putchar wrote more characters to the file \
than it was supposed to");
		}
		if ( ret_val != i )
		{
			error("putchar wrote '%c', '%c' was expected",
				ret_val, i);
		}
		i--;
	}

	if ( i >= 'a' )
	{
		error("putchar did not write all the characters out \
to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis
}

/*
** t_putw
**	Test the put 'word' routine.
*/
t_putw()
{
	FILE	*fp;
	int	i;
	int	ret_val;

testing("Multiple calls to putw");
testing("should return 0, and a file of integers.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpputw","w");

	for ( i = 0 ; i < NUMINTS ; i++ )
	{
		expecting (VOID);
		if ( (ret_val = putw(i,fp)) != 0 )
		{
			if ( ret_val == EOF )
			{
				error("putw returned EOF while writing %d", i);
			}

			error("putw returned %d while writing %d",
				ret_val, i);
		}
	}

	expecting (VOID);
	if ( (ret_val = putw(-2,fp)) != 0 )
	{
		if ( ret_val == EOF )
			error("putw returned EOF while writing -2.");
		else
			error("putw returned %d while writing -2.",
				ret_val);
	}

	expecting (SUCCESS);
	fclose(fp);

	expecting (SUCCESS);
	fp = temp_fopen("tmpputw","r");

	i = 0;

	expecting (VOID);
	while ( (ret_val = getw(fp)) != EOF )
	{
		if ( i > (NUMINTS + 1) )
		{
			error("putw wrote too many integers into the file");
		}
		if ( ret_val != i )
		{
			if ( i != NUMINTS  )
			{
				error("putw wrote %d, %d was exepcted",
					ret_val, i);
				return;
			}
			if ( ret_val != -2 )
			{
				error("putw wrote %d, -2 was expected",
					ret_val);
			}
		}
		i++;
	}

	if ( i != (NUMINTS + 1) )
	{
		error("putw did not write all the integers to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis
}
