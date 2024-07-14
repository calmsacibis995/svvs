# include "main.h"
# include "eval_arg.h"
# include "lib_pitch.h"
# include "device.h"
# include "comb.h"

SCCSID("@(#)conf_both.c	3.2 (SVVS) 12/30/86");

/*
** "FirstCall" tells us the first partial call to "printf()" and "scanf()"
** type functions
**
** "LastCall" tells us the last partial call to "printf()" and "scanf()"
** type functions
*/

BOOLEAN FirstCall;
BOOLEAN LastCall;

typedef	int	(*PFI)();

void F__(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	front_end = front_end;
	call_ptr = call_ptr;

# ifdef TRACE
	if (Trace(10, 0))
	{
		if (SideType == SIDE_CATCHER)
		{
			fprintf(stderr, "()");
		}
	}
# endif /* TRACE */

	(void) (*funct_addr)();

	result->var_type = DATA_NUM;
	result->var_data.var_num = 0;
}

void Fc_(funct_addr, front_end, call_ptr, result)
chtype		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	chtype		tmp_value;

	front_end = front_end;
	call_ptr = call_ptr;

# ifdef TRACE
	if (Trace(10, 0))
	{
		if (SideType == SIDE_CATCHER)
		{
			fprintf(stderr, "()");
		}
	}
# endif /* TRACE */

	tmp_value = (*funct_addr)();

	result->var_type = DATA_CHAR;
	result->var_data.var_chtype = tmp_value;
}

void Fcnn(funct_addr, front_end, call_ptr, result)
chtype		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	chtype		tmp_value;
	char		*types = "nn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  arg_list[1]->var_data.var_num);
	}

	result->var_type = DATA_CHAR;
	result->var_data.var_chtype = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fn4n(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "nnnn";

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
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_num,
					  arg_list[3]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void FnT(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "x";

	arg_list = eval_arg(call_ptr, types, NULL);

	front_end = front_end;

	var_inflate(arg_list[0]);
	tmp_value = (*funct_addr)(arg_list[0]);
	var_deflate(arg_list[0]);

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fn_(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	int		tmp_value;

	front_end = front_end;
	call_ptr = call_ptr;

# ifdef TRACE
	if (Trace(10, 0))
	{
		if (SideType == SIDE_CATCHER)
		{
			fprintf(stderr, "()");
		}
	}
# endif /* TRACE */

	tmp_value = (*funct_addr)();

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;
}

void Fnc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "c";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "n";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnnn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "nn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  arg_list[1]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void FnnnT(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "nnx";

	front_end = front_end;

	arg_list = eval_arg(call_ptr, types, NULL);

	var_inflate(arg_list[2]);
	tmp_value = (*funct_addr)(arg_list[0], arg_list[1], arg_list[2]);
	var_deflate(arg_list[2]);

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnnnc(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "nnc";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1], 
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  arg_list[1]->var_data.var_num,
					  arg_list[2]->var_data.var_chtype);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnnnt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "nnt";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1], 
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  arg_list[1]->var_data.var_num,
					  text_addr(arg_list[2]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnnt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "nt";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  text_addr(arg_list[1]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnntn(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "ntn";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0], 
					  arg_list[1],
					  arg_list[2]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num,
					  text_addr(arg_list[1]),
					  arg_list[2]->var_data.var_num);
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Fnt(funct_addr, front_end, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value;
	char		*types = "t";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(text_addr(arg_list[0]));
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_arg(arg_list, strlen(types));
}

void Ft_(funct_addr, front_end, call_ptr, result)
char		*(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	char		*tmp_value;

	front_end = front_end;
	call_ptr = call_ptr;

# ifdef TRACE
	if (Trace(10, 0))
	{
		if (SideType == SIDE_CATCHER)
		{
			fprintf(stderr, "()");
		}
	}
# endif /* TRACE */

	tmp_value = (*funct_addr)();

	result->var_type = DATA_TEXT;
	result->var_data.var_text_ptr = tmp_value;
}

void Ftc(funct_addr, front_end, call_ptr, result)
char		*(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	char		*tmp_value;
	char		*return_value;
	char		*types = "c";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num);
	}

	if ((return_value = malloc((unsigned) (strlen(tmp_value) + 1))) == NULL)
	{
		screech("Ftc", 0, "malloc failed\n");
	}

	(void) strcpy(return_value, tmp_value);

	result->var_type = DATA_TEXT;
	result->var_data.var_text_ptr = return_value;

	free_arg(arg_list, strlen(types));
}

void Ftn(funct_addr, front_end, call_ptr, result)
char		*(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	char		*tmp_value;
	char		*return_value;
	char		*types = "n";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(arg_list[0]->var_data.var_num);
	}

	if ((return_value = malloc((unsigned) (strlen(tmp_value) + 1))) == NULL)
	{
		screech("Ftn", 0, "malloc failed\n");
	}

	(void) strcpy(return_value, tmp_value);

	result->var_type = DATA_TEXT;
	result->var_data.var_text_ptr = return_value;

	free_arg(arg_list, strlen(types));
}

void Ftt(funct_addr, front_end, call_ptr, result)
char		*(*funct_addr)();
BOOLEAN		front_end;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	char		*tmp_value;
	char		*types = "t";

	arg_list = eval_arg(call_ptr, types, NULL);

	if (front_end)
	{
		tmp_value = (*funct_addr)(arg_list[0]);
	}
	else
	{
		tmp_value = (*funct_addr)(text_addr(arg_list[0]));
	}

	result->var_type = DATA_TEXT;
	result->var_data.var_text_ptr = tmp_value;

	free_arg(arg_list, strlen(types));
}

void FnTt0(funct_addr, scanf_type, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		scanf_type;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value = 0;
	char		*types = "xt";

	VAR_ARG_ENTRY	*var_arg_list;
	VAR_ARG_ENTRY	*guide;
	TBL_INDEX	next_expr;
	PFI		funct_special;

	arg_list = eval_arg(call_ptr, types, &next_expr);
	var_arg_list = eval_var_arg(scanf_type, 
				    text_addr(arg_list[1]), 
				    next_expr);

	FirstCall = TRUE;
	LastCall = FALSE;

	funct_special = Funct[call_ptr->call_funct_index].funct_special;

	if (((call_ptr->call_expr_count - strlen(types)) <= COMB_COUNT) &&
	    (funct_special != NULL) &&
	    (SideType == SIDE_PITCHER))
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Special\n");
		}
# endif /* TRACE */

		tmp_value = (*funct_special)(arg_list[0], 
					     arg_list[1],
					     var_arg_list);
	}
	else
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Normal\n");
		}
# endif /* TRACE */

		for (guide = var_arg_list; guide; guide = guide->var_arg_next)
		{
			if (!guide->var_arg_next)
			{
				LastCall = TRUE;
			}

			tmp_value += (*funct_addr)(arg_list[0], 
						   guide->var_arg_fmt,
						   guide->var_arg_arg);

			FirstCall = FALSE;
		}
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_v_arg(var_arg_list);
	free_arg(arg_list, strlen(types));
}

void Fnnnt0(funct_addr, scanf_type, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		scanf_type;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value = 0;
	char		*types = "nnt";

	VAR_ARG_ENTRY	*var_arg_list;
	VAR_ARG_ENTRY	*guide;
	TBL_INDEX	next_expr;
	PFI		funct_special;

	arg_list = eval_arg(call_ptr, types, &next_expr);
	var_arg_list = eval_var_arg(scanf_type, 
				    text_addr(arg_list[2]),
				    next_expr);

	FirstCall = TRUE;
	LastCall = FALSE;

	funct_special = Funct[call_ptr->call_funct_index].funct_special;

	if (((call_ptr->call_expr_count - strlen(types)) <= COMB_COUNT) &&
	    (funct_special != NULL) &&
	    (SideType == SIDE_PITCHER))
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Special\n");
		}
# endif /* TRACE */

		tmp_value = (*funct_special)(arg_list[0],
					     arg_list[1],
					     arg_list[2],
					     var_arg_list);
	}
	else
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Normal\n");
		}
# endif /* TRACE */

		for (guide = var_arg_list; guide ; guide = guide->var_arg_next)
		{
			if (!guide->var_arg_next)
			{
				LastCall = TRUE;
			}

			tmp_value += (*funct_addr)(arg_list[0],
						   arg_list[1],
						   guide->var_arg_fmt,
						   guide->var_arg_arg);

			FirstCall = FALSE;
		}
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_v_arg(var_arg_list);
	free_arg(arg_list, strlen(types));
}

void Fnt0(funct_addr, scanf_type, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		scanf_type;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value = 0;
	char		*types = "t";

	VAR_ARG_ENTRY	*var_arg_list;
	VAR_ARG_ENTRY	*guide;
	TBL_INDEX	next_expr;
	PFI		funct_special;

	arg_list = eval_arg(call_ptr, types, &next_expr);
	var_arg_list = eval_var_arg(scanf_type, 
				    text_addr(arg_list[0]), 
				    next_expr);

	FirstCall = TRUE;
	LastCall = FALSE;

	funct_special = Funct[call_ptr->call_funct_index].funct_special;

	if (((call_ptr->call_expr_count - strlen(types)) <= COMB_COUNT) &&
	    (funct_special != NULL) &&
	    (SideType == SIDE_PITCHER))
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Special\n");
		}
# endif /* TRACE */

		tmp_value = (*funct_special)(arg_list[0], var_arg_list);
	}
	else
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Normal\n");
		}
# endif /* TRACE */

		for (guide = var_arg_list; guide ; guide = guide->var_arg_next)
		{
			if (!guide->var_arg_next)
			{
				LastCall = TRUE;
			}

			tmp_value += (*funct_addr)(guide->var_arg_fmt, 
						   guide->var_arg_arg);

			FirstCall = FALSE;
		}
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_v_arg(var_arg_list);
	free_arg(arg_list, strlen(types));
}

void Fnwnnt0(funct_addr, scanf_type, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		scanf_type;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value = 0;
	char		*types = "wnnt";

	VAR_ARG_ENTRY	*var_arg_list;
	VAR_ARG_ENTRY	*guide;
	TBL_INDEX	next_expr;
	PFI		funct_special;

	arg_list = eval_arg(call_ptr, types, &next_expr);
	var_arg_list = eval_var_arg(scanf_type, 
				    text_addr(arg_list[3]), 
				    next_expr);

	FirstCall = TRUE;
	LastCall = FALSE;

	funct_special = Funct[call_ptr->call_funct_index].funct_special;

	if (((call_ptr->call_expr_count - strlen(types)) <= COMB_COUNT) &&
	    (funct_special != NULL) &&
	    (SideType == SIDE_PITCHER))
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Special\n");
		}
# endif /* TRACE */

		tmp_value = (*funct_special)(arg_list[0], 
					     arg_list[1],
					     arg_list[2],
					     arg_list[3], 
					     var_arg_list);
	}
	else
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Normal\n");
		}
# endif /* TRACE */

		for (guide = var_arg_list; guide; guide = guide->var_arg_next)
		{
			if (!guide->var_arg_next)
			{
				LastCall = TRUE;
			}

			tmp_value += (*funct_addr)(arg_list[0], 
						   arg_list[1],
						   arg_list[2],
						   guide->var_arg_fmt,
						   guide->var_arg_arg);

			FirstCall = FALSE;
		}
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_v_arg(var_arg_list);
	free_arg(arg_list, strlen(types));
}

void Fnwt0(funct_addr, scanf_type, call_ptr, result)
int		(*funct_addr)();
BOOLEAN		scanf_type;
CALL_ENTRY	*call_ptr;
VAR_ENTRY	*result;
{
	VAR_ENTRY	**arg_list;
	int		tmp_value = 0;
	char		*types = "wt";

	VAR_ARG_ENTRY	*var_arg_list;
	VAR_ARG_ENTRY	*guide;
	TBL_INDEX	next_expr;
	PFI		funct_special;

	arg_list = eval_arg(call_ptr, types, &next_expr);
	var_arg_list = eval_var_arg(scanf_type, 
				    text_addr(arg_list[1]), 
				    next_expr);

	FirstCall = TRUE;
	LastCall = FALSE;

	funct_special = Funct[call_ptr->call_funct_index].funct_special;

	if (((call_ptr->call_expr_count - strlen(types)) <= COMB_COUNT) &&
	    (funct_special != NULL) &&
	    (SideType == SIDE_PITCHER))
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Special\n");
		}
# endif /* TRACE */

		tmp_value = (*funct_special)(arg_list[0], 
					     arg_list[1],
					     var_arg_list);
	}
	else
	{

# ifdef TRACE
		if (Trace(20, 0))
		{
			fprintf(stderr, "Normal\n");
		}
# endif /* TRACE */

		for (guide = var_arg_list; guide; guide = guide->var_arg_next)
		{
			if (!guide->var_arg_next)
			{
				LastCall = TRUE;
			}

			tmp_value += (*funct_addr)(arg_list[0], 
						   guide->var_arg_fmt,
						   guide->var_arg_arg);

			FirstCall = FALSE;
		}
	}

	result->var_type = DATA_NUM;
	result->var_data.var_num = tmp_value;

	free_v_arg(var_arg_list);
	free_arg(arg_list, strlen(types));
}
