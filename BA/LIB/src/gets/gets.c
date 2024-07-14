#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)gets.c	1.3"



extern	FILE	*data_fopen();
extern	FILE	*temp_fopen();

/*
** test for the gets, and fgets routines.
*/
main()
{
setup("gets");
setstdin("datagets");
continue_on (ERROR);

funct ("gets");
start
	t_gets();
finis

funct ("fgets");
start
	t_fgets();
finis

done();
/*NOTREACHED*/
}

/*
** setsdtin
**	Set the FILE stdin to the filename passed
**
** Arguments
**	filename	Filename to be reassigned to stdin
*/
setstdin(file)
char	*file;
{
	char	buf[STRLEN];

	exit_on (ERROR);
	expecting (SUCCESS);

	fclose(stdin);
	data_fopen(file,"r");
}

char	gets1[] = "abcdefghijklmnopqrstuvwxyz";
char	gets2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char	gets3[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
/*
** t_gets
**	Test the gets routine.
*/
t_gets()
{
	char	buffer[2050];		/* VERY large */
	char	*sp;

testing("A file contains three (different) lines .");
testing("The initial line read should be 'abcdefghijklmnopqrstuvwxyz'.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = gets(buffer);

	if ( sp != buffer )
	{
		error("gets did not return the array it was passed");
		return;
	}

	expecting (VOID);  /*  strcmp can return any value  */
	if ( strcmp(sp,gets1) )
	{
		error ("incorrect first line = '%s'", sp);
	}
finis


testing("The second line should be 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.");
start
	break_on (ERROR);
	expecting (SUCCESS);

	sp = gets(buffer);
	if ( sp != buffer )
	{
		error("gets did not return the array it was passed");
	}
	expecting (VOID);  /*  strcmp can return any value  */
	if ( strcmp(sp,gets2) )
	{
		error("incorrect second line = '%s'", sp);
	}
finis


testing("The third line should be");
testing("'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()'");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = gets(buffer);
	if ( sp != buffer )
	{
		error("gets did not return the array it was passed");
	}
	expecting (VOID);  /*  strcmp can return any value  */
	if ( strcmp(sp,gets3) )
	{
		error ("incorrect third line = '%s'", sp);
	}
finis


testing("gets at EOF should return NULL.");
start
	expecting ((char *) 0);
	sp = gets(buffer);
finis


testing("gets on a file that has an EOF, but no new-line");
testing ("should return 'abcdefghijklmnopqrstuvwxyz'.");
start
	break_on (ERROR);
	setstdin("datanonl");
	expecting (SUCCESS);
	sp = gets(buffer);
	expecting (VOID);
	if ( strcmp(sp,gets1) )
	{
		error("gets did not return the correct string");
	}
finis

}

char	fgets1[] = "abcdefghijklmnopqrstuvwxyz\n";
char	fgets2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
char	fgets3[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()\n";
/*
** t_fgets
**	Test the fgets routine.
*/
t_fgets()
{
	char	buffer[2050];		/* VERY large */
	char	*sp;
	char	*fg;
	FILE	*fp;

testing("Reading a file with three (different) lines.");
testing("The initial line should be 'abcdefghijklmnopqrstuvwxyz\\n'.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	fp = data_fopen("datagets","r");

	expecting (SUCCESS);
	sp = fgets(buffer,sizeof (buffer), fp);
	if ( sp != buffer )
	{
		error("fgets did not return the array it was passed");
	}
	expecting (VOID);
	if ( strcmp(sp,fgets1) )
	{
		error ("incorrect first line = '%s'", sp);
	}
finis


testing("The second line should be 'ABCDEFGHIJKLMNOPQRSTUVWXYZ\\n'.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = fgets(buffer,sizeof (buffer), fp);
	if ( sp != buffer )
	{
		error("fgets did not return the array it was passed");
	}
	expecting (VOID);  /*  strcmp can return any value  */
	if ( strcmp(sp,fgets2) )
	{
		error ("incorrect second line = '%s'", sp);
	}
finis


testing("The third line should be");
testing("'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()\\n'");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = fgets(buffer,sizeof (buffer), fp);
	if ( sp != buffer )
	{
		error("fgets did not return the array it was passed");
	}
	expecting (VOID);  /*  strcmp can return any value  */
	if ( strcmp(sp,fgets3) )
	{
		error ("Incorrect third line = '%s'", sp);
	}
finis


testing("fgets at EOF should return NULL.");
start
	break_on (ERROR);
	expecting (NULL);
	sp = fgets(buffer,sizeof (buffer), fp);
finis


testing("fgets on a file that has an EOF, but no new-line");
testing("should return 'abcdefghijklmnopqrstuvwxyz'.");
start
	break_on (ERROR);
	expecting (SUCCESS);

	fclose(fp);

	fp = data_fopen("datanonl","r");

	sp = fgets(buffer,sizeof (buffer), fp);
	expecting (VOID);
	if ( strcmp(sp,gets1) )
	{
		error("fgets returned the incorrect string '%s'", sp);
	}
	expecting (SUCCESS);
	fclose(fp);
finis


testing("Reading a file with fgets, and a n of 1");
testing("should read the file back.");
start
	break_on (ERROR);
	expecting (SUCCESS);

	fp = data_fopen("datagets","r");
	
	fg = fgets1;
	while ( *fg )
	{
		expecting (SUCCESS);
		sp = fgets(buffer,2,fp);

		if ( *sp != *fg )
		{
			error ("fgets returned the wrong character '%c'",
				*sp);
		}
		if ( sp[1] != '\0' )
		{
			error("fgets did not put a NULL byte at the second byte of the string");
		}
		fg++;
	}
	fg = fgets2;
	while ( *fg )
	{
		expecting (SUCCESS);
		sp = fgets(buffer,2,fp);
		if ( *sp != *fg )
		{
			error ("fgets returned the wrong character '%c'\non reading the second line",*sp);
		}
		if ( sp[1] != '\0' )
		{
			error ("fgets did not put a NULL byte at the\nsecond byte of the string on reading the second line");
		}
		fg++;
	}
	fg = fgets3;
	while ( *fg )
	{
		expecting (SUCCESS);
		sp = fgets(buffer,2,fp);
		if ( *sp != *fg )
		{
			error ("fgets returned the wrong character '%c'\non reading the third line",*sp);
		}
		if ( sp[1] != '\0' )
		{
			error ("fgets did not put a NULL byte at the\nthird byte of the string on reading the second line");
		}
		fg++;
	}
	expecting (NULL);  /*  EOF should return NULL  */
	fgets(buffer,2,fp);
	expecting (SUCCESS);
	fclose(fp);
finis


testing("Using fgets on a write only file should return NULL.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	fp = temp_fopen("tmpgets","w");
	fprintf(fp,"Hi there");

	expecting (VOID);
	if ((sp = fgets(buffer,sizeof (buffer), fp)) != NULL)
	{
		if (sp == buffer)
			warning ("%s\nfgets returned non-null\nwhen trying to read from a write only file",
				ADVISORY);
		else
			error("fgets returned non-null\nwhen trying to read from a write only file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis

}
