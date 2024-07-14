#include	<sv_base.h> /*replacement macros */
/*
** The following defines and variable declarations
** need to remain where they are with respect to the 
** first # include <regexp.h>.  Please do not change
** the order.
*/
# define	ESIZE		100
char		Instring[ESIZE];
char		Linebuf[10*ESIZE];
char		Expbuf[ESIZE];

# define	INIT		register char *sp = Instring;
# define	GETC()		(*sp++)
# define	PEEKC()		(*sp)
# define	UNGETC(c)	(--sp)
# define	RETURN(c)	return((char *)0)
# define	ERROR(c)	regerr()
#include	<regexp.h>

/*
 *  see regexp (5) for an explanation of subroutines
 *  COMPILE, STEP, and ADVANCE
 */

/*
** The following portion of the header/define/variable can
** be changed to have any order.  The lines above are the
** only portion with a specific line ordering.
*/
# undef ERROR
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)regexp.c	1.3"



# define	MAXNUM		100
# define	MAXNUM2		128

char			*Substr;
extern	char		*compile();
extern	int 		step();
extern	int 		advance();
extern	char		*loc1;
extern	char		*loc2;
extern	char		*locs;

main()
{

setup("regexp");

funct("compile");
funct("step");
funct("advance");
start

	Substr = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+={}[]`~|\\:;\"'?/<>,.\n\0";

testing("Using COMPILE, compile an expression consisting of '.'.");
testing("Executing this should match any char.");
start
	r_dot();
finis

testing("Using COMPILE, compile an expression consisting of '.*'.");
testing("Executing this should match the entire line");
testing("except the new_line character.");
start
	r_dotstar();
finis

testing("Using COMPILE, compile an expression consisting of 'A*'.");
testing("Executing this should match any number of");
testing("consecutive A's (including 0). ");
start
	r_Astar();
finis

testing("Using COMPILE, compile an expression consisting of '[A-Z]'.");
testing("Executing this should match only single upper case characters.");
start
	r1_AtoZ();
finis

testing("Using COMPILE, compile an expression consisting of '[A-K]'.");
testing("Executing this should match only single upper case characters");
testing("up to and including K.");
start
	r_AtoK();
finis

testing("Using COMPILE, compile an expression consisting of '[0-9]'.");
testing("Executing this should match only single digits");
testing("up to and including 9.");
start
	r_0to9();
finis

testing("Using COMPILE, compile an expression consisting of '[0-5]'.");
testing("Executing this should match only single digits up to and including 5.");
start
	r_0to5();
finis

testing("Using COMPILE, compile an expression consisting of '[^A-Z]'.");
testing("Executing this should match the range other than");
testing("'A' through 'Z'.");
start
	r2_AtoZ();
finis

testing("Using COMPILE, compile an expression consisting of '^a'.");
testing("Executing this should match only a's at the beginning of lines.");
start
	r_hata();
finis

testing("Using COMPILE, compile an expression consisting of '[A-Za-z]+'.");
testing("Executing this should match only one or more ");
testing("occurrences of upper and lower case or spaces.");
start
	r6_AtoZatoz();
finis

testing("Using COMPILE, compile an expression consisting of '[A-Z][a-z]$'.");
testing("Executing this should match only a 2 character");
testing("string at the end of the line.");
start
	r7_AtoZatoz();
finis

testing("Using COMPILE, compile an expression consisting of '[A-Z]+'.");
testing("Executing this should not match lower case characters.");
start
	r3_AtoZ();
finis

testing("Using COMPILE, compile an expression consisting of 'A\\{0,\\}'.");
testing("Executing this matches at least zero occurrences of A.");
start
	r_Azero();
finis

testing("Using COMPILE, compile an expression consisting of 'A\\{2,\\}'.");
testing("Executing this should match only two or more ");
testing("occurrences of the character 'A'.");
start
	r_Atwo();
finis

testing("Using COMPILE, compile an expression consisting of '^[A-Z]\\{4,\\}'.");
testing("Executing this should match at least 4 upper case");
testing("characters at the beginning of the line.");
start
	r5_AtoZ();
finis

testing("Using COMPILE, compile an expression consisting of 'A\\{4,5\\}'.");
testing("Executing this should match only four or five ");
testing("occurrences the char 'A'.");
start
	r_Afour();
finis

testing("Using COMPILE, compile an expression consisting of '^[a-z]\\{4,8\\}'.");
testing("Executing this should match only four to eight lower case chars.");
start
	r_atoz();
finis

finis


done();
/*NOTREACHED*/
}

/*
** r_dot()
**	compile an expression consisting of ".".  Check that executing
**	this will match any char including the new_line char '\n'.
**
** Level:
**	This test is in the Normal level (0).
*/
r_dot()
{
	int		i;

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, ".");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	for (i = 1; i < MAXNUM2; i++)
	{
		expecting (SUCCESS);
		sprintf(Linebuf, "%c", i);

		expecting (SUCCESS);
		step(Linebuf, Expbuf);
		if ( *loc1 != *Linebuf )
			error ("step did not match '.' to all chars");
	}
}

/*
** r_dotstar
**	compile an expression consisting of ".*".  Check that executing
**	this will match entire lines including the new_line char '\n'.
**
** Level:
**	This test is in the normal level (0).
*/
r_dotstar()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, ".*");
	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (SUCCESS);
	step(Substr, Expbuf) != 0;
	if ( *loc1 != *Substr )
		error ("step did not match '.*' to all chars");
	if ( *loc2 )
		error ("step did not match '.*' to entire string");
}

/*
** r_Astar()
**	compile an expression consisting of "A*".  Check that executing
**	this will match any number of consecutive A's (including 0). 
**
** Level:
**	This test is in the Normal level (0)
*/
r_Astar()
{
	int		i;

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "A*");
	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');
	Linebuf[0] = '\0'; 
	for (i = 1; i < MAXNUM; i++)
	{
		expecting (SUCCESS);
		step(Linebuf, Expbuf);
		if ( *loc1 != *Linebuf )
			error ("step did not match 'A*' to any number of \
consecutive A's");
		if ( *loc2 != '\0' )
			error ("loc2 for a string of A's should point to\n\
the null char at the end of the string.");

		expecting (VOID);
		strcat (Linebuf, "A");
	}
}

/*
** r1_AtoZ()
**	compile an expression consisting of "[A-Z]".  Check that executing
**	this will match only single upper case chars.
**
** Level:
**	This test is in the Normal level (0).
*/
r1_AtoZ()
{
	int		i;
	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[A-Z]");
	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	*(Linebuf+1) = '0';
	*(Linebuf+2) = '\0';
	for (i='A'; i <= 'Z'; i++)
	{
		*Linebuf = i;

		expecting (SUCCESS);
		advance(Linebuf, Expbuf);
		if ( loc1 != Linebuf )
		{
			error ("Advance did not match all single upper case \
characters");
		}
		if ( *loc2 != '0' )
		{
			error ("loc2 should point to the '0' char at the \
end of the string.");
		}
	}
}

/*
** r_AtoK()
**	compile an expression consisting of "[A-K]".  Check that executing
**	this will match only single upper case chars up to and including K.
**
** Level:
**	This test is in the Normal level (0).
*/
r_AtoK()
{
	int		i;

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[A-K]");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	*(Linebuf+1) = '0';
	*(Linebuf+2) = '\0';
	for (i='A'; i <= 'K'; i++)
	{
		*Linebuf = i;
		expecting (SUCCESS);
		step(Linebuf, Expbuf);
		if ( loc1 != Linebuf )
		{
			error ("step did not match all single upper case \n\
characters up to and including K");
		}
		if ( *loc2 != '0' )
		{
			error ("loc2 should point to the '0' char at the \
end of the string.");
		}
	}
}

/*
** r_0to9()
**	compile an expression consisting of "[0-9]".  Check that executing
**	this will match only single digits up to and including 9.
**
** Level:
**	This test is in the Normal level (0).
*/
r_0to9()
{
	int		i;

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[0-9]");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	*(Linebuf+1) = '0';
	*(Linebuf+2) = '\0';
	for (i='0'; i <= '9'; i++)
	{
		*Linebuf = i;
		expecting (SUCCESS);
		step(Linebuf, Expbuf);
		if ( loc1 != Linebuf )
		{
			error ("step did not match all single digits from \
0 to 9");
		}
		if ( *loc2 != '0' )
		{
			error ("loc2 should point to the '0' char at the \
end of the string.");
		}
	}
}

/*
** r_0to5()
**	compile an expression consisting of "[0-5]".  Check that executing
**	this will match only single digits up to and including 5.
**
** Level:
**	This test is in the Normal level (0).
*/
r_0to5()
{
	int		i;

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[0-5]");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	*(Linebuf+1) = '0';
	*(Linebuf+2) = '\0';
	for (i='0'; i <= '5'; i++)
	{
		*Linebuf = i;
		expecting (SUCCESS);
		step(Linebuf, Expbuf);
		if ( loc1 != Linebuf )
		{
			error ("step did not match all single digits \
from 0 to 5");
		}
		if ( *loc2 != '0' )
		{
			error ("loc2 should point to the '0' char at the \
end of the string.");
		}
	}
}

/*
** r2_AtoZ()
**	compile an expression consisting of "[^A-Z]".  Check that executing
**	this will match the range other than "A" through "Z".
**
** Level:
**	This test is in the Normal level (0).
*/
r2_AtoZ()
{
	int		i;

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[^A-Z]");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	Linebuf[1] = '0';
	Linebuf[2] = '\0';
	for (i = 1; i < MAXNUM2; i++)
	{
		expecting (VOID);
		sprintf(Linebuf, "%c", i);

		expecting (VOID);
		if (step(Linebuf, Expbuf) != 0)
		{
			if ( *Linebuf >= 'A' && *Linebuf <= 'Z' )
			{
				error ("step matched characters in the range \
'A' thru  'Z'", ERROR);
			}
			if ( *loc2 != '\0' )
			{
				error ("loc2 should point to the '0' char at \
the end of the string.");
			}
		}
		else
		{
			if ( *Linebuf >= 'A' && *Linebuf <= 'Z' )
				continue;
			else
			{
				error ("step did not match characters outside \
the range 'A' thru  'Z'");
			}
		}
	}
}

/*
** r_hata()
**	compile an expression consisting of "^a".  Check that executing
**	this will match only a's at the beginning of lines.
**
** Level:
**	This test is in the Normal level (0).
*/
r_hata()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "^a");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "ab");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'a' )
			error ("step matched characters other than 'a'\n\
at the beginning of the line 'ab'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "ba");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( loc1 != NULL )
			error ("step matched 'a' to the string 'ba'");
	}
}

/*
** r6_AtoZatoz()
**	compile an expression consisting of "[A-Za-z]+".  Check that
**	Executing this will match only one or more occurrences of upper
**	and lower case or spaces.
**
** Level:
**	This test is in the Normal level (0).
*/
r6_AtoZatoz()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[A-Za-z]+");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	Linebuf[1] = '\0';
	expecting (VOID);
	if (advance(Linebuf, Expbuf) != 0)
		error ("advance matched '[A-Za-z]+' to a NULL string");

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "0123");

	expecting (VOID);
	if ( advance(Linebuf, Expbuf) != 0 )
	{
		if ( loc1 != NULL )
			error ("advance matched '[A-Za-z]+' to '0123'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "a0123");
	if ( advance(Linebuf, Expbuf) != 0 )
	{
		if ( *loc2 != '0' )
			error ("loc2 should be a pointer to first \
unmatched character in 'a0123'.");
		if ( *loc1 != 'a' )
			error ("advance did not match the 'a' at the \
beginning of the line 'a0123' to '[A-Za-z]+'");
	}
}

/*
** r7_AtoZatoz()
**	compile an expression consisting of "[A-Z][a-z]$".  Check that
**	Executing this will match only a 2 character string at the end
**	of the line.
**
** Level:
**	This test is in the Normal level (0).
*/
r7_AtoZatoz()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[A-Z][a-z]$");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	Linebuf[0] = '\0';
	expecting (VOID);
	if ( advance(Linebuf, Expbuf) != 0 )
		error ("advance matched '[A-Z][a-z]$' to a NULL string");

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "1ab");

	expecting (VOID);
	if ( advance(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'a' )
			error ("advance did not match the 'a' of the line \n\
'1ab' to '[A-Z][a-z]$'");
	}
}

/*
** r3_AtoZ()
**	compile an expression consisting of "[A-Z]+".  Check that executing
**	this will not match lower case chars.
**
** Level:
**	This test is in the Normal level (0).
*/
r3_AtoZ()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "[A-Z]+");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "abcdefgh");

	expecting (VOID);
	if ( advance(Linebuf, Expbuf) != 0 )
	{
		error ("advance matched lower case characters to '[A-Z]+'");
	}
}

/*
** r_Azero()
**	compile an expression consisting of "A{0,}".  Check that 
**	this expression matches at least zero occurrences of A.
**
** Level:
**	This test is in the Boundary level (1).
*/
r_Azero()
{
	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "A\\{0,\\}");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != Linebuf[0] )
			error ("step did not match a null string to 'A{0,}'");
	}
}

/*
** r_Atwo()
**	compile an expression consisting of "A{2,}".  Check that
**	this matches at least 2 occurrences of the letter A.
**
** Level:
**	This test is in the Boundary level (1).
*/
r_Atwo()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "A\\{2,\\}");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
		error ("step matched 'A{2,}' to a NULL string");

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "A");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( loc1 != NULL )
			error ("step matched 'A{2,}' to 'A'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AA");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'A' )
			error ("step did not match the 'AA' at the\n\
beginning of the line to 'A{2,}'");
	}
}

/*
** r5_AtoZ()
**	compile an expression consisting of "^[A-Z]{4,}".  Check that
**	Executing this will match at least 4 upper case chars.
**
** Level:
**	This test is in the Robustness level (2).
*/
r5_AtoZ()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "^[A-Z]\\{4,\\}");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAA");

	expecting (VOID);
	if ( advance(Linebuf, Expbuf) != 0 )
		error ("advance matched '^[A-Z]{4,}' to 'AAA'");

	/*
	** Now try 4  caps
	*/
	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAAA");

	expecting (VOID);  /*  give more detailed messages */
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( loc1 != Linebuf )
			error ("advance did not match '^[A-Z]{4,}' to \
four or more caps");
	}
	else
		error ("advance failed when given the compiled\n\
regular expression '^[A-Z]{4,}'.");
}

/*
** r_Afour()
**	compile an expression consisting of "A{4,5}". Check that
**	Executing this will match only four or five occurrences of
**	the character "A".
**
** Level:
**	This test is in the Robustness level (2).
*/
r_Afour()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "A\\{4,5\\}");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAA");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
		error ("step matched 'A{4,5}' to 'AAA'");

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAAA");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'A' )
			error ("step should have matched 'A{4,5}' to 'AAAA'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAAAA");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'A' )
			error ("step should have matched 'A{4,5}' to 'AAAAA'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAAAAA");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) == 0 )
		error ("step should have matched 'A{4,5}' to 'AAAAAA'");

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "AAAAAAA");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) == 0 )
		error ("step should have matched 'A{4,5}' to 'AAAAAAA'");

}

/*
** r_atoz()
**	compile an expression consisting of "^[a-z]{4,8}". Check that
**	Executing this will match only four to eight lower case chars.
**
** Level:
**	This test is in the Robustness level (2).
*/
r_atoz()
{

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Instring, "^[a-z]\\{4,8\\}");

	expecting (SUCCESS);
	compile(Instring,Expbuf,&Expbuf[ESIZE],'\0');

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "aaa");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
		error ("step matched '^[a-z]{4,8}' to 'aaa'");

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "aaaa");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'a' )
			error ("step did not match '^[a-z]{4,8}' to 'aaaa'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "aaaaaaaa");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) != 0 )
	{
		if ( *loc1 != 'a' )
			error ("step did not match '^[a-z]{4,8}' to 'aaaaaaaa'");
	}

	expecting (VOID);  /*  strcpy has no invalid return  */
	strcpy(Linebuf, "aaaaaaaaa");

	expecting (VOID);
	if ( step(Linebuf, Expbuf) == 0 )
		error ("step should have matched '^[a-z]{4,8}' to 'aaaaaaaaa'");

}

regerr(){}

