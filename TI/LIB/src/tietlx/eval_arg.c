# include "main.h"
# include "eval_arg.h"

SCCSID("@(#)eval_arg.c	3.1 (SVID) 12/1/86");

char		get_format();
DATA_TYPE	fmt_to_type();
void 		v_arg_convert();

VAR_ARG_ENTRY *eval_var_arg(scanf_type, format, expr_index)
BOOLEAN		scanf_type;
char		*format;
TBL_INDEX	expr_index;
{
	char		*partial;
	char		fmt_ch;
	char		*tmp_str;
	VAR_ARG_ENTRY	*new_entry;
	VAR_ARG_ENTRY	*last_entry ;
	VAR_ARG_ENTRY	*var_arg_list = NULL;

	while (fmt_ch = get_format(format, &partial))
	{
		format = NULL;
		new_entry = (VAR_ARG_ENTRY *) malloc(sizeof(VAR_ARG_ENTRY));
		if (new_entry == NULL)
		{
			screech("eval_var_arg", 0, "malloc failed\n");
		}

		/*
		** Evaluate argument
		*/

		new_entry->var_arg_type = fmt_to_type(fmt_ch);

		if (new_entry->var_arg_type == DATA_NULL)
		{
			new_entry->var_arg_arg = NULL;
		}
		else
		{
			expr_index = do_eval(expr_index, 
					     &(new_entry->var_arg_arg));
		}

		/*
		** Copy partial format string
		*/

		new_entry->var_arg_fmt = 
			(VAR_ENTRY *) malloc(sizeof(VAR_ENTRY));
		if (new_entry == NULL)
		{
			screech("eval_var_arg", 0, "malloc failed\n");
		}

		new_entry->var_arg_fmt->var_data.var_text_ptr = 
			malloc((unsigned) (strlen(partial) + 1));
		if (new_entry->var_arg_fmt->var_data.var_text_ptr == NULL)
		{
			screech("eval_var_arg", 0, "malloc failed\n");
		}

		(void) strcpy(new_entry->var_arg_fmt->var_data.var_text_ptr, 
			      partial);

		/*
		** Assign values to argument list
		*/

		v_arg_convert(scanf_type, new_entry);

		if (var_arg_list == NULL)
		{
			var_arg_list = new_entry;
			last_entry = new_entry;
		}
		else
		{
			last_entry->var_arg_next = new_entry;
			last_entry = new_entry;
		}
		last_entry->var_arg_next = NULL;
	}

	return(var_arg_list);
}

void free_v_arg(var_arg_list)
VAR_ARG_ENTRY	*var_arg_list;
{
	VAR_ARG_ENTRY	*guide;
	VAR_ARG_ENTRY	*junk;

	guide = var_arg_list;
	while (guide != NULL)
	{
		free_text(guide->var_arg_fmt);
		free_var(guide->var_arg_fmt);

		if (guide->var_arg_arg)
		{
			if (guide->var_arg_arg->var_class == CLASS_TEMP)
			{
				free_text(guide->var_arg_arg);
				free_var(guide->var_arg_arg);
			}
			else if (guide->inflated)
			{
				var_deflate(guide->var_arg_arg);
			}
		}

		junk = guide;
		guide = guide->var_arg_next;
		free(junk);
	}
}

char get_format(format, partial)
char	*format;
char	**partial;
{
	static	char	save_char;
	static	char	*hold;
	auto	char	result;
	auto	char	*target = "douxXfeEgGcs";

	*partial = NULL;
	result = '\0';

	if (format)
	{
		hold = format;
	}
	else if ((hold) && (save_char != '\0'))
	{
		*hold = save_char;
	} 
	else
	{
		return(result);
	}

	*partial = hold;
	result = ' ';

	while ((hold = strchr(hold, '%')) && (*(hold + 1) == '%'))
	{
		hold += 2;
	}

	if ((*hold == '%') && (hold = strpbrk(hold + 1, target)))
	{
		result = *hold;
		save_char = *(++hold);
		*hold = '\0';
	}

	return(result);
}

DATA_TYPE fmt_to_type(fmt_char)
char		fmt_char;
{
	DATA_TYPE	result;

	switch(fmt_char)
	{
	  case ' ':
		result = DATA_NULL;
		break;

	  case 'd':
	  case 'o':
	  case 'u':
	  case 'x':
	  case 'X':
		result = DATA_NUM;
		break;

	  case 's':
		result = DATA_TEXT;
		break;

	  case 'c':
		result = DATA_CHAR;
		break;

	  default:
		screech("fmt_to_type", 0, 
			"bad format type %c\n", fmt_char);
	}

	return(result);
}

void v_arg_convert(scanf_type, new_entry)
BOOLEAN		scanf_type;
VAR_ARG_ENTRY	*new_entry;
{
	new_entry->inflated = FALSE;
	if (scanf_type)
	{
		if (new_entry->var_arg_arg->var_type == DATA_TEXT)
		{
			free_text(new_entry->var_arg_arg);
		}

		new_entry->var_arg_arg->var_type = new_entry->var_arg_type;

		if (new_entry->var_arg_arg->var_type == DATA_TEXT)
		{
			new_entry->var_arg_arg->var_data.var_text_ptr = NULL;
			var_inflate(new_entry->var_arg_arg);
			new_entry->inflated = TRUE;
		}
		else if (new_entry->var_arg_arg->var_type == DATA_CHAR)
		{
			new_entry->inflated = (SideType == SIDE_PITCHER);
		}
	}
	else if (new_entry->var_arg_arg)
	{
		new_entry->var_arg_arg = convert(new_entry->var_arg_arg, 
					         new_entry->var_arg_type);
	}
}

VAR_ENTRY **eval_arg(call_ptr, arg_type, next_expr)
CALL_ENTRY	*call_ptr;
char		*arg_type;
TBL_INDEX	*next_expr;
{
	VAR_ENTRY	**arg_list;
	unsigned	arg_count;
	TBL_INDEX	arg_index;
	TBL_INDEX	expr_index;

	unsigned	num_byte;
	DATA_TYPE	eval_type;

/*
** Initialize variables
*/

	expr_index = call_ptr->call_expr;
	arg_count = strlen(arg_type);

	num_byte = arg_count * sizeof(VAR_ENTRY *);
	arg_list = (VAR_ENTRY **) malloc(num_byte);

/*
** Evaluate expressions
** Print final args
*/

# ifdef TRACE
	if (Trace(10, 0))
	{
		if (SideType == SIDE_CATCHER)
		{
			fprintf(stderr, "(");
		}
	}
# endif /* TRACE */

	for (arg_index = 0; arg_index < arg_count; arg_index++)
	{
		if (arg_index >= strlen(arg_type))
		{
			eval_type = DATA_NUM;
		}
		else
		{
			eval_type = arg_type[arg_index];
		}

		expr_index = do_eval(expr_index, &arg_list[arg_index]);
		arg_list[arg_index] = convert(arg_list[arg_index], eval_type);

# ifdef TRACE
		if (Trace(10, 0))
		{
			if (SideType == SIDE_CATCHER)
			{
				shw_var(NIL, arg_list[arg_index]);
				if (arg_index + 1 < arg_count)
				{
					fprintf(stderr, ", ");
				}
			}
		}
# endif /* TRACE */

	}

# ifdef TRACE
	if (Trace(10, 0))
	{
		if (SideType == SIDE_CATCHER)
		{
			fprintf(stderr, ")\n");
		}
	}
# endif /* TRACE */

	if (next_expr)
	{
		*next_expr = expr_index;
	}

	return(arg_list);
}

void free_arg(arg_list, arg_count)
VAR_ENTRY	**arg_list;
int		arg_count;
{
	int	arg_index;

	for (arg_index = 0; arg_index < arg_count; arg_index++)
	{
		if (arg_list[arg_index]->var_class == CLASS_TEMP)
		{
			free_text(arg_list[arg_index]);
			free_var(arg_list[arg_index]);
		}
	}
	free(arg_list);
}
