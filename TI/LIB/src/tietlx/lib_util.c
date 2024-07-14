# include "main.h"

SCCSID("@(#)lib_util.c	3.1 (SVVS) 12/1/86");

void get_work_str(win, str)
WINDOW	*win;
char	**str;
{
	char	tmp[BASIC_STR_LEN];
	
	wgetstr(win, tmp);
	if (*str != NULL)
	{
		free(*str);
	}
	if ((*str = malloc((unsigned) (strlen(tmp) + 2))) == NULL)
	{
		screech("get_work_str", 0, "malloc failed\n");
	}
	(void) strcpy(*str, tmp);
	(void) strcat(*str, "\n");
}

int fake_scan(work_str, fmt, arg)
char		**work_str;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	char		left_over[BASIC_STR_LEN];
	char		*add_fmt = "%[^\n]";
	char		*new_fmt;
	char		*old_fmt;
	unsigned	size;
	int		result;
	char		tmp_char;

	if (!*work_str)
	{
		screech("fake_scan", 0, "No input to read\n");
	}

	if ((*work_str)[strlen(*work_str) - 1] != '\n')
	{
		screech("fake_scan", 0, "Input not \\n terminated\n");
	}

	old_fmt = text_addr(fmt);
	size = strlen(old_fmt) + strlen(add_fmt) + 1;
	if ((new_fmt = malloc(size)) == NULL)
	{
		screech("fake_scan", 0, "malloc failed\n");
	}
	(void) strcpy(new_fmt, old_fmt);
	(void) strcat(new_fmt, add_fmt);

	switch (arg->var_type)
	{
	  case DATA_NUM:
		result = sscanf(*work_str, 
				new_fmt, 
				&(arg->var_data.var_num),
				left_over);
		break;

	  case DATA_CHAR:
		result = sscanf(*work_str, 
				new_fmt, 
				&tmp_char,
				left_over);
		arg->var_data.var_chtype = tmp_char;
		break;

	  case DATA_TEXT:
		result = sscanf(*work_str, 
				new_fmt, 
				arg->var_data.var_text_ptr, 
				left_over);
		break;

	  default:
		screech("fake_scan", 0, "Missing argument\n");
	}

	free(new_fmt);

	switch (result)
	{
	  case 2:
		free(*work_str);

		size = strlen(left_over);
		if ((*work_str = malloc(size + 2)) == NULL)
		{
			screech("fake_scan", 0, "malloc failed\n");
		}
		(void) strcpy(*work_str, left_over);
		(void) strcat(*work_str, "\n");

		break;

	  case 1:
		free(*work_str);
		*work_str = NULL;
		break;

	  case 0:
		break;

	  default:
		screech("fake_scan", 0, "sscanf return %d\n", result);
	}

	return(result);
}

/*
** Value Call
**
**	Call function with printf/scanf style.
**	All parameters are passed by value.
*/

int p_v_call(funct_addr, fmt, arg)
int		(*funct_addr)();
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	char		*fmt_str;
	int		result;

	fmt_str = text_addr(fmt);

	switch (arg->var_type)
	{
	  case DATA_NUM:
		result = (*funct_addr)(fmt_str, arg->var_data.var_num);
		break;

	  case DATA_CHAR:
		result = (*funct_addr)(fmt_str, arg->var_data.var_chtype);
		break;

	  case DATA_TEXT:
		result = (*funct_addr)(fmt_str, text_addr(arg));
		break;

	  default:
		result = (*funct_addr)(fmt_str);
	}

	return(result);
}

/*
** Window/Value Call
**
**	Call function with printf/scanf style.
**	Call needs a window and all parameters are passed by value
*/

int p_wv_call(funct_addr, win, fmt, arg)
int		(*funct_addr)();
VAR_ENTRY	*win;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	WINDOW		*win_ptr;
	char		*fmt_str;
	int		result;

	win_ptr = win->var_data.var_window;
	fmt_str = text_addr(fmt);

	switch (arg->var_type)
	{
	  case DATA_NUM:
		result = (*funct_addr)(win_ptr, 
				       fmt_str, 
				       arg->var_data.var_num);
		break;

	  case DATA_CHAR:
		result = (*funct_addr)(win_ptr, 
				       fmt_str, 
				       arg->var_data.var_chtype);
		break;

	  case DATA_TEXT:
		result = (*funct_addr)(win_ptr, 
				       fmt_str, 
				       text_addr(arg));
		break;

	  default:
		result = (*funct_addr)(win_ptr, 
				       fmt_str);
	}

	return(result);
}

/*
** Reference Call
**
**	Call function with printf/scanf style.
**	All parameters are passed by reference.
*/

int p_r_call(funct_addr, fmt, arg)
int		(*funct_addr)();
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	char		*fmt_str;
	int		result;

	fmt_str = text_addr(fmt);

	switch (arg->var_type)
	{
	  case DATA_NUM:
		result = (*funct_addr)(fmt_str, &(arg->var_data.var_num));
		break;

	  case DATA_CHAR:
		result = (*funct_addr)(fmt_str, &(arg->var_data.var_chtype));
		break;

	  case DATA_TEXT:
		result = (*funct_addr)(fmt_str, text_addr(arg));
		break;

	  default:
		result = (*funct_addr)(fmt_str);
	}

	return(result);
}

/*
** Window/Reference Call
**
**	Call function with printf/scanf style.
**	Call needs a window and all parameters are passed by reference
*/

int p_wr_call(funct_addr, win, fmt, arg)
int		(*funct_addr)();
VAR_ENTRY	*win;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	WINDOW		*win_ptr;
	char		*fmt_str;
	int		result;

	win_ptr = win->var_data.var_window;
	fmt_str = text_addr(fmt);
	
	switch (arg->var_type)
	{
	  case DATA_NUM:
		result = (*funct_addr)(win_ptr, 
				       fmt_str, 
				       &(arg->var_data.var_num));
		break;

	  case DATA_CHAR:
		result = (*funct_addr)(win_ptr, 
				       fmt_str, 
				       &(arg->var_data.var_chtype));
		break;

	  case DATA_TEXT:
		result = (*funct_addr)(win_ptr, 
				       fmt_str, 
				       text_addr(arg));
		break;

	  default:
		result = (*funct_addr)(win_ptr, 
				       fmt_str);
	}

	return(result);
}
