# include "main.h"

SCCSID("@(#)operate.c	3.1 (SVID) 12/1/86");

operate(op_left, op_right, operator, op_result)
VAR_ENTRY	*op_left;
VAR_ENTRY	*op_right;
OPERATOR_TYPE	operator;
VAR_ENTRY	*op_result;
{
	switch (operator)
	{
	  case OP_PREINCR:
	  case OP_PREDECR:
	  case OP_POSTINCR:
	  case OP_POSTDECR:
		op_incr_decr(op_right, operator, op_result);
		break;

	  case OP_UMINUS:
		op_result->var_data.var_num = op_right->var_data.var_num * -1;
		break;

	  case OP_NOT:
		op_result->var_data.var_bool = ! op_right->var_data.var_bool;
		break;

	  case OP_CAT:
		op_text(text_addr(op_left),
			text_addr(op_right),
			&op_result->var_data.var_text_ptr);
		break;

	  case OP_ADDASSIGN:
	  case OP_SUBASSIGN:
	  case OP_MULTASSIGN:
	  case OP_DIVASSIGN:
	  case OP_MODASSIGN:
		op_op_assign(op_left, op_right, operator, op_result);
		break;

	  case OP_ASSIGN:
		op_assign(op_left, op_right, op_result);
		break;

	  case OP_LT:
	  case OP_GT:
	  case OP_LE:
	  case OP_GE:
	  case OP_NE:
	  case OP_EQ:
	  case OP_AND:
	  case OP_OR:
		op_bool(op_left, op_right, operator, op_result);
		break;

	  default:
		op_num(&op_left->var_data.var_num,
		       &op_right->var_data.var_num,
		       operator,
		       &op_result->var_data.var_num);
	}
}

op_incr_decr(operand, operator, op_result)
VAR_ENTRY	*operand;
OPERATOR_TYPE	operator;
VAR_ENTRY	*op_result;
{
	VAR_ENTRY	*tmp_var;

/*
** Watch out for "convert()".  If no conversion is necessary, convert will
** simply return "operand".  Therefore an operation like 
** "operand.blah = tmp.blah + 1" could change "tmp.blah".
*/

	tmp_var = convert(operand, DATA_NUM);

	free_text(operand);

	op_result->var_type = DATA_NUM;
	operand->var_type = DATA_NUM;

	op_result->var_data.var_num = tmp_var->var_data.var_num;
	operand->var_data.var_num = tmp_var->var_data.var_num;

	if ((operator == OP_PREINCR) || (operator == OP_POSTINCR))
	{
		if (operator == OP_PREINCR)
		{
			op_result->var_data.var_num++;
		}
		operand->var_data.var_num++;
	}
	else
	{
		if (operator == OP_PREDECR)
		{
			op_result->var_data.var_num--;
		}
		operand->var_data.var_num--;
	}

	free_text(tmp_var);
	free_var(tmp_var);
}

op_num(op_left, op_right, operator, op_result)
NUMBER_TYPE	*op_left;
NUMBER_TYPE	*op_right;
OPERATOR_TYPE	operator;
NUMBER_TYPE	*op_result;
{
	switch (operator)
	{
	  case OP_ADD:
	  case OP_ADDASSIGN:
		*op_result = *op_left + *op_right;
		break;

	  case OP_SUB:
	  case OP_SUBASSIGN:
		*op_result = *op_left - *op_right;
		break;

	  case OP_DIV:
	  case OP_DIVASSIGN:
		*op_result = *op_left / *op_right;
		break;

	  case OP_MULT:
	  case OP_MULTASSIGN:
		*op_result = *op_left * *op_right;
		break;

	  case OP_MOD:
	  case OP_MODASSIGN:
		*op_result = *op_left % *op_right;
		break;

	  case OP_BIT_AND:
		*op_result = *op_left & *op_right;
		break;

	  case OP_BIT_OR:
		*op_result = *op_left | *op_right;
		break;

	  default:
		screech("op_num", 0, "Bad operator = %d\n", operator);
	}
}

op_text(op_left, op_right, op_result)
TEXT_ENTRY	*op_left;
TEXT_ENTRY	*op_right;
TEXT_ENTRY	**op_result;
{
	unsigned	total_len;

	total_len = strlen(op_left) + strlen(op_right) + 1;

	if ((*op_result = malloc(total_len)) == NULL)
	{
		screech("op_text", 0, "malloc failed\n");
	}

	(void) strcpy(*op_result, op_left);
	(void) strcat(*op_result, op_right);
}

op_assign(op_left, op_right, op_result)
VAR_ENTRY	*op_left;
VAR_ENTRY	*op_right;
VAR_ENTRY	*op_result;
{
	free_text(op_left);

	op_result->var_type = op_right->var_type;
	op_left->var_type = op_right->var_type;

	if (op_right->var_type == DATA_TEXT)
	{
		copy_text(op_result, op_right);
		copy_text(op_left, op_right);
	}
	else
	{
		op_result->var_data = op_right->var_data;
		op_left->var_data = op_right->var_data;
	}
}

op_op_assign(op_left, op_right, operator, op_result)
VAR_ENTRY	*op_left;
VAR_ENTRY	*op_right;
OPERATOR_TYPE	operator;
VAR_ENTRY	*op_result;
{
	VAR_ENTRY	*tmp_var;

	tmp_var = convert(op_left, DATA_NUM);
	op_right = convert(op_right, DATA_NUM);

	free_text(op_left);

	op_left->var_type = DATA_NUM;
	op_num(&tmp_var->var_data.var_num,
	       &op_right->var_data.var_num,
	       operator,
	       &op_left->var_data.var_num);

	op_result->var_type = DATA_NUM;
	op_result->var_data = op_left->var_data;

	free_var(tmp_var);
}

copy_text(dest_ptr, src_ptr)
VAR_ENTRY	*dest_ptr;
VAR_ENTRY	*src_ptr;
{
	TEXT_ENTRY	*tmp_text_ptr;

	tmp_text_ptr = text_addr(src_ptr);

	dest_ptr->var_data.var_text_ptr = 
		malloc((unsigned) (strlen(tmp_text_ptr) + 1));

	if (dest_ptr->var_data.var_text_ptr == NULL)
	{
		screech("copy_text", 0, "malloc failed\n");
	}

	(void) strcpy(dest_ptr->var_data.var_text_ptr, tmp_text_ptr);
}

op_bool(op_left, op_right, operator, op_result)
VAR_ENTRY	*op_left;
VAR_ENTRY	*op_right;
OPERATOR_TYPE	operator;
VAR_ENTRY	*op_result;
{
	switch (op_left->var_type)
	{
	  case DATA_NUM:
		bool_num(&op_left->var_data.var_num,
			 &op_right->var_data.var_num, 
			 operator, 
			 &op_result->var_data.var_bool);
		break;

	  case DATA_BOOL:
		bool_bool(&op_left->var_data.var_bool,
			  &op_right->var_data.var_bool, 
			  operator, 
			  &op_result->var_data.var_bool);
		break;

	  case DATA_CHAR:
		bool_char(&op_left->var_data.var_chtype,
			  &op_right->var_data.var_chtype, 
			  operator, 
			  &op_result->var_data.var_bool);
		break;

	  case DATA_TEXT:
		bool_text(text_addr(op_left),
			  text_addr(op_right),
			  operator, 
			  &op_result->var_data.var_bool);
		break;

	  case DATA_WINDOW:
		bool_window(op_left->var_data.var_window,
			    op_right->var_data.var_window, 
			    operator, 
			    &op_result->var_data.var_bool);
		break;

	  case DATA_SCREEN:
		bool_screen(op_left->var_data.var_screen,
			    op_right->var_data.var_screen, 
			    operator, 
			    &op_result->var_data.var_bool);
		break;

	  default:
		screech("eval_bool", 0, "Bad type '%c'\n", op_left->var_type);
	}
}

bool_num(op_left, op_right, operator, op_result)
NUMBER_TYPE	*op_left;
NUMBER_TYPE	*op_right;
OPERATOR_TYPE	operator;
BOOLEAN		*op_result;
{
	switch (operator)
	{
	  case OP_LT:
		*op_result = *op_left < *op_right;
		break;

	  case OP_GT:
		*op_result = *op_left > *op_right;
		break;

	  case OP_LE:
		*op_result = *op_left <= *op_right;
		break;

	  case OP_GE:
		*op_result = *op_left >= *op_right;
		break;

	  case OP_EQ:
		*op_result = *op_left == *op_right;
		break;

	  case OP_NE:
		*op_result = *op_left != *op_right;
		break;

	  case OP_OR:
		*op_result = *op_left || *op_right;
		break;

	  case OP_AND:
		*op_result = *op_left && *op_right;
		break;

	  default:
		break;
	}
}

bool_bool(op_left, op_right, operator, op_result)
BOOLEAN		*op_left;
BOOLEAN		*op_right;
OPERATOR_TYPE	operator;
BOOLEAN		*op_result;
{
	switch (operator)
	{
	  case OP_LT:
		*op_result = *op_left < *op_right;
		break;

	  case OP_GT:
		*op_result = *op_left > *op_right;
		break;

	  case OP_LE:
		*op_result = *op_left <= *op_right;
		break;

	  case OP_GE:
		*op_result = *op_left >= *op_right;
		break;

	  case OP_EQ:
		*op_result = *op_left == *op_right;
		break;

	  case OP_NE:
		*op_result = *op_left != *op_right;
		break;

	  case OP_OR:
		*op_result = *op_left || *op_right;
		break;

	  case OP_AND:
		*op_result = *op_left && *op_right;
		break;

	  default:
		break;
	}
}

bool_char(op_left, op_right, operator, op_result)
chtype		*op_left;
chtype		*op_right;
OPERATOR_TYPE	operator;
BOOLEAN		*op_result;
{
	switch (operator)
	{
	  case OP_LT:
		*op_result = *op_left < *op_right;
		break;

	  case OP_GT:
		*op_result = *op_left > *op_right;
		break;

	  case OP_LE:
		*op_result = *op_left <= *op_right;
		break;

	  case OP_GE:
		*op_result = *op_left >= *op_right;
		break;

	  case OP_EQ:
		*op_result = *op_left == *op_right;
		break;

	  case OP_NE:
		*op_result = *op_left != *op_right;
		break;

	  case OP_OR:
		*op_result = *op_left || *op_right;
		break;

	  case OP_AND:
		*op_result = *op_left && *op_right;
		break;

	  default:
		break;
	}
}

bool_text(op_left, op_right, operator, op_result)
TEXT_ENTRY	*op_left;
TEXT_ENTRY	*op_right;
OPERATOR_TYPE	operator;
BOOLEAN		*op_result;
{
	switch (operator)
	{
	  case OP_LT:
		*op_result = strcmp(op_left, op_right) < 0;
		break;

	  case OP_GT:
		*op_result = strcmp(op_left, op_right) > 0;
		break;

	  case OP_LE:
		*op_result = strcmp(op_left, op_right) <= 0;
		break;

	  case OP_GE:
		*op_result = strcmp(op_left, op_right) >= 0;
		break;

	  case OP_EQ:
		*op_result = strcmp(op_left, op_right) == 0;
		break;

	  case OP_NE:
		*op_result = strcmp(op_left, op_right) != 0;
		break;

	  case OP_OR:
		*op_result = strlen(op_left) || strlen(op_right);
		break;

	  case OP_AND:
		*op_result = strlen(op_left) && strlen(op_right);
		break;

	  default:
		break;
	}
}

bool_window(op_left, op_right, operator, op_result)
WINDOW		*op_left;
WINDOW		*op_right;
OPERATOR_TYPE	operator;
BOOLEAN		*op_result;
{
	switch (operator)
	{
	  case OP_LT:
		*op_result = op_left < op_right;
		break;

	  case OP_GT:
		*op_result = op_left > op_right;
		break;

	  case OP_LE:
		*op_result = op_left <= op_right;
		break;

	  case OP_GE:
		*op_result = op_left >= op_right;
		break;

	  case OP_EQ:
		*op_result = op_left == op_right;
		break;

	  case OP_NE:
		*op_result = op_left != op_right;
		break;

	  case OP_OR:
		*op_result = op_left || op_right;
		break;

	  case OP_AND:
		*op_result = op_left && op_right;
		break;

	  default:
		break;
	}
}

bool_screen(op_left, op_right, operator, op_result)
SCREEN		*op_left;
SCREEN		*op_right;
OPERATOR_TYPE	operator;
BOOLEAN		*op_result;
{
	switch (operator)
	{
	  case OP_LT:
		*op_result = op_left < op_right;
		break;

	  case OP_GT:
		*op_result = op_left > op_right;
		break;

	  case OP_LE:
		*op_result = op_left <= op_right;
		break;

	  case OP_GE:
		*op_result = op_left >= op_right;
		break;

	  case OP_EQ:
		*op_result = op_left == op_right;
		break;

	  case OP_NE:
		*op_result = op_left != op_right;
		break;

	  case OP_OR:
		*op_result = op_left || op_right;
		break;

	  case OP_AND:
		*op_result = op_left && op_right;
		break;

	  default:
		break;
	}
}
