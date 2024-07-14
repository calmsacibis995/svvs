#include	<sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<sys/types.h>
#include	<sys/stat.h>
#endif

#include	<fcntl.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ifndef SYSVR_2
# define 	WORLD	S_IWOTH | S_IROTH | S_IWGRP | S_IRGRP | S_IWRITE | S_IREAD
# else
# define	WORLD	0666
# endif

#ident	"@(#)mktemp.c	1.5"



extern	char	*mktemp();
extern	char	*temp_path();

/*
** mktemp
**	Check that the mktemp routine works.
*/
main()
{
	int	i;
	int	pid;	/* used for processing the actual pid  */
	char	PathPointer[PATH_MAX];
	char	buffer[13];
	char	pidbuf[10];
	char	*sp;
	char	*pp;

setup("mktemp");
funct("mktemp");

testing("mktemp with a legal string");
testing("should return a string of the form 'mktempa#####'.");
start
	expecting (VOID);
	strcpy(PathPointer,temp_path());
	PathPointer[strlen(PathPointer)-1] = 0;
	expecting (SUCCESS);
	chdir(PathPointer);

	expecting (VOID);
	strcpy(buffer,"mktempXXXXXX");
	expecting (VOID);  /*  I am testing the return myself  */
	sp = mktemp(buffer);
	if ( !sp )
		error ("%s\nmktemp returned NULL",FUTURDIR);
	if ( !*sp )
		error ("mktemp failed on the first call, \
returning a (null str)");
	if ( sp != buffer )
		error ("mktemp did not return the buffer passed it");
	expecting (VOID);
	if ( strncmp("mktempa",sp,7) )
		error ("mktemp did not set the first 7 characters correctly\n\
returning '%.7s' instead of 'mktempa'", sp);

/*
 *  Convert the pid from binary to characters,
 *  using only 5 digits
 *  and leading zeroes if necessary
 */
expecting (VOID);
pid = getpid ();
pp = pidbuf + 5;

*pp = '\0';
while (--pp >= pidbuf)
	{
	*pp = (pid % 10) + '0';
	pid /= 10;
	}

	expecting (VOID);
	if ( strncmp(&sp[7],pidbuf,5) )
		error ("mktemp incorrectly set the digit part of the file name\n\
'%s' was returned, '%s' was expected", &sp[7], pidbuf);
	mkfile(buffer);
finis

	
testing("mktemp changes a string which is only 6 X's.");
testing("'a#####'");
start
	for(;;)
	{

		expecting (VOID);
		strcpy(buffer,"XXXXXX");
		sp = mktemp(buffer);
		if ( !sp )
			error ("%s\nmktemp returned null", FUTURDIR);
		if ( !sp || !*sp )
		{
			error ("mktemp failed returnig (null) or (null str)");
			break;
		}
		if ( *sp != 'a' )
		{
			error ("mktemp did not place an 'a' in the string, \
returning %s.", sp);
			break;
		}
		if ( strncmp(&sp[1],pidbuf,5) )
		{
			error("mktemp did not place the correct number into \
the template\nreturning '%s' where '%s' was expected",&sp[1],pidbuf);
			break;
		}
		break;
	}
finis


testing("a total of 26 calls to mktemp can be made");
testing("yielding 26 valid returns.");
start
	for ( i = 0 ; i < 25 ; i++ )
	{
		expecting (VOID);
		strcpy(buffer,"mktempXXXXXX");
		sp = mktemp(buffer);
		if ( !sp )
			error ("%s\nmktemp returned null", FUTURDIR);
		if ( !sp || !*sp )
			error("mktemp failed returning (null) or (null str)");
		mkfile(buffer);
	}
finis


testing("a 27th call to mktemp should fail");
testing("returning (null str).");
start

	expecting (VOID);
	strcpy(buffer,"mktempXXXXXX");
	sp = mktemp(buffer);
	if ( !sp )
		error ("%s\nmktemp returned null", FUTURDIR);
	if ( sp && *sp )
		error ("mktemp did fail on the 27th call, returning '%s'",
			sp);
	removeall(pidbuf);
finis

done();
/*NOTREACHED*/
}

/*
** mkfile
**	Make a temp file.
*/
mkfile(file)
char	*file;
{
	int	fd;
	char	errstr[STRLEN];

	expecting (VOID);
	if ( (fd = open(file,O_CREAT|O_TRUNC,WORLD)) == -1 )
	{
		error ("can not create temp file '%s'",file);
	}
	if ( close(fd) )
		error ("close(fd = temp file) returned non-zero");
}

/*
** removeall
**	Delete all of the temporaries we just made.
*/
removeall(pidbuf)
char	*pidbuf;
{
	char	buffer[STRLEN];
	char	errstring[STRLEN];
	int	i;

	expecting (VOID);
	strcpy(buffer,"mktempa");
	strcat(buffer,pidbuf);

	for ( i = 0 ; i < 26 ; i++ )
	{
		if ( unlink(buffer) )
		{
			error ("Could not unlink temp file '%s'",buffer);
		}
		buffer[6]++;
	}
}
