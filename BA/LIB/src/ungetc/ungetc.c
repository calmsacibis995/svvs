#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)ungetc.c	1.2"



extern	int	ungetc();
extern	FILE	*data_fopen();

/*
** ungetc
**	Check that ungetc works.
*/
main()
{
	int	c;
	FILE	*fp;

setup("ungetc");
funct("ungetc");

testing("Read part of a file, ungetc a character ('0') not in the file,");
testing("then do a getc.");
testing("The ungetc should return '0', and the next getc should  return a '0'.");
start
	expecting (SUCCESS);
	fp = data_fopen("dataungetc","r");

	expecting (VOID);
	while ( (c = getc(fp)) != EOF && c != 'm' )
		continue;

	if ( c == EOF )
		error ("Unexpected EOF in data file 'ungetc'");

	expecting ('0');
	ungetc('0',fp);

	expecting ('0');
	getc(fp);
finis


testing("ungetc(EOF) should return EOF");
testing("and a 'n' from the next call to getc.");
start
	expecting (EOF);
	ungetc(EOF,fp);

	expecting ('n');
	getc(fp);

	expecting (SUCCESS);
	fclose(fp);
finis

done();
/*NOTREACHED*/
}
