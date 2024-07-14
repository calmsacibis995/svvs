# include	<bless.h>
# include	<common.h>
# include	<search.h>
# include	<ctype.h>

SCCSID("@(#)bless.c	3.1 (SVID) 12/1/86");


/*
** bless
**	External interface to the reverse curses system.
**	This routine will do the "appropriate" thing for
**	translating a series of escape sequences into a
**	representation of a screen.
**
**	The basic idea is to look for a terminal escape sequence,
**	if it is found then call the function with the approprite
**	arguments (found by apply()). If there is no escape sequence,
**	then just add the first character in 'string' to the display,
**	and start all over again at the second character.
**
** Arguments
**	term	The terminal structure to update
**	string	A string containing an escape sequence.
**	length	The length of the string, if <= 0 do a
**		strlen(string) to get the length. This
**		is here as NULL bytes might be in the string.
*/
bless(term,string,length)
TERMINAL	*term;
char		*string;
int		length;
{
	COMM		*comm;		/* command from find_command */
	ARGS		*arg;		/* argument list */
	ARGS		argument;	/* for default arguments */
	int		len;		/* length of substrings */
	char		*str;		/* previous string location */
	int		(*func)();	/* function from comm->comm_func */
	extern	ARGS	*apply();
	extern	COMM	*find_command();
# ifdef	TRACE
	char		strbuf[21];
# endif	/* TRACE */


	if ( length <= 0 )
		length = strlen(string);

# ifdef	TRACE
	if ( Trace(20,0) )
	{
		(void) strncpy(strbuf,string,20);
		strbuf[20] = '\0';
		print("bless: (%s,%d)\n",strbuf,length);
	}
# endif	/* TRACE */

	argument.arg_type = ARG_CHAR;

	while ( length > 0 )
	{
		if ( string == NULL )
			break;
		stack_init();
		/*
		** Quick optimization, only look for a command
		** if there is an escape sequence at the front.
		*/
		if ( isascii(*string) && isprint(*string) )
			comm = (COMM *) NULL;
		else
			comm = find_command(string,length);
		/*
		** If there is no command possible with
		** this initial set of characters, just put
		** the character on the screen.
		*/
# ifdef	TRACE
		if ( Trace(20,1) )
			printf("(x,y) = (%d,%d) ",term->term_xloc,term->term_yloc);
# endif	/* TRACE */
		if ( comm == (COMM *) NULL )
		{
			argument.arg_char = *string;
			arg = &argument;
			func = direct;
# ifdef	TRACE
			if ( Trace(20,0) )
				printf("bless: func direct '%c'\n",*string);
# endif	/* TRACE */
			string++;
			length--;
		}
		else
		{
			if ( comm->comm_string == NULL )
				len = 0;
			else
				len = strlen(comm->comm_string);
			length -= len;
			string += len;
			str = string;
			arg = apply(comm,&string);
			length -= abs(str - string);
			func = comm->comm_func;
# ifdef	TRACE
			if ( Trace(20,0) )
			{
				printf("bless: func %s ",comm->comm_name);
				printarg(arg);
				printf("\n");
			}
# endif	/* TRACE */
		}

		/*
		** call the function.
		*/
		(void) func(term,arg);
	}
	return;
}
