# include "main.h"

SCCSID("@(#)convert.c	3.2 (SVID) 12/30/86");

VAR_ENTRY *convert(src_ptr, convert_type)
VAR_ENTRY	*src_ptr;
DATA_TYPE	convert_type;
{
	VAR_ENTRY	*do_convert();
	VAR_ENTRY	*dest_ptr;

	if (src_ptr->var_type == DATA_ARRAY)
	{
		screech("convert", 0, "Trying to convert an array!\n");
	}

	if ((src_ptr->var_type != convert_type) && (convert_type != DATA_NULL))
	{
		dest_ptr = do_convert(src_ptr, convert_type);

		if (src_ptr->var_class == CLASS_TEMP)
		{
			free_text(src_ptr);
			free_var(src_ptr);
		}
	}
	else
	{
		dest_ptr = src_ptr;
	}

	return(dest_ptr);
}

VAR_ENTRY *do_convert(target_ptr, convert_type)
VAR_ENTRY	*target_ptr;
DATA_TYPE	convert_type;
{
	VAR_ENTRY	*convert_ptr;

	if ((convert_ptr = (VAR_ENTRY *) malloc(sizeof(VAR_ENTRY))) == NULL)
	{
		screech("do_convert", 0, "malloc failed\n");
	}

	convert_ptr->var_class = CLASS_TEMP;
	convert_ptr->var_type = convert_type;
	convert_ptr->var_name.name_ptr = NULL;

	switch(convert_type)
	{
	  case DATA_NUM:
		to_num(target_ptr, convert_ptr);
		break;

	  case DATA_BOOL:
		to_bool(target_ptr, convert_ptr);
		break;

	  case DATA_CHAR:
		to_char(target_ptr, convert_ptr);
		break;

	  case DATA_TEXT:
		to_text(target_ptr, convert_ptr);
		break;

	  case DATA_WINDOW:
		to_window(target_ptr, convert_ptr);
		break;

	  case DATA_SCREEN:
		to_screen(target_ptr, convert_ptr);
		break;

	  case DATA_TERM:
		to_term(target_ptr, convert_ptr);
		break;

	  default:
		screech("convert", 0, "Bad type = %d\n", convert_type);
	}

	return(convert_ptr);
}

to_num(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	NUMBER_TYPE	tmp_num;
	char		*tmp_text;

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		redundant_error();

	  case DATA_BOOL:
		tmp_num = src_ptr->var_data.var_num;
		break;

	  case DATA_CHAR:
		tmp_num = (NUMBER_TYPE) src_ptr->var_data.var_chtype;
		break;

	  case DATA_TEXT:
		tmp_text = text_addr(src_ptr);
		(void) text_num(tmp_text, &tmp_num);
		break;

	  case DATA_WINDOW:
		tmp_num = (NUMBER_TYPE) src_ptr->var_data.var_window;
		break;

	  case DATA_SCREEN:
		tmp_num = (NUMBER_TYPE) src_ptr->var_data.var_screen;
		break;

	  case DATA_TERM:
		tmp_num = (NUMBER_TYPE) src_ptr->var_data.var_term;
		break;

	  default:
		screech("to_num", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}
	dest_ptr->var_data.var_num = tmp_num;
}

to_bool(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	BOOLEAN		tmp_bool;
	NUMBER_TYPE	tmp_num;
	char		*tmp_text;

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		tmp_bool = src_ptr->var_data.var_num != 0;
		break;

	  case DATA_BOOL:
		redundant_error();

	  case DATA_CHAR:
		tmp_bool = (NUMBER_TYPE) src_ptr->var_data.var_chtype != '\0';
		break;

	  case DATA_TEXT:
		tmp_text = text_addr(src_ptr);
		if (text_num(tmp_text, &tmp_num))
		{
			tmp_bool = (tmp_num != 0);
		}
		else
		{
			if (strcmp(tmp_text, "TRUE") == 0)
			{
				tmp_bool = TRUE;
			}
			else if (strcmp(tmp_text, "FALSE") == 0)
			{
				tmp_bool = FALSE;
			}
			else
			{
				tmp_bool = (strcmp(tmp_text, "") != 0);
			}
		}
		break;

	  case DATA_WINDOW:
		tmp_bool = (NUMBER_TYPE) src_ptr->var_data.var_window != NULL;
		break;

	  case DATA_SCREEN:
		tmp_bool = (NUMBER_TYPE) src_ptr->var_data.var_screen != NULL;
		break;

	  case DATA_TERM:
		tmp_bool = (NUMBER_TYPE) src_ptr->var_data.var_term != NULL;
		break;

	  default:
		screech("to_num", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}
	dest_ptr->var_data.var_bool = tmp_bool;
}

to_char(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	chtype	tmp_char;

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		tmp_char = (chtype) src_ptr->var_data.var_num;
		break;

	  case DATA_BOOL:
		tmp_char = (src_ptr->var_data.var_bool) ? 'T' : 'F';
		break;

	  case DATA_CHAR:
		redundant_error();

	  case DATA_TEXT:
		tmp_char = text_addr(src_ptr)[0];
		break;

	  case DATA_WINDOW:
		tmp_char = (chtype) src_ptr->var_data.var_window;
		break;

	  case DATA_SCREEN:
		tmp_char = (chtype) src_ptr->var_data.var_screen;
		break;

	  case DATA_TERM:
		tmp_char = (chtype) src_ptr->var_data.var_term;
		break;

	  default:
		screech("to_char", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}
	dest_ptr->var_data.var_chtype = tmp_char;
}

to_text(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	char		tmp_text[BASIC_STR_LEN];

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		(void) sprintf(tmp_text, "%d", src_ptr->var_data.var_num);
		break;

	  case DATA_BOOL:
		if (src_ptr->var_data.var_num)
		{
			(void) strcpy(tmp_text, "TRUE");
		}
		else
		{
			(void) strcpy(tmp_text, "FALSE");
		}
		break;

	  case DATA_CHAR:
		tmp_text[0] = src_ptr->var_data.var_chtype;
		tmp_text[1] = '\0';
		break;

	  case DATA_TEXT:
		redundant_error();

	  case DATA_WINDOW:
		(void) sprintf(tmp_text, 
			       "%d", 
			       (NUMBER_TYPE) src_ptr->var_data.var_window);
		break;

	  case DATA_SCREEN:
		(void) sprintf(tmp_text, 
			       "%d", 
			       (NUMBER_TYPE) src_ptr->var_data.var_screen);
		break;

	  case DATA_TERM:
		(void) sprintf(tmp_text, 
			       "%d", 
			       (NUMBER_TYPE) src_ptr->var_data.var_term);
		break;

	  default:
		screech("to_text", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}

	dest_ptr->var_data.var_text_ptr = 
		malloc((unsigned) (strlen(tmp_text) + 1));

	if (dest_ptr->var_data.var_text_ptr == NULL)
	{
		screech("to_text", 0, "malloc returns zero\n");
	}

	(void) strcpy(dest_ptr->var_data.var_text_ptr, tmp_text);
}

to_window(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	WINDOW		*tmp_window;
	NUMBER_TYPE	tmp_num;
	char		*tmp_text;

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		tmp_window = (WINDOW *) src_ptr->var_data.var_num;
		break;

	  case DATA_BOOL:
		tmp_window = NULL;
		break;

	  case DATA_CHAR:
		tmp_window = (WINDOW *) src_ptr->var_data.var_chtype;
		break;

	  case DATA_TEXT:
		tmp_text = text_addr(src_ptr);
		(void) text_num(tmp_text, &tmp_num);
		tmp_window = (WINDOW *) tmp_num;
		break;

	  case DATA_WINDOW:
		redundant_error();

	  case DATA_SCREEN:
		tmp_window = (WINDOW *) src_ptr->var_data.var_screen;
		break;

	  case DATA_TERM:
		tmp_window = (WINDOW *) src_ptr->var_data.var_term;
		break;

	  default:
		screech("convert_window", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}
	dest_ptr->var_data.var_window = tmp_window;
}

to_screen(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	SCREEN		*tmp_screen;
	NUMBER_TYPE	tmp_num;
	char		*tmp_text;

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		tmp_screen = (SCREEN *) src_ptr->var_data.var_num;
		break;

	  case DATA_BOOL:
		tmp_screen = NULL;
		break;

	  case DATA_CHAR:
		tmp_screen = (SCREEN *) src_ptr->var_data.var_chtype;
		break;

	  case DATA_TEXT:
		tmp_text = text_addr(src_ptr);
		(void) text_num(tmp_text, &tmp_num);
		tmp_screen = (SCREEN *) tmp_num;
		break;

	  case DATA_WINDOW:
		tmp_screen = (SCREEN *) src_ptr->var_data.var_window;
		break;

	  case DATA_SCREEN:
		redundant_error();

	  case DATA_TERM:
		tmp_screen = (SCREEN *) src_ptr->var_data.var_term;
		break;

	  default:
		screech("convert_screen", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}
	dest_ptr->var_data.var_screen = tmp_screen;
}

to_term(src_ptr, dest_ptr)
VAR_ENTRY	*src_ptr;
VAR_ENTRY	*dest_ptr;
{
	TERMINAL	*tmp_term;
	NUMBER_TYPE	tmp_num;
	char		*tmp_text;

	switch(src_ptr->var_type)
	{
	  case DATA_NUM:
		tmp_term = (TERMINAL *) src_ptr->var_data.var_num;
		break;

	  case DATA_BOOL:
		tmp_term = NULL;
		break;

	  case DATA_CHAR:
		tmp_term = (TERMINAL *) src_ptr->var_data.var_chtype;
		break;

	  case DATA_TEXT:
		tmp_text = text_addr(src_ptr);
		(void) text_num(tmp_text, &tmp_num);
		tmp_term = (TERMINAL *) tmp_num;
		break;

	  case DATA_WINDOW:
		tmp_term = (TERMINAL *) src_ptr->var_data.var_window;
		break;

	  case DATA_SCREEN:
		tmp_term = (TERMINAL *) src_ptr->var_data.var_screen;
		break;

	  case DATA_TERM:
		redundant_error();

	  default:
		screech("convert_screen", 0, "Bad type = '%c'\n", src_ptr->var_type);
	}
	dest_ptr->var_data.var_term = tmp_term;
}

redundant_error()
{
	screech("convert", 0, "Redundant conversion\n");
}
