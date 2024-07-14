#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)puts.c	1.2"



extern	FILE	*temp_fopen();
extern	char	*fgets();

/*
** puts
**	Test the puts routines.
*/
main()
{
setup("puts");

funct ("puts");
start
	continue_on (WARNING);
	t_puts();
finis

funct ("fputs");
start
	continue_on (WARNING);
	t_fputs();
finis

done();
/*NOTREACHED*/
}

/*
** t_puts
**	Test the puts routine.
*/
t_puts()
{
	int	r;
	char	buffer[STRLEN];		/* buffer from fgets */
	FILE	*fp;
	char	*firstline;
	char	*secondline;
	char	*thirdline;

testing("Writing a file with puts should cause puts to write a file.");
start
	firstline = "The First Line.";
	secondline = "The Second Line.";
	thirdline = "The VERY last line.";

	push_stdout("tmpputs","w");

	expecting (VOID);
	if ( puts(firstline) == EOF )
	{
		pop_stdout();
		error ("puts returned EOF attempting to write the first line");
	}
	if ( puts(secondline) == EOF )
	{
		pop_stdout();
		error("puts returned EOF attempting to write the second line");
	}
	if ( puts(thirdline) == EOF )
	{
		pop_stdout();
		error("puts returned EOF attempting to write the third line");
	}
	pop_stdout();

	expecting (SUCCESS);
	fp = temp_fopen("tmpputs","r");

	expecting (VOID);
	if ( fgets(buffer, STRLEN, fp) == NULL )
	{
		error ("puts did not write anything to the file");
	}

	expecting (VOID);
	if ( strncmp(firstline, buffer, strlen(firstline)) )
	{
		error ("the first line is '%s', should be '%s'",
			buffer, firstline);
	}

	if ( buffer[strlen(firstline)] != '\n' )
	{
		error ("puts did not put a new-line out at the end of the \
first line");
	}

	if ( buffer[strlen(firstline) + 1] != '\0' )
	{
		error ("puts wrote more than the line passed to it to the file");
	}

	expecting (VOID);
	if ( fgets(buffer, STRLEN, fp) == NULL )
	{
		error ("puts did not write the second line to the file");
	}

	if ( strncmp(secondline,buffer,strlen(secondline)) )
	{
		error ("the second line is '%s', should be '%s'",
			buffer, secondline);
	}

	if ( buffer[strlen(secondline)] != '\n' )
	{
		error ("puts did not put a new-line out at the end of the \
second line");
	}

	if ( buffer[strlen(secondline) + 1] != '\0' )
	{
		error ("puts wrote more than the line passed to it to the file");
	}

	expecting (VOID);
	if ( fgets(buffer, STRLEN, fp) == NULL )
	{
		error ("puts did not write the third line to the file");
	}

	if ( strncmp(thirdline,buffer,strlen(thirdline)) )
	{
		error ("the third line is '%s', should be '%s'",
			buffer, thirdline);
	}

	if ( buffer[strlen(thirdline)] != '\n' )
	{
		error ("puts did not put a new-line out at the end of the line");
	}

	if ( buffer[strlen(thirdline) + 1] != '\0' )
	{
		error ("puts wrote more than the line passed to it to the file");
	}

	expecting (VOID);
	if ( fgets(buffer,STRLEN,fp) != NULL )
	{
		error ("puts wrote more than three lines to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis


testing("puts(\"\") should write a file with a new-line in it.");
start
	push_stdout("tmpputs","w");

	expecting (SUCCESS);
	puts("");
	pop_stdout();

	expecting (SUCCESS);
	fp = temp_fopen("tmpputs","r");

	expecting (SUCCESS);
	fgets(buffer, STRLEN, fp);

	if ( buffer[0] != '\n' )
	{
		error ("puts(\"\") did not write out a new-line");
	}

	if ( buffer[1] != '\0' )
	{
		error ("puts(\"\") wrote more than just a new-line");
	}

	expecting (SUCCESS);
	fclose(fp);
finis


testing("if stdout is not open for writing, puts should return EOF.");
start
	push_stdout("tmpputs","r");

	expecting (VOID);
	if ( (r = puts("Hi there")) != EOF )
	{
		if(r == 0)
			warning ("%s\nputs returned 0.", ADVISORY);
		else
			error("puts() returned %d", r);
	}
	pop_stdout();
finis


return;
}

/*
** t_fputs
**	Test the fputs routine.
*/
t_fputs()
{
	char	buffer[STRLEN];		/* buffer from fgets */
	FILE	*fp;
	int	ret_val;
	char	*firstline;
	char	*secondline;
	char	*thirdline;

testing("Writing a file with fputs should cause fputs to write a file.");
start
	firstline = "The First Line.\n";
	secondline = "The Second Line.\n";
	thirdline = "The VERY last line.";

	expecting (SUCCESS);
	fp = temp_fopen("tmpfputs","w");

	fputs(firstline,fp);
	fputs(secondline,fp);
	fputs(thirdline,fp);

	fclose(fp);

	fp = temp_fopen("tmpfputs","r");

	expecting (VOID);
	if ( fgets(buffer, STRLEN, fp) == NULL )
	{
		error ("fputs did not write anything to the file");
	}

	expecting (VOID);
	if ( strncmp(firstline,buffer,strlen(firstline)) )
	{
		error ("the first line is '%s', should be '%s'",
			buffer, firstline);
	}

	if ( buffer[strlen(firstline)] != '\0' )
	{
		error ("fputs wrote more than the line passed to it to the file");
	}

	if ( fgets(buffer, STRLEN, fp) == NULL )
	{
		error ("fputs did not write the second line to the file");
	}

	if ( strncmp(secondline,buffer,strlen(secondline)) )
	{
		error ("the second line is '%s', should be '%s'",
			buffer, secondline);
	}

	if ( buffer[strlen(secondline)] != '\0' )
	{
		error ("fputs wrote more than the line passed to it to the file");
	}

	if ( fgets(buffer, STRLEN, fp) == NULL )
	{
		error ("fputs did not write the third line to the file");
	}

	if ( strncmp(thirdline,buffer,strlen(thirdline)) )
	{
		error ("the third line is '%s', should be '%s'",
			buffer, thirdline);
	}

	if ( buffer[strlen(thirdline)] != '\0' )
	{
		error ("fputs did not put a NULL out at the end of the line");
	}

	if ( fgets(buffer,STRLEN,fp) != NULL )
	{
		error ("fputs wrote more than three lines to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis


testing("fputs(\"\") should create a file with a new-line in it.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpfputs","w");

	fputs("",fp);

	fclose(fp);

	fp = temp_fopen("tmpfputs","r");

	expecting (VOID);
	if ( fgets(buffer, STRLEN, fp) != NULL )
	{
		error ("fputs(\"\") wrote something to the file");
	}

	expecting (SUCCESS);
	fclose(fp);
finis


testing("fputs if the FILE is not open for writing should return EOF.");
start
	expecting (SUCCESS);
	fp = temp_fopen("tmpfputs","r");

	expecting (VOID);
	if ((ret_val = fputs("Hi there",fp)) != EOF)
	{
		if(ret_val == 0)
			warning ("%s\nfputs returned 0", ADVISORY);
		else
			error("fputs() returned %d", ret_val);
	}
	expecting (SUCCESS);
	fclose(fp);
finis
}
