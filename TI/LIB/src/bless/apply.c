# include	<bless.h>
# include	<common.h>
# include	<stack.h>
# include	<memory.h>
# include	<search.h>
# include	<ctype.h>

SCCSID("@(#)apply.c	3.2 (SVID) 12/30/86");

# define	HOLDNUM		35

static	ARGS	*Current;
int	Arg_status;
ARGS	Hold[HOLDNUM];

/*
** apply
**	Given a string from the terminal, and a parameritized
**	string, match as much of the string as makes sense.
**
** Arguments
**	comm	The current command.
**	data	The string from the terminal.
**
** Return Value
**	Whatever arguments were originally given to the function.
*/
ARGS	*
apply(comm,data)
COMM	*comm;
char	**data;
{
	char	*str;
	char	*param;
	extern	char	*doper();
	extern	ARGS	*copy_args();


	arg_init();
	Current = Hold;
	str = *data;
	param = comm->comm_data;
# ifdef	TRACE
	if ( Trace(10,0) )
	{
		char	prbuf[10];
		strncpy(prbuf,str,9);
		prbuf[9] = 0;
		printf("apply:("); printf("%s",param); printf(","); show(prbuf); printf(")\n");
	}
# endif	/* TRACE */

	if ( str == NULL || *str == '\0' || *comm->comm_data == '\0' )
		return ( (ARGS *) NULL );

	while ( *param )
	{
# ifdef	TRACE
		if ( Trace(10,5) )
			print("apply: param = '%c'\n",*param);
# endif	/* TRACE */
		if ( *param == '%' )
		{
			if ( (str = doper(&param,str)) == NULL )
			{
# ifdef	TRACE
				if ( Trace(10,0) )
					print("apply: doper returned NULL\n");
# endif	/* TRACE */
				return (NULL);
			}
		}
		else
		{
			if ( *param != *str )
			{
# ifdef	TRACE
				if ( Trace(10,0) )
					print("apply: %c (%o) != %c (%o)\n",*param,*param,*str,*str);
# endif	/* TRACE */
				return ( (ARGS *) NULL );
			}
			param++;
			str++;
			stack_init();
		}
	}
	*data = str;
	if ( Arg_status )
	{
		Hold[0].arg_int--;
		Hold[1].arg_int--;
		Arg_status = 0;
	}
	return (copy_args(Hold));
}
/*
** doper
**	Translate a '%' string, and apply it to a string
**	from a terminal.
**
** Arguments
**	param	The parameritized '%' string
**	data	Data string from the terminal
**
** Return value
**	Index into the 'data' string, after application of param string.
**	NULL	on any error.
**
** Side Effects
**	The param variable is a double pointer, so it can be advanced
**	past the end of the current '%' command. This means it is changed
**	in the calling environment.
*/
char	*
doper(param,data)
char	**param;
char	*data;
{
	char	*str;
	char	*sp;
	char	ch;
	char	tmp[100];
	int	i,j;

	str = *param;
# ifdef	TRACE
	if ( Trace(10,0) )
	{
		strncpy(tmp,data,9);
		tmp[9] = 0;
		printf("doper: (str = %9s, data = ",str);
		show(tmp);
		printf(")\n");
	}
# endif	/* TRACE */

	/*
	** Check for legal values (sanity).
	*/
	if ( *str++ != '%' )
	{
# ifdef	TRACE
		if ( Trace(10,0) )
			printf("*str == '%c', not %%\n",str[-1]);
# endif	/* TRACE */
		return ((char *) NULL);
	}

	switch( *str )
	{
	  /*
	  ** '%%' this means a single '%'
	  */
	  case '%':
		if ( *data != '%' )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("*str == '%c', not %%\n",*data);
# endif	/* TRACE */
			return (NULL);
		}
		return (&data[1]);

	  /*
	  ** '%p' Push a parameter.
	  */
	  case 'p':
		str++;
		if ( *str < '1' || *str > '9' )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("p%c not between 0 & 9\n",*str);
# endif	/* TRACE */
			return (NULL);
		}
		i = *str - '0';
		push(STK_PARAM,i);
		Current = &Hold[i - 1];
		break;

	  /*
	  ** %{nn} Push an integer constant.
	  */
	  case '{':
		str++;
		i = 0;
		while ( *str && isdigit(*str) )
		{
			i *= 10;
			i += *str - '0';
			str++;
		}
# ifdef	TRACE
		if ( Trace(10,2) )
			print("doper: pushed int %d",i);
# endif	/* TRACE */
		push(STK_CONST,i);
		break;


	  /*
	  ** '%'x'' Push a character constant.
	  */
	  case '\'':
		str++;
		i = *str++;
		/*
		** Handle \X conventions.
		*/
		if ( i == '\\' )
		{
			i = *str++;
			/*
			** A very unique situation. When I look at 
			** the strings themselves, the \X characters
			** have already been resolved. However, the
			** SVID does not guarantee this, so I must
			** check for explicit \X characters. However,
			** I must also handle \X NOT being the convention,
			** which explains the unusual code in case '\''.
			*/
			switch ( i )
			{
			  /*
			  ** A single '.
			  */
			  case '\'':
				if ( *str == '\'' )
					i = '\'';
				else
				{
					i = '\\';
					str--;
				}
				break;

			  /*
			  ** A space.
			  */
			  case 's':
				i = ' ';
				break;

			  /*
			  ** Newline
			  */
			  case 'n':
			  case 'l':
				i = '\n';
				break;

			  /*
			  ** Tab
			  */
			  case 't':
				i = '\t';
				break;

			  /*
			  ** Backspace.
			  */
			  case 'b':
				i = '\b';
				break;

			  /*
			  ** Formfeeds
			  */
			  case 'f':
				i = '\f';
				break;

			  /*
			  ** Backslash (\)
			  */
			  case '\\':
				i = '\\';
				break;
			}

		}
# ifdef	TRACE
		if ( Trace(10,2) )
			print("doper: pushed character %c",i);
# endif	/* TRACE */
		push(STK_CONST,i);
		break;

	  /*
	  ** '%i' Add one to the first two parameters
	  */
	  case 'i':
		Arg_status = 1;
# ifdef	TRACE
		if ( Trace(10,2) )
			print("doper: %i seen\n");
# endif	/* TRACE */
		break;

	  /*
	  ** '%+' Add the top two elements.
	  */
	  case '+':
# ifdef	TRACE
		if ( Trace(10,2) )
			print("doper: pushed addition\n");
# endif	/* TRACE */
		push(STK_ADD);
		break;

	  /*
	  ** '%-' Add the top two elements.
	  */
	  case '-':
# ifdef	TRACE
		if ( Trace(10,2) )
			print("doper: pushed subtraction\n");
# endif	/* TRACE */
		push(STK_SUB);
		break;

	  /*
	  ** '%*' Add the top two elements.
	  */
	  case '*':
# ifdef	TRACE
		if ( Trace(10,2) )
			print("doper: pushed mult\n");
# endif	/* TRACE */
		push(STK_MUL);
		break;

	  /*
	  ** '%c' Print as a character
	  */
	  case 'c':
		if ( Current == NULL )
		{
# ifdef	TRACE
			if ( Trace(10,2) )
				print("doper: Tried to print stack top, with no top\n");
# endif	/* TRACE */
			return (NULL);
		}
		Current->arg_type = ARG_CHAR;
		if ( b_eval(*data, &Current->arg_int) == ERR )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("b_eval returned ERR on %%c\n");
# endif	/* TRACE */
			return (NULL);
		}
		stack_init();
		data++;
		break;

	  /*
	  ** '%0n' Zero pad.
	  */
	  case '0':
		str++;
		if ( *str < '1' || *str > '9' )
		{
# ifdef	TRACE
		if ( Trace(10,0) )
			printf("%%c: %c not in 0->9\n",*str);
# endif	/* TRACE */
			return (NULL);
		}

	  /*
	  ** '%n' Print as a decimal number at least 'n' characters wide.
	  */
	  case '1': case '2': case '3':
	  case '4': case '5': case '6':
	  case '7': case '8': case '9':
		i = *str - '0';
		str++;
		if ( *str != 'd' )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				print("Unknown command %%%d%c\n",i,*str);
# endif	/* TRACE */
			return (NULL);
		}
		for ( j = 0 ; j < i ; j++ )
		{
			if ( !isdigit(*data) && *data != ' ' )
			{
# ifdef	TRACE
				if ( Trace(10,0) )
					print("%%n, *data == %c, not a digit\n",*data);
# endif	/* TRACE */
				return (NULL);
			}
			tmp[j] = *data++;
		}
		tmp[j] = '\0';
		if ( Current == NULL )
		{
# ifdef	TRACE
		if ( Trace(10,0) )
			print("%%n, no current\n");
# endif	/* TRACE */
			return (NULL);
		}
		Current->arg_type = ARG_INT;
		if ( b_eval(atoi(tmp), &Current->arg_int) == ERR )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				print("%%n, EVAL returned ERR\n");
# endif	/* TRACE */
			return (NULL);
		}
		stack_init();
		break;

	  /*
	  ** '%d' Print as a decimal number.
	  */
	  case 'd':
		if ( Current == NULL )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("%%d, no current\n");
# endif	/* TRACE */
			return (NULL);
		}
		Current->arg_type = ARG_INT;
		i = strtol(data,&sp,10);
		if ( data == sp && Current->arg_int == 0 )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("%%d: data == sp, or arg == 0\n");
# endif	/* TRACE */
			return (NULL);
		}
		if ( b_eval(i, &Current->arg_int) == ERR )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("%%d: eval returned ERR\n");
# endif	/* TRACE */
			return (NULL);
		}
		data = sp;
		stack_init();
		break;

	  /*
	  ** '%s' Print as a character string.
	  */
	  case 's':
		if ( Current == NULL )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("%%s: Current == NULL\n");
# endif	/* TRACE */
			return (NULL);
		}
		ch = str[1];
		i = 0;
		while ( *data != ch )
			tmp[i++] = *data++;
		tmp[i] = '\0';
		Current->arg_type = ARG_STR;
		if ( (Current->arg_str = malloc(i)) == (char *) NULL )
		{
# ifdef	TRACE
			if ( Trace(10,0) )
				printf("%%s: malloc returned NULL\n");
# endif	/* TRACE */
			return (NULL);
		}
		(void) strcpy(Current->arg_str,tmp);
		break;

	}
	*param = ++str;
# ifdef	TRACE
	if ( Trace(10,0) )
		printf("doper: return out the bottom\n");
# endif	/* TRACE */
	return (data);
}


/*
** arg_init
**	Initialize the Hold array.
*/
arg_init()
{
	int	i;

	for ( i = 0 ; i < (HOLDNUM - 1); i++ )
	{
		Hold[i].arg_next = &Hold[i + 1];
		Hold[i].arg_type = ARG_NADA;
	}
	Arg_status = 0;
	Hold[i].arg_next = (ARGS *) NULL;
	Hold[i].arg_type = ARG_NADA;
}

/*
** copy_args
**	Given a linked list of ARGS, copy them, and return a pointer
**	to the first.
** Arguments
**	list	A pointer to the list to be copied.
**
** Return Value
**	ARGS pointer	to a copied list
**	NULL on any error.
*/
ARGS	*
copy_args(list)
ARGS	*list;
{
	ARGS	*start;
	ARGS	*arg;
	ARGS	*last;
	extern	char	*calloc();

	start = (ARGS *) NULL;

# ifdef lint
	last = (ARGS *) NULL;
# endif	/* lint */

	while ( list != (ARGS *) NULL && list->arg_type != ARG_NADA)
	{
		if ( (arg = (ARGS *) calloc(1,sizeof (ARGS))) == (ARGS *) NULL)
			return ((ARGS *) NULL);
		(void) memcpy((char *) arg, (char *) list, sizeof (ARGS));
		if ( start == (ARGS *) NULL )
			start = arg;
		else
			last->arg_next = arg;
		list = list->arg_next;
		last = arg;
	}
	arg->arg_next = (ARGS *) NULL;
	return (start);
}

/*
** find_command
**	Given a string from the terminal, find out what command
**	has been issued. We look for the longest possible command.
**
** Arguments
**	str	A string to parse, looking for a command.
**	length	The length of the string.
**
** Return Value
**	The COMM structure for a command
**	NULL if it can't be found.
*/
COMM	*
find_command(str,length)
char	*str;
int	length;
{
	int	len;
	COMM	*comm;
	COMM	spare;
# ifdef	TRACE
	char	sbuf[21];
# endif	/* TRACE */
	static	char	*buf = NULL;
	extern	int	compare();

	len = MIN(Commlength,length);
	comm = NULL;
	if ( buf == NULL )
		buf = malloc((Commlength + 1) * sizeof (char));
	spare.comm_string = buf;

# ifdef	TRACE
	if ( Trace(12,0) )
	{
		strncpy(sbuf,str,20);
		sbuf[20] = '\0';
		print("find_command: (%s,%d) ",sbuf,length);
	}
# endif	/* TRACE */

	while ( len )
	{
		(void) strncpy(spare.comm_string,str,len);
		spare.comm_string[len] = '\0';

		comm = (COMM *) bsearch((char *) &spare,(char *) Commands,Numcommands,sizeof (COMM),compare);
		if ( comm != NULL )
		{
			break;
		}
		len--;
	}

# ifdef	TRACE
	if ( Trace(12,0) )
	{
		if ( comm != NULL )
			print("found %s\n",comm->comm_name);
		else
			print("not found\n");
	}
# endif	/* TRACE */
	return ( comm );
}
