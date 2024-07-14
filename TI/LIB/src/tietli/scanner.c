# include	"main.h"
# include	"yyparse.h"
# include	"yy.h"
# include	<ctype.h>

SCCSID("@(#)scanner.c	3.2 (SVVS) 12/30/86");

# define	BUFLEN	250

static	int	Length = 0;	/* Amount of data left */
static	char	Buffer[BUFLEN];	/* Internal buffer for characters */
static	char	*Bufptr;	/* pointer to the current character in the buffer */
char	yytext[BUFLEN];		/* Current identifier */

# define	UNDEF_STMT	-5
/*
** The table of keywords for the scanner. This does not need to
** be in order, as an explicit sort happens in scan_init.
*/
typedef	struct
{
	char	*key;
	int	data;
} KEYWORD;

KEYWORD Keywords[] =
{
	"TRUE",		(int) BOOL,
	"FALSE",	(int) BOOL,
	"while",	(int) WHILE_STMT,
	"for",		(int) FOR_STMT,
	"if",		(int) IF_STMT,
	"else",		(int) ELSE_STMT,
	"continue",	(int) CONTINUE_STMT,
	"break",	(int) BREAK_STMT,
	"exit",		(int) EXIT_STMT,
	"abstract",	(int) ABSTRACT_STMT,
	"testing",	(int) TESTING_STMT,
	"prep",		(int) PREP_STMT,
	"cleanup",	(int) CLEANUP_STMT,
	"start",	(int) START_STMT,
	"finis",	(int) FINIS_STMT,
	NULL,		0
};

/*
** scan_compare
**	Compare two KEYWORD pointers. This routine is used
**	by qsort, and bsearch.
**
** Arguments
**	a,b	The two pointers.
**
** Returns
**	< 0	if a < b
**	0	if a == b
**	> 0	if a > b
*/
static	int	
scan_compare(a,b)
KEYWORD	*a, *b;
{
	return (strcmp(a->key,b->key));
}

/*
** scan_init
**	Initilize the keywords for the scanner lookup.
**	This just does a qsort so we can use bsearch.
*/
void
scan_init()
{
	(void) qsort(Keywords,sizeof (Keywords) / sizeof (KEYWORD), sizeof (KEYWORD), scan_compare);
}
/*
** scan_getc
**	The routine yylex is used to read a character from the input.
**
** Return value
**	The character read in
**	EOF on End of file.
*/
static	int	
scan_getc()
{
	int	ch;

	if ( Length <= 0 )
	{
		Bufptr = Buffer;
		while ( (ch = getchar()) != EOF && (Length < BUFLEN) )
		{
			Length++;
			if ( (*Bufptr++ = ch) == '\n' )
			{
				LineNumber++;
				SideType = SIDE_NULL;
				break;
			}
		}
		Bufptr = Buffer;
	}
	if ( Length <= 0 )
		return (EOF);

	Length--;
	return (*Bufptr++);
}

/*
** scan_ungetc
**	Put a character BACK into the buffer.
**
** Arguments
**	ch	The character to put back into the buffer.
*/
static	void
scan_ungetc(ch)
int	ch;
{
	if ( ch != EOF )
	{
		Length++;
		Bufptr--;
		*Bufptr = ch;
	}
}


/*
** yylex
**	Lexical analizer for tietl. Does simple binary operators,
**	C type operators, a couple of constants, keywords, compiler
**	switches, and not much else.
**
** Return Value
**	Lexical value of the operator.
*/
int
yylex()
{
	int	ch;
	char	*sp;
	KEYWORD	entry, *entryptr;
	extern	char	*malloc();

	sp = yytext;

	if ( (ch = scan_getc()) == EOF )
		return ( 0 );

	/*
	** Look for constants.
	*/
	if ( isdigit(ch) )
	{
		*sp++ = ch;
		while ( (ch = scan_getc()) != EOF )
		{
			if ( isdigit(ch) )
				*sp++ = ch;
			else
			{
				scan_ungetc(ch);
				break;
			}
		}
		*sp = '\0';
		yylval.const_lexval = sav_const(yytext, DATA_NUM);
		return (NUMERIC);
	}
	/*
	** Look for identifiers.
	*/
	if ( isalpha(ch) || ch == '_' )
	{
		*sp++ = ch;
		while ( (ch = scan_getc()) != EOF )
		{
			if ( isalnum(ch) || ch == '_' )
				*sp++ = ch;
			else
			{
				scan_ungetc(ch);
				break;
			}
		}
		*sp = '\0';
		entry.key = yytext;
		if ( (entryptr = (KEYWORD *)bsearch(&entry,Keywords,sizeof (Keywords) / sizeof (KEYWORD), sizeof (KEYWORD), scan_compare)) != (KEYWORD *) NULL )
		{
			if ( entryptr->data == UNDEF_STMT )
				return ( yylex() );
			if ( entryptr->data == BOOL )
				yylval.const_lexval = sav_const(yytext, DATA_BOOL);
			return(entryptr->data);
		}

		yylval.var_lexval = malloc(strlen(yytext) + 1);
		strcpy(yylval.var_lexval, yytext);
		return(IDENTIFIER);
	}
	*sp = ch;
	/*
	** Not an identifier, or a number.
	*/
	switch ( ch )
	{
	  /*
	  ** For unknown characters, just send them to the parser.
	  */
	  default:
		yytext[0] = ch;
		yytext[1] = '\0';
		yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		return ((int) yylval.op_lexval);

	  /*
	  ** White space, just ignore.
	  */
	  case ' ':
	  case '\n':
	  case '\t':
		while ( (ch = scan_getc()) != EOF && isspace(ch) )
			continue;
		if ( ch != EOF )
			scan_ungetc(ch);
		return (yylex());

	  /*
	  ** either +, ++, or +=
	  */
	  case '+':
		ch = scan_getc();
		if ( ch == '+' )
			yylval.op_lexval = INCR;
		else
			if ( ch == '=' )
				yylval.op_lexval = ADDASSIGN;
			else
			{
				scan_ungetc(ch);
				yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
			}
		return ((int) yylval.op_lexval);
			
	  /*
	  ** either -, --, or -=
	  */
	  case '-':
		ch = scan_getc();
		if ( ch == '-' )
			yylval.op_lexval = DECR;
		else
			if ( ch == '=' )
				yylval.op_lexval = SUBASSIGN;
			else
			{
				scan_ungetc(ch);
				yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
			}
		return ((int) yylval.op_lexval);

	  /*
	  ** Either *, or *=.
	  */
	  case '*':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = MULTASSIGN;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either /, or /= /*
	  */
	  case '/':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = DIVASSIGN;
		else
		{
			/*
			** C style comments
			*/
			if ( ch == '*' )
			{
				while ( (ch = scan_getc()) != EOF )
				{
					if ( ch == '*' )
					{
						if ((ch = scan_getc()) == '/' )
							break;
						scan_ungetc(ch);
					}
				}
				if ( ch == EOF )
					screech("scanner",0,"Unexpected EOF inside of a 'C' style comment");
				return ( yylex() );
			}
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either %, or %=
	  */
	  case '%':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = MODASSIGN;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either =, or ==.
	  */
	  case '=':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = EQ;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either > or >=
	  */
	  case '>':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = GE;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either < or <=
	  */
	  case '<':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = LE;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either !, or !=
	  */
	  case '!':
		ch = scan_getc();
		if ( ch == '=' )
			yylval.op_lexval = NE;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either |, or ||
	  */
	  case '|':
		ch = scan_getc();
		if ( ch == '|' )
			yylval.op_lexval = OR;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** either &, or &&
	  */
	  case '&':
		ch = scan_getc();
		if ( ch == '&' )
			yylval.op_lexval = AND;
		else
		{
			scan_ungetc(ch);
			yylval.op_lexval = (OPERATOR_TYPE) yytext[0];
		}
		return ((int) yylval.op_lexval);

	  /*
	  ** literal string ("xxxxx")
	  */
	  case '"':
		sp++;
		while ( (ch = scan_getc()) != EOF )
		{
			/*
			** check for \" embedded in a string
			*/
			if ( ch == '"' && *(sp - 1) != '\\' )
				break;
			*sp++ = ch;
		}
		*sp++ = '"';
		*sp = '\0';
		yylval.const_lexval = sav_const(yytext,DATA_TEXT);
		return (TEXT);

	  /*
	  ** Character constant ('x', '\t', '\044')
	  */
	  case '\'':
		sp = yytext;
		*sp++ = '\'';
		ch = scan_getc();
		if ( ch != '\\' )
		{
			*sp++ = ch;
		}
		else
		{
			*sp++ = ch;
			ch = scan_getc();
			switch (ch)
			{
			  case EOF:
				screech("yylex",0,"Line %d: Unexpected EOF in a character constant",LineNumber);
				break;

			  case '\n':
				screech("yylex",0,"Line %d: Unexpected \\n in a character constant",LineNumber);
				break;

			  default:
				*sp++ = ch;
				break;

			  case '0': case '1': case '2':
			  case '3': case '4': case '5':
			  case '6': case '7': case '8':
			  case '9':
				*sp++ = ch;
				while ( (ch = scan_getc()) != EOF && ch != '\'' )
				{
					if ( !isdigit(ch) )
						*sp++ = ch;
					else
						break;
				}
				if ( ch != EOF )
					scan_ungetc(ch);
				*sp = '\0';
				break;
			}
		}
		*sp++ = '\'';
		*sp = '\0';
		yylval.const_lexval = sav_const(yytext,DATA_CHAR);
		if ( (ch = scan_getc()) != '\'' )
			screech("yylex",0,"Line %d: Unterminated character constant",LineNumber);
		return (CHAR);

	  /*
	  ** A comment, or a compiler switch.
	  */
	  case '#':
		sp = yytext;
		*sp++ = ch;

		ch = scan_getc();
		switch (ch)
		{
		  case '<':
			SideType = SIDE_PITCHER;
			break;
		  case '>':
			SideType = SIDE_CATCHER;
			break;
		  /*
		  ** Look for possible compiler switches.
		  */
		  default:
		  case '\n':
		  case ' ':
		  case '\t':
			if ( ch == EOF || ch == '\n' )
				SideType = SIDE_NULL;
			else
			{
				do
				{
					*sp++ = ch;
					ch = scan_getc();
				} while ( (ch != EOF) && (ch != '\n') );

				*sp = '\0';

				compile_switch(yytext);
			}
			/*
			** Return the next token.
			*/
		}
		return (yylex());
	}
}
