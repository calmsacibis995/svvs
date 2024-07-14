# include "main.h"
# include "eval_arg.h"
# include "lib_pitch.h"
# include "device.h"

SCCSID("@(#)conf_pitch.c	3.1 (SVVS) 12/1/86");

typedef	int	(*PFI)();

void Fcw(funct_addr, front_end, call_ptr, result)
chtype		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	chtype		tmp_value;
	char		*types = "w";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window);
	}

	result->var_type = DATA_CHAR;
	result->var_data.var_chtype = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fcwnn(funct_addr, front_end, call_ptr, result)
chtype		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	chtype		tmp_value;
	char		*types = "wnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1], 
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num);
	}

	result->var_type = DATA_CHAR;
	result->var_data.var_chtype = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnw(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "w";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnw6n(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wnnnnnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0],
					  arg_list[1],
					  arg_list[2],
					  arg_list[3],
					  arg_list[4],
					  arg_list[5],
					  arg_list[6]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num,
					  arg_list[3]->var_data.var_num,
					  arg_list[4]->var_data.var_num,
					  arg_list[5]->var_data.var_num,
					  arg_list[6]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void FnwT(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wx";

	arg_list = eval_arg(call_ptr, types, NULL);

	front_end = front_end;

	var_inflate(arg_list[1]);
	tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	var_deflate(arg_list[1]);

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwb(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wb";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_bool);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wc";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwcc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wcc";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0],
					  arg_list[1],
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_chtype,
					  arg_list[2]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwnn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1], 
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void FnwnnT(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wnnx";

	arg_list = eval_arg(call_ptr, types, NULL);

	front_end = front_end;		/* Make lint happy */

	var_inflate(arg_list[3]);
	tmp_value = (*funct_addr)(arg_list[0],
				  arg_list[1],
				  arg_list[2],
				  arg_list[3]);
	var_deflate(arg_list[3]);

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwnnc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wnnc";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0],
					  arg_list[1],
					  arg_list[2],
					  arg_list[3]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num,
					  arg_list[3]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnw2nt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wnnt";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0],
					  arg_list[1],
					  arg_list[2],
					  arg_list[3]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num,
					  text_addr(arg_list[3]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnwt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wt";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window, 
					  text_addr(arg_list[1]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnww(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "ww";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_window);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnww7n(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "wwnnnnnnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0],
					  arg_list[1],
					  arg_list[2],
					  arg_list[3],
					  arg_list[4],
					  arg_list[5],
					  arg_list[6],
					  arg_list[7],
					  arg_list[8]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_window,
					  arg_list[1]->var_data.var_window,
					  arg_list[2]->var_data.var_num,
					  arg_list[3]->var_data.var_num,
					  arg_list[4]->var_data.var_num,
					  arg_list[5]->var_data.var_num,
					  arg_list[6]->var_data.var_num,
					  arg_list[7]->var_data.var_num,
					  arg_list[8]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fwnn(funct_addr, front_end, call_ptr, result)
PFI		funct_addr;
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	WINDOW		*tmp_win;
	char		*types = "nn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_win = 
		(WINDOW *) (*funct_addr)(arg_list[0], 
					 arg_list[1]);
	}
	else
	{
		tmp_win = 
		(WINDOW *) (*funct_addr)(arg_list[0]->var_data.var_num,
					 arg_list[1]->var_data.var_num);
	}

	result->var_type = DATA_WINDOW;
	result->var_data.var_window = tmp_win;

	free_arg(arg_list, strlen(types));
}

void Fwnnnn(funct_addr, front_end, call_ptr, result)
PFI		funct_addr;
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	WINDOW		*tmp_win;
	char		*types = "nnnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_win = 
		(WINDOW *) (*funct_addr)(arg_list[0],
					 arg_list[1],
					 arg_list[2],
					 arg_list[3]);
	}
	else
	{
		tmp_win = 
		(WINDOW *) (*funct_addr)(arg_list[0]->var_data.var_num,
					 arg_list[1]->var_data.var_num,
					 arg_list[2]->var_data.var_num,
					 arg_list[3]->var_data.var_num);
	}

	result->var_type = DATA_WINDOW;
	result->var_data.var_window = tmp_win;

	free_arg(arg_list, strlen(types));
}

void Fwwnnnn(funct_addr, front_end, call_ptr, result)
PFI		funct_addr;
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	WINDOW		*tmp_win;
	char		*types = "wnnnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_win = 
		(WINDOW *) (*funct_addr)(arg_list[0],
					 arg_list[1],
					 arg_list[2],
					 arg_list[3],
					 arg_list[4]);
	}
	else
	{
		tmp_win = 
		(WINDOW *) (*funct_addr)(arg_list[0]->var_data.var_window,
					 arg_list[1]->var_data.var_num,
					 arg_list[2]->var_data.var_num,
					 arg_list[3]->var_data.var_num,
					 arg_list[4]->var_data.var_num);
	}

	result->var_type = DATA_WINDOW;
	result->var_data.var_window = tmp_win;

	free_arg(arg_list, strlen(types));
}
