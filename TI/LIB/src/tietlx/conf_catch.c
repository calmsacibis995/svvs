# include "main.h"
# include "eval_arg.h"
# include "lib_pitch.h"
# include "device.h"

SCCSID("@(#)conf_catch.c	3.1 (SVVS) 12/1/86");

typedef	int	(*PFI)();

void Fcm(funct_addr, front_end, call_ptr, result)
chtype		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	chtype		tmp_value;
	char		*types = "m";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term);
	}

	result->var_type = DATA_CHAR;
	result->var_data.var_chtype = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fcmnn(funct_addr, front_end, call_ptr, result)
chtype		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	chtype		tmp_value;
	char		*types = "mnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1], 
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num);
	}

	result->var_type = DATA_CHAR;
	result->var_data.var_chtype = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnm(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "m";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnm6n(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mnnnnnn";

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
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
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

void FnmT(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mx";

	arg_list = eval_arg(call_ptr, types, NULL);

	front_end = front_end;

	var_inflate(arg_list[1]);
	tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	var_deflate(arg_list[1]);

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmb(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mb";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_bool);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mc";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmcc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mcc";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0],
					  arg_list[1],
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_chtype,
					  arg_list[2]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmnn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1], 
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void FnmnnT(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mnnx";

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

void Fnmnnc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mnnc";

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
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num,
					  arg_list[3]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmnnt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mnnt";

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
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num,
					  text_addr(arg_list[3]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mt";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term, 
					  text_addr(arg_list[1]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmm(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mm";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_term);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnmm7n(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "mmnnnnnnn";

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
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_term,
					  arg_list[1]->var_data.var_term,
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

void Fmnn(funct_addr, front_end, call_ptr, result)
PFI		funct_addr;
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	TERMINAL	*tmp_term;
	char		*types = "nn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_term = 
		(TERMINAL *) (*funct_addr)(arg_list[0], 
					 arg_list[1]);
	}
	else
	{
		tmp_term = 
		(TERMINAL *) (*funct_addr)(arg_list[0]->var_data.var_num,
					   arg_list[1]->var_data.var_num);
	}

	result->var_type = DATA_TERM;
	result->var_data.var_term = tmp_term;

	free_arg(arg_list, strlen(types));
}

void Fmnnnn(funct_addr, front_end, call_ptr, result)
PFI		funct_addr;
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	TERMINAL	*tmp_term;
	char		*types = "nnnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_term = 
		(TERMINAL *) (*funct_addr)(arg_list[0],
					   arg_list[1],
					   arg_list[2],
					   arg_list[3]);
	}
	else
	{
		tmp_term = 
		(TERMINAL *) (*funct_addr)(arg_list[0]->var_data.var_num,
					   arg_list[1]->var_data.var_num,
					   arg_list[2]->var_data.var_num,
					   arg_list[3]->var_data.var_num);
	}

	result->var_type = DATA_TERM;
	result->var_data.var_term = tmp_term;

	free_arg(arg_list, strlen(types));
}

void Fmmnnnn(funct_addr, front_end, call_ptr, result)
PFI		funct_addr;
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	TERMINAL	*tmp_term;
	char		*types = "mnnnn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_term = 
		(TERMINAL *) (*funct_addr)(arg_list[0],
					   arg_list[1],
					   arg_list[2],
					   arg_list[3],
					   arg_list[4]);
	}
	else
	{
		tmp_term = 
		(TERMINAL *) (*funct_addr)(arg_list[0]->var_data.var_term,
					   arg_list[1]->var_data.var_num,
					   arg_list[2]->var_data.var_num,
					   arg_list[3]->var_data.var_num,
					   arg_list[4]->var_data.var_num);
	}

	result->var_type = DATA_TERM;
	result->var_data.var_term = tmp_term;

	free_arg(arg_list, strlen(types));
}
