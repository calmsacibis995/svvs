#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<svvs.h>


#ident	"@(#)tmpnam.c	1.4"



extern	char	*getenv();
extern	char	*malloc();

/*
** tmpnam
**	Test the tmpnam and tempnam functions.
*/
main()
{
	char		*file1_name;
	char		*file2_name;
	char		test1_name[L_tmpnam];
	char		test2_name[L_tmpnam];
	char		*save_name;
	char		*svvs_temp;
#include	<sv_env.h>
#include	<sv_macro.h>
	char		buf[STRLEN];

	extern	char	*tmpnam();
	extern	char	*tempnam();
	extern	char	*temp_path();

setup("tmpnam");
funct("tmpnam");


testing ("The environment variable TMPDIR must not be defined");
start
	/*
	** The environment variable TMPDIR must be
	** undefined for the following tests.
	*/
	expecting (VOID);
	if ( getenv("TMPDIR") != NULL )
		error ("The environment variable TMPDIR is defined");
finis


testing("A temp file name created using tmpnam");
testing("should have a directory name of P_tmpdir.");
start
	/*
	** First call tmpnam with 'test1' to get the
	** directory name.
	*/
	strcpy(test1_name, "test1");
	expecting (VOID);
	file1_name = tmpnam(test1_name);

	/*
	** save the file name
	*/
	expecting (SUCCESS);
	save_name = (char *)malloc(strlen(file1_name) + 1);

	expecting (VOID);
	strncpy(save_name, file1_name, strlen(file1_name));

	if ( strncmp(file1_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tmpnam with 'test1' returned '%s'",
			file1_name);
finis


testing("A temp file name created using tmpnam");
testing("has a directory name of P_tmpdir.");
testing("This call should return a different file name from the previous call.");
start
	/*
	** First call tmpnam with 'test2' to get the
	** directory name.
	*/

	expecting (VOID);
	strcpy(test2_name, "test2");
	file2_name = tmpnam(test2_name);
	if ( strncmp(file2_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tmpnam with 'test2' returned '%s'",
			file2_name);

	if ( strcmp(save_name, file2_name) == 0 )
	{
		testing(save_name, " :Different from");
		error ("%s %s\n'%s' %s '%s'",
			"The call to tmpnam with 'test2' did not return a",
			"different file name",
			save_name, "is not different from", file2_name);
	}

	expecting (VOID);
	free(save_name);
finis


testing("A temp file name created using tmpnam");
testing("with a NULL string has a directory name of P_tmpdir");
testing("should return a directory name of P_tmpdir for the temp file.");
start
	/*
	** First call tmpnam with a NULL string to get the
	** directory name.
	*/
	expecting (VOID);
	file1_name = tmpnam(NULL);

	/*
	** save the file name
	*/
	expecting (SUCCESS);
	save_name = (char *)malloc(strlen(file1_name) + 1);

	expecting (VOID);
	strncpy(save_name, file1_name, strlen(file1_name));

	if ( strncmp(file1_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tmpnam with a NULL returned '%s'",
			file1_name);
finis


testing("A temp file name created using tmpnam");
testing("with a NULL has a directory name of P_tmpdir and the");
testing("file name is different from the last call should return");
testing("a directory name of P_tmpdir for the temp file and");
testing("a different file name from the previous call.");
start
	/*
	** First call tmpnam with a NULL string to get the
	** directory name.
	*/

	expecting (VOID);
	file2_name = tmpnam(NULL);
	if ( strncmp(file2_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tmpnam with a NULL returned '%s'\n%s",
			file2_name, "an incorrect directory name");

	if ( strcmp(save_name, file2_name) == 0 )
	{
		error ("%s %s\n'%s' %s '%s'",
			"The call to tmpnam with 'test2' did not return a",
			"different file name",
			save_name, "is not different from", file2_name);
	}

	expecting (VOID);
	free(save_name);
finis


funct("tempnam");
testing("A temp file name created using tempnam");
testing("with NULL params has a directory name of P_tmpdir");
testing("should return a directory name of P_tmpdir for the temp file.");
start
	/*
	** First call tempnam with NULL params to get the
	** directory name.
	*/
	expecting (VOID);
	file1_name = tempnam(NULL, NULL);

	/*
	** save the file name
	*/
	expecting (SUCCESS);
	save_name = (char *)malloc(strlen(file1_name) + 1);

	expecting (VOID);
	strncpy(save_name, file1_name, strlen(file1_name));

	if ( strncmp(file1_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tempnam with a NULL returned '%s'\n%s",
			file1_name, "an incorrect directory name");
finis



testing("A temp file name, created using tempnam");
testing("with NULL, has a directory name of P_tmpdir and");
testing("should return a file name that is different from the last call.");
start
	/*
	** First call tempnam with NULL params to get the
	** directory name.
	*/

	expecting (VOID);
	file2_name = tempnam(NULL, NULL);
	if ( strncmp(file2_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tempnam with a NULL returned '%s'\n%s",
			file2_name, "an incorrect directory name");

	if ( strcmp(save_name, file2_name) == 0 )
	{
		error ("%s %s\n'%s' %s '%s'",
			"The call to tmpnam with a NULL  did not return a",
			"different file name",
			save_name, "is not different from", file2_name);
	}
finis


testing("A temp file name created using tempnam");
testing("with the svvs temp directory");
testing("should return a directory name same as the SVVS temp directory.");
start
	/*
	** First call tempnam with the SVVS temp directory
	*/
	expecting (VOID);
	svvs_temp = temp_path();
	strcpy(test1_name, svvs_temp);
	test1_name[strlen(test1_name)-1] = 0;
	file1_name = tempnam(test1_name, NULL);

	if ( strncmp(file1_name, test1_name, strlen(test1_name)) != 0 )
		error ("The call to tempnam with the svvs temp directory \n\
returned '%s', an incorrect directory name\n'%s' was expected",
file1_name, test1_name);
finis


testing("A temp file name created using tempnam");
testing("with the svvs temp directory and a prefix");
testing("should return a directory name same as the SVVS temp directory");
testing("and the correct prefix.");
start
	/*
	** First call tempnam with the SVVS temp directory
	*/
	expecting (VOID);
	svvs_temp = temp_path();
	strcpy(test1_name, svvs_temp);
	test1_name[strlen(test1_name)-1] = 0;
	file1_name = tempnam(test1_name, "svvs");
	
	if ( strncmp(file1_name, test1_name, strlen(test1_name)) != 0 )
		error ("The call to tempnam with the svvs temp directory \n\
returned '%s', an incorrect directory name\n'%s' was expected",
file1_name, test1_name);

	if ( strncmp(&file1_name[strlen(test1_name) +1], "svvs", strlen("svvs")) != 0 )
		error ("The call to tempnam with a prefix of 'svvs' failed,\n\
returning '%s'", file1_name);
finis



testing("A temp file name created using tempnam");
testing("with no directory name and a prefix");
testing("should return a directory name same as P_tmpdir");
testing ("and the correct prefix.");
start
	/*
	** First call tempnam with a NULL dir and prefix
	*/
	expecting (VOID);
	file1_name = tempnam(NULL, "svvs");
	
	if ( strncmp(file1_name, P_tmpdir, strlen(P_tmpdir)) != 0 )
		error ("The call to tempnam with the svvs temp directory\n\
returned '%s', an incorrect directory name", file1_name);

	if ( strncmp(&file1_name[strlen(P_tmpdir)], "svvs", strlen("svvs")) != 0 )
		error ("The call to tempnam with a prefix of 'svvs' failed,\n\
returning '%s'", file1_name);
finis


testing("A temp file name created using tempnam");
testing("having a directory name contained in TMPDIR");
testing("should return a directory name same as that");
testing("contained in TMPDIR for the temp file.");
start
	/*
	** Set the environment variable TMPDIR to the SVVS 
	** temp directory.
	*/
	expecting (VOID);
	strcpy(buf,"TMPDIR=");
	strcat (buf,temp_path());
	buf[strlen(buf)-1] = 0;

	expecting (SUCCESS);
	putenv(buf);
	save_name = getenv("TMPDIR");

	/*
	** First call tempnam with TMPDIR set to get the
	** directory name.
	*/
	expecting (VOID);
	file1_name = tempnam(NULL, NULL);

	if ( strncmp(file1_name, save_name, strlen(save_name)) != 0 )
		error ("%s\n'%s' %s\n%s %s",
			"The call to tempnam with TMPNAM set returned",
			file1_name,
			"an incorrect directory name",
			save_name, "was expected");
finis


done();
/*NOTREACHED*/

}
