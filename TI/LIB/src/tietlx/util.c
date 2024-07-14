# include "main.h"
# include "option.h"
# include "eval_arg.h"
# include <fcntl.h>

SCCSID("@(#)util.c	3.1 (SVID) 12/1/86");

void free_text(var_ptr)
VAR_ENTRY	*var_ptr;
{
	if ((var_ptr->var_class != CLASS_CONST) &&
	    (var_ptr->var_type == DATA_TEXT) && 
	    (var_ptr->var_data.var_text_ptr != NULL))
	{
		free(var_ptr->var_data.var_text_ptr);
		var_ptr->var_data.var_text_ptr = NULL;
	}
}

void free_var(var_ptr)
VAR_ENTRY	*var_ptr;
{
	if ((var_ptr != NULL) && ((var_ptr)->var_class == CLASS_TEMP))
	{
		free(var_ptr);
	}
}

BOOLEAN text_num(var_text, var_num)
char		*var_text;
NUMBER_TYPE	*var_num;
{
	char	junk_char;
	BOOLEAN	result;
	char	new_fmt[10];

	(void) sprintf(new_fmt, "%s%%c", "%d");

	if (sscanf(var_text, new_fmt, var_num, &junk_char) == 1)
	{
		result = TRUE;
	}
	else
	{
		*var_num = 0;
		result = FALSE;
	}

	return(result);
}

int pprintf(dest_str, fmt, arg)
char		**dest_str;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	char		*fmt_str;
	char		*tmp_str;
	unsigned	tmp_size;
	unsigned	new_size;
	int		result;

	fmt_str = text_addr(fmt);

	if (arg->var_type == DATA_TEXT)
	{
		tmp_size = strlen(text_addr(arg)) + BASIC_STR_LEN;
	}
	else
	{
		tmp_size = BASIC_STR_LEN;
	}

	if ((tmp_str = malloc(tmp_size)) == NULL)
	{
		screech("pprintf", 0, "malloc failed\n");
	}

	switch(arg->var_type)
	{ 
	  case DATA_NUM:
		result = sprintf(tmp_str, fmt_str, arg->var_data.var_num);
		break;

	  case DATA_CHAR:
		result = sprintf(tmp_str, fmt_str, arg->var_data.var_chtype);
		break;

	  case DATA_TEXT:
		result = sprintf(tmp_str, fmt_str, text_addr(arg));
		break;

	  default:
		result = sprintf(tmp_str, fmt_str);
	}

	if (FirstCall)
	{
		new_size = strlen(tmp_str) + 1;
		if ((*dest_str = malloc(new_size)) == NULL)
		{
			screech("pprintf", 0, "malloc failed\n");
		}
		(void) strcpy(*dest_str, tmp_str);
	}
	else
	{
		new_size = strlen(*dest_str) + strlen(tmp_str) + 1;
		if ((*dest_str = realloc(*dest_str, new_size)) == NULL)
		{
			screech("pprintf", 0, 
				"realloc failed. new_size = %d\n", new_size);
		}
		(void) strcat(*dest_str, tmp_str);
	}

	free(tmp_str);

	return(result);
}

void var_inflate(str)
VAR_ENTRY	*str;
{
	char	*mondo_str;

	if (str->var_class == CLASS_CONST)
	{
		screech("var_inflate", 0, "trying to inflate a constant\n");
	}

	if ((mondo_str = malloc((unsigned) BASIC_STR_LEN)) == NULL)
	{
		screech("var_inflate", 0, "malloc failed\n");
	}

	*mondo_str = '\0';

	if (str->var_type == DATA_TEXT)
	{
		free_text(str);
	}
	else
	{
		str->var_type = DATA_TEXT;
	}

	str->var_data.var_text_ptr = mondo_str;
}

void var_deflate(str)
VAR_ENTRY	*str;
{
	unsigned	tmp_size;
	char		*tmp_str;

	switch (str->var_type)
	{
	  case DATA_TEXT:
		tmp_size = strlen(text_addr(str)) + 1;
		if ((tmp_str = malloc(tmp_size)) == NULL)
		{
			screech("var_deflate", 0, "malloc failed\n");
		}

		(void) strcpy(tmp_str, text_addr(str));

		free(text_addr(str));
		str->var_data.var_text_ptr = tmp_str;
		break;

	  case DATA_CHAR:
		str->var_data.var_chtype = str->var_data.var_char;
		break;

	  default:
		screech("var_deflate", 0, 
			"Trying to deflate var type = %d\n", str->var_type);
	}
}

int set_var_arg(var_arg_list, var_arg_tbl)
VAR_ARG_ENTRY	*var_arg_list;
VAR_ENTRY	*var_arg_tbl[];
{
	int		tbl_index = 0;
	int		result = 0;
	VAR_ARG_ENTRY	*guide;

	for (guide = var_arg_list; guide != NULL; guide = guide->var_arg_next)
	{
		switch (guide->var_arg_type)
		{
		  case DATA_CHAR:
			var_arg_tbl[tbl_index++] = guide->var_arg_arg;
			result = (result * 8) + 1;
			break;

		  case DATA_TEXT:
			var_arg_tbl[tbl_index++] = guide->var_arg_arg;
			result = (result * 8) + 2;
			break;

		  case DATA_NUM:
			var_arg_tbl[tbl_index++] = guide->var_arg_arg;
			result = (result * 8) + 3;
			break;

		  case DATA_NULL:
			break;

		  default:
			screech("set_var_arg", 0, 
				"Bad var_arg_type", guide->var_arg_type);
		}
	}

# ifdef TRACE
	if (Trace(20, 0))
	{
		fprintf(stderr, "set_var_arg = %3.3o\n", result);
	}
# endif /* TRACE */

	return(result);
}

void cx_initscr()
{
	(void) c_initscr();
	reload_var();
}
