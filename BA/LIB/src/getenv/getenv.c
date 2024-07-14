#include	<sv_base.h> /*replacement macros */
#include	<ctype.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)getenv.c	1.2"



extern	char	*getenv();		/* Function itself... */
/*
** getenv
**	Test the getenv routine.
*/
main()
{
	char	*sp;
	char	*pp;

setup("getenv");
funct("getenv");


testing("The guaranteed environment variable HOME should exist");
testing("with a string with a 'reasonable' pathname in it.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = getenv("HOME");

	if ( *sp != '/' )
		error (
		"getenv (HOME) returned '%s', which doesn't begin with a '/'",
		sp);
	pp = sp;

	while ( *pp )
	{
		if ( *pp < 040 )
		{
			error (
"getenv(HOME) returned a string with the control character '%o' in it",
*pp);
		}
		else
			pp++;
	}
finis
	
testing("The guaranteed environment variable PATH should exist with");
testing("a string with a 'reasonable' pathname in it.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = getenv("PATH");
	if ( *sp != ':' && *sp != '/' )
		error("Path of '%s'\n\
returned by getenv(PATH) does not start with either a ':', or a '/'",
sp);
finis


testing("The guaranteed environment variable TERM should exist set to");
testing("a string with a terminal name in it.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = getenv("TERM");
	pp = sp;

	while ( *pp )
	{
		if ( *pp < 040 )
		{
		error (
"getenv(TERM) returned a string with the control character '%o' in it",
*pp);
		}
		else
			pp++;
	}
finis
	

testing("Looking for the environment variable TZ");
testing("Should yield a string representing the current time zone back.");
start
	break_on (ERROR);
	expecting (SUCCESS);
	sp = getenv("TZ");
	expecting (VOID);  /*  for the isalpha  */
	if ( !isalpha(*sp) || !isalpha(sp[1]) || !isalpha(sp[2]) )
		error ("TZ '%s'returned from getenv is malformed",sp);

	if ( sp[3] == '-' )
	{
		if ( !isdigit(sp[4]) )
			error ("TZ '%s'returned from getenv is malformed",sp);
	}
	else
		if ( !isdigit(sp[3]) )
			error ("TZ '%s'returned from getenv is malformed",sp);
finis
	

testing("The partial string 'PAT' should not match, returning NULL.");
start
	break_on (ERROR);
	expecting (NULL);
	sp = getenv("PAT");
finis
	

testing("The partial string 'T' should not match, returning NULL.");
start
	break_on (ERROR);
	expecting (NULL);

	sp = getenv("T");
finis
	

testing("The partial string 'TERMI' should not match, returning NULL.");
start
	expecting (NULL);
	getenv("TERMI");
finis
	

testing("The similar string 'Home' should not match, returning NULL.");
start
	expecting (NULL);
	getenv("Home");
finis
	

testing("A totally unknown environment variable should not found, returning NULL.");
start
	expecting (NULL);
	getenv("__NOTEVENREMOTELY!@@#$%PoSsIbLe");
finis
	

done();
/*NOTREACHED*/

}
