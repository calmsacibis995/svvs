#include	<sv_base.h> /*replacement macros */
#include	<stdio.h>
#include	<ctype.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)ctype.c	1.1"



# define	MAXASCII	0177		/* Maximum ascii value */
# define	FIRST_PRINT	040		/* First printable character */
# define	FIRST_GRAPH	041		/* First graphic character */
# define	LAST_PRINT	0176		/* Last printable character */
# define	DELETE		0177		/* Delete character */

char	Spacechar[] = " \t\f\n\r\013";		/* Space characters */
char	Punctchar[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
char	String[STRLEN];				/* A handy string for printing error messages in */

/*
** ctype
**	Check that the ctype functions/macros work correctly.
*/
main()
{
setup("ctype");

	t_isalpha();
	t_isupper();
	t_islower();
	t_isdigit();
	t_isalnum();
	t_isspace();
	t_ispunct();
	t_isprint();
	t_isprint();
	t_isgraph();
	t_iscntrl();
	t_isascii();
	t_isxdigit();

done();
/*NOTREACHED*/

}

/*
** t_isalpha
**	Test the isalpha macro. Note that this is not called test_isalpha
**	because we are trying to keep a 6 character unique name.
** Level
**	This test covers all 3 levels.
*/
t_isalpha()
{
	int	character;

funct ("isalpha");
start

testing("isalpha(all letters) should return values of non-zero.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 'a' ; character <= 'z' ; character++ )
	{
		if ( isalpha(character) == 0 )
		{
			error ("isalpha returned 0 on the letter '%c'",
				character);
		}
	}
	for ( character = 'A' ; character <= 'Z' ; character++ )
	{
		if ( isalpha(character) == 0 )
		{
			error ("isalpha returned 0 on the letter '%c'",
				character);
		}
	}
finis


testing("EOF should return zero.");
start
	expecting (VOID);

	if ( isalpha(EOF) )
	{
		error("isalpha returned non-zero with an argument of EOF");
	}
finis


testing("isalpha(all possible ascii values) should return 0");
testing("for everything except letters.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isalpha(character) )
		{
			if ( character < 'a' || character > 'z' )
			{
				if ( character < 'A' || character > 'Z' )
				{
					error("isalpha returns non-zero\
 on the nonletter whose octal value = %o",
						character);
				}
			}
		}
		else
		{
			if ( character >= 'a' && character <= 'z' )
			{
				error ("isalpha returned zero on the letter %c",
					character);
			}
			if ( character >= 'A' && character <= 'Z' )
			{
				error ("isalpha returned zero on the letter %c",
					character);
			}
		}
	}
	finis
finis


}
/*
** t_isupper
**	Test the isupper macro.
** Level
**	This test covers all 3 levels.
*/
t_isupper()
{
	int	character;

funct ("isupper");
start

testing("isupper(all upper case) should return values of non-zero.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 'A' ; character <= 'Z' ; character++ )
	{
		if ( isupper(character) == 0 )
		{
			error("isupper returned 0 on the letter '%c'",
				character);
		}
	}
finis


testing("isupper (EOF) should return zero");
start
	expecting (VOID);

	if ( isupper(EOF) )
	{
		error("isupper returned non-zero with an argument of EOF");
	}
finis



testing("isupper(all possible ascii values) should");
testing("return zero for everything except uppercase letters.");
start
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isupper(character) )
		{
			if ( character < 'A' || character > 'Z' )
			{
				error ("isupper returns non-zero on the\
 nonletter whose octal value = %o",
					character);
			}
		}
		else
		{
			if ( character >= 'A' && character <= 'Z' )
			{
				error ("isupper returned zero on the letter %c",
					character);
			}
		}
	}
finis
finis


}
/*
** t_islower
**	Test the islower macro.
** Level
**	This test covers all 3 levels.
*/
t_islower()
{
	int	character;

funct ("islower");
start

testing("islower(all lower case) should return non-zero values.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 'a' ; character <= 'z' ; character++ )
	{

		if ( islower(character) == 0 )
		{
			error("islower returned 0 on the letter '%c'",
				character);
		}
	}
finis


testing("islower (EOF) should return zero");
start
	expecting (VOID);

	if ( islower(EOF) )
	{
		error("islower returned non-zero with an argument of EOF");
	}
finis


testing("islower(all possible ascii values) should");
testing("return zero for everything except lowercase letters.");
start
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( islower(character) )
		{
			if ( character < 'a' || character > 'z' )
			{
				error ("islower returned non-zero \
on the nonletter whose octal value = %o",
					character);
			}
		}
		else
		{
			if ( character >= 'a' && character <= 'z' )
			{
				error ("islower returned zero on the letter %c",
					character);
			}
		}
	}
finis

finis


}

/*
** t_isdigit
**	Test the isdigit macro.
** Level
**	This test covers all 3 levels.
*/
t_isdigit()
{
	int	character;

funct ("isdigit");
start

testing("isdigit(all digits) should return values of non-zero.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = '0' ; character <= '9' ; character++ )
	{
		if ( isdigit(character) == 0 )
		{
			error("isdigit returned 0 on the digit", character);
		}
	}
finis

testing("isdigit (EOF) should return zero");
start
	break_on (ERROR);
	expecting (VOID);


	if ( isdigit(EOF) )
	{
		error("isdigit returned non-zero with an argument of EOF");
	}
finis


testing("isdigit(all possible ascii values) should");
testing("return zero for everything except digits.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isdigit(character) )
		{
			if ( character < '0' || character > '9' )
			{
				error ("isdigit returns non-zero on the \
nondigit whose octal value = %o",character);
			}
		}
		else
		{
			if ( character >= '0' && character <= '9' )
			{
				error ("isdigit returned zero on the \
digit %c",character);
			}
		}
	}
finis

finis

}

/*
** t_isxdigit
**	Test the isxdigit macro.
** Level
**	This test covers all 3 levels.
*/
t_isxdigit()
{
	int	character;

funct ("isxdigit");
start

testing("isxdigit(all hex digits) should return values of non-zero.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = '0' ; character <= '9' ; character++ )
	{
		if ( isxdigit(character) == 0 )
		{
			error("isxdigit returned 0 on the digit '%c'",
				character);
		}
	}
	for ( character = 'A' ; character <= 'F' ; character++ )
	{
		if ( isxdigit(character) == 0 )
		{
			error ("isxdigit returned 0 on '%c'",character);
		}
	}
	for ( character = 'a' ; character <= 'f' ; character++ )
	{
		if ( isxdigit(character) == 0 )
		{
			error ("isxdigit returned 0 on '%c'",character);
		}
	}
finis

testing("isxdigit (EOF) should return zero");
start
	expecting (VOID);

	if ( isxdigit(EOF) )
	{
		error("isxdigit returned non-zero with an argument of EOF");
	}
finis


testing("isxdigit(all possible ascii values) should");
testing("return zero for everything except hexadecimal digits.");
start
	break_on (ERROR);
	expecting (VOID);


	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isxdigit(character) )
		{
			if ( character < '0' || character > '9' )
			{
				if ( character < 'a' || character > 'f' )
				{
					if ( character < 'A' || character > 'F' )
					{
						error ("isxdigit returns \
non-zero on the nondigit whose octal value = %o",character);
					}
				}
			}
		}
		else
		{
			if ( character >= '0' && character <= '9' )
			{
				error ("isxdigit returned zero on the digit %c",
					character);
			}
			if ( character >= 'A' && character <= 'F' )
			{
				error ("isxdigit returned zero on the digit %c",
					character);
			}
			if ( character >= 'a' && character <= 'f' )
			{
				error ("isxdigit returned zero on the digit %c",
					character);
			}
		}
	}
finis

finis

}

/*
** t_isalnum
**	Test the isalnum macro.
** Level
**	This test covers all 3 levels.
*/
t_isalnum()
{
	int	character;

funct ("isalnum");
start

testing("isalnum(all letters and digits) should return values of non-zero.");
start
	break_on (ERROR);
	expecting (VOID);


	for ( character = '0' ; character <= '9' ; character++ )
	{
		if ( isalnum(character) == 0 )
		{
			error("isalnum returned 0 on a digit");
		}
	}
	for ( character = 'A' ; character <= 'Z' ; character++ )
	{
		if ( isalnum(character) == 0 )
		{
			error ("isalnum returned 0 on '%c'",character);
		}
	}
	for ( character = 'a' ; character <= 'z' ; character++ )
	{
		if ( isalnum(character) == 0 )
		{
			error ("isalnum returned 0 on '%c'",character);
		}
	}
finis


testing("isalnum (EOF) should return zero");
start
	expecting (VOID);

	if ( isalnum(EOF) )
	{
		error ("isalnum returned non-zero with an argument of EOF");
	}
finis


testing("isalnum(all possible ascii values) should");
testing("return zero for everything except hexadecimal digits.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isalnum(character) )
		{
			if ( character < '0' || character > '9' )
			{
				if ( character < 'a' || character > 'z' )
				{
					if ( character < 'A' || character > 'Z' )
					{
						error ("isalnum returns \
non-zero on the character whose octal value = %o",character);
					}
				}
			}
		}
		else
		{
			if ( character >= '0' && character <= '9' )
			{
				error ("isalnum returned zero on the digit %c",
					character);
			}
			if ( character >= 'A' && character <= 'Z' )
			{
				error ("isalnum returned zero on the letter %c",
					character);
			}
			if ( character >= 'a' && character <= 'z' )
			{
				error ("isalnum returned zero on the letter %c",
					character);
			}
		}
	}
finis

finis

}

/*
** t_isspace
**	Test the isspace macro.
** Level
**	This test covers all 3 levels.
*/
t_isspace()
{
	int	character;
	char	*sp;

funct ("isspace");
start

testing("isspace(all whitespace) should return non-zero.");
start
	break_on (ERROR);
	expecting (VOID);


	for ( sp = Spacechar ; *sp != '\0' ; sp++ )
	{
		if ( isspace((int) (*sp)) == 0 )
		{
			error ("isspace returned 0 on white space character %o",
				*sp);
		}
	}
finis


testing("isspace (EOF) should return zero");
start
	expecting (VOID);

	if ( isspace(EOF) )
	{
		error("isspace returned non-zero with an argument of EOF");
	}
finis


testing("isspace(all possible ascii values) should return zero");
testing("for everything except white space characters.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isspace(character) )
		{
			for ( sp = Spacechar ; *sp != '\0' ; sp++ )
				if ( *sp == character )
					break;
			if ( *sp == '\0' )
			{
				error ("isspace returns non-zero on \
the non white space character whose octal value = %o",character);
			}
		}
		else
		{
			for ( sp = Spacechar ; *sp != '\0' ; sp++ )
				if ( *sp == character )
					break;
			if ( *sp != '\0' )
			{
				error ("isspace returned zero on \
the white space character '%o'",character);
			}
		}
	}
finis

finis

}

/*
** t_ispunct
**	Test the ispunct macro.
** Level
**	This test covers all 3 levels.
*/
t_ispunct()
{
	int	character;
	char	*sp;

funct ("ispunct");
start

testing("ispunct(all punctuation) should return non-zero.");
start
	expecting (VOID);

	for ( sp = Punctchar ; *sp != '\0' ; sp++ )
	{
		if ( ispunct((int) (*sp)) == 0 )
		{
			error ("ispunct returned 0 on punctuation character %o",
				*sp);
		}
	}
finis


testing("ispunct (EOF) should return zero");
start
	expecting (VOID);

	if ( ispunct(EOF) )
	{
		error("ispunct returned non-zero with an argument of EOF");
	}
finis


testing("ispunct(all possible ascii values) should return zero");
testing("for everything except punctuation characters.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( ispunct(character) )
		{
			for ( sp = Punctchar ; *sp != '\0' ; sp++ )
				if ( *sp == character )
					break;
			if ( *sp == '\0' )
			{
				error ("ispunct returns non-zero on the \
non punctuation character whose octal value = %o",character);
			}
		}
		else
		{
			for ( sp = Punctchar ; *sp != '\0' ; sp++ )
				if ( *sp == character )
					break;
			if ( *sp != '\0' )
			{
				error ("ispunct returned zero on the \
punctuation character '%o'",character);
			}
		}
	}
finis

finis
}

/*
** t_isprint
**	Test the isprint macro.
** Level
**	This test covers all 3 levels.
*/
t_isprint()
{
	int	character;

funct ("isprint");
start

testing("isprint(all printable characters) should return non-zero");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = FIRST_PRINT ; character <= LAST_PRINT ; character++ )
	{
		if ( isprint(character) == 0 )
		{
			error("isprint returned 0 for '%c'",character);
		}
	}
finis


testing("isprint (EOF) should return zero");
start
	expecting (VOID);

	if ( isprint(EOF) )
	{
		error("isprint returned non-zero with an argument of EOF");
	}
finis


testing("isprint(all possible ascii values) should return 1");
testing("for all printable characters.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isprint(character) )
		{
			if ( character < FIRST_PRINT || character > LAST_PRINT )
			{
				error ("isprint returns non-zero on the \
non printable character whose octal value = %o",character);
			}
		}
		else
		{
			if ( character >= FIRST_PRINT && character <= LAST_PRINT )
			{
				error ("isprint returned zero on the \
printable character %c",character);
			}
		}
	}
finis

finis
}

/*
** t_isgraph
**	Test the isgraph macro.
** Level
**	This test covers all 3 levels.
*/
t_isgraph()
{
	int	character;

funct (" isgraph");
start


testing("isgraph(all printables except space) should return non-zero");
start
	break_on (ERROR);
	expecting (VOID);


	for ( character = FIRST_GRAPH ; character <= LAST_PRINT ; character++ )
	{
		if ( isgraph(character) == 0 )
		{
			error("isgraph returned 0 on the printable '%c'",
				character);
		}
	}
finis


testing("isgraph (EOF) should return zero");
start
	expecting (VOID);

	if ( isgraph(EOF) )
	{
		error("isgraph returned non-zero with an argument of EOF");
	}
finis


testing("isgraph (all possible ascii values) should return 1");
testing("for all printable characters.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isgraph(character) )
		{
			if ( character < FIRST_GRAPH || character > LAST_PRINT )
			{
				error ("isgraph returns non-zero \
on the non printable character whose octal value = %o",character);
			}
		}
		else
		{
			if ( character >= FIRST_GRAPH && character <= LAST_PRINT )
			{
				error ("isgraph returned zero \
on the printable character %c",character);
			}
		}
	}
finis

finis
}

/*
** t_iscntrl
**	Test the iscntrl macro.
** Level
**	This test covers all 3 levels.
*/
t_iscntrl()
{
	int	character;

funct ("iscntrl");
start

testing("iscntrl(all control characters) should return non-zero");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = (FIRST_PRINT -1) ; character >= 0 ; character-- )
	{
		if ( iscntrl(character) == 0 )
		{
			error ("iscntrl returned 0 on control '%d'",character);
		}
	}
	if ( iscntrl(DELETE) == 0 )
	{
		error ("iscntrl returned 0 on a DELETE");
	}
finis


testing("iscntrl (EOF) should return zero");
start
	expecting (VOID);

	if ( iscntrl(EOF) )
	{
		error("iscntrl returned non-zero with an argument of EOF");
	}
finis


testing("iscntrl(all possible ascii values) should return 1");
testing("for all control characters.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( iscntrl(character) )
		{
			if ( character >= FIRST_PRINT && character != DELETE )
			{
				error ("iscntrl returns non-zero on the \
non control character whose octal value = %o",character);
			}
		}
		else
		{
			if ( character < FIRST_PRINT || character == DELETE )
			{
				error ("iscntrl returned zero on the \
control character %o",character);
			}
		}
	}
finis

finis
}

/*
** t_isascii
**	Test the isascii macro.
** Level
**	This test covers all 3 levels.
*/
t_isascii()
{
	int	character;

funct ("isascii");
start

testing("isascii(all characters) should return non-zero.");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = 0 ; character <= MAXASCII ; character++ )
	{
		if ( isascii(character) == 0 )
		{
			error("isascii returned 0 for character '%d'",
				character);
		}
	}
finis


testing("isascii (EOF) should return zero");
start
	expecting (VOID);

	if ( isascii(EOF) )
	{
		error("isascii returned non-zero with an argument of EOF");
	}
finis


testing("isascii(all possible ascii values) should return 1");
testing("for all ascii characters");
start
	break_on (ERROR);
	expecting (VOID);

	for ( character = SHRT_MIN ; character <= SHRT_MAX ; character++ )
	{
		if ( isascii(character) )
		{
			if ( (character < 0 || character > DELETE) && character != EOF )
			{
				error ("isascii returns non-zero on the \
non ascii character whose octal value = %o",character);
			}
		}
		else
		{
			if ( (character >= 0 && character <= DELETE) || character == DELETE )
			{
				error ("isascii returned zero on the \
ascii character %o",character);
			}
		}
		if (character == SHRT_MAX)
			break;
	}
finis

finis
}
