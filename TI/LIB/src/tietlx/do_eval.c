# include "main.h"
# include "flag.h"
# include "device.h"
# include "collect.h"
# include "protocal.h"
# include "vs.h"
# include "option.h"
# include <exec_head.h>

SCCSID("@(#)do_eval.c	3.1 (SVID) 12/1/86");

TBL_INDEX do_eval(expr_index, result)
TBL_INDEX	expr_index;
VAR_ENTRY	**result;
{
	VAR_ENTRY	*stack[STACK_LEN];
	TBL_INDEX	stack_index;

	EXPR_ENTRY	*expr_ptr;
	TBL_INDEX	old_index;

	stack_index = NIL;
	for (expr_ptr = &Prgm.ExprTbl[expr_index];
	     expr_ptr->expr_type != EXPR_END; 
	     expr_ptr = &Prgm.ExprTbl[expr_index])
	{
		switch (expr_ptr->expr_type)
		{
		  case EXPR_OP:
			eval_op(stack, 
				&stack_index, 
				expr_ptr->expr_data.expr_op);
			if (! short_circuit(&stack[stack_index], &expr_index))
			{
				expr_index++;
			}
			break;

		  case EXPR_CALL:
			old_index = expr_index;
			expr_index = eval_call(stack,
					       &stack_index,
					       expr_ptr->expr_data.expr_call);
			if (short_circuit(&stack[stack_index], &old_index))
			{
				expr_index = old_index;
			}
			break;

		  default:
			push_stack(stack, 
				   &stack_index, 
				   &Prgm.VarTbl[expr_ptr->expr_data.expr_var]);
			if (! short_circuit(&stack[stack_index], &expr_index))
			{
				expr_index++;
			}
		}
	}

	if (stack_index == 0)
	{
		*result = stack[stack_index];
	}
	else if (stack_index == NIL)
	{
		if ((*result = (VAR_ENTRY *) malloc(sizeof(VAR_ENTRY))) == NULL)
		{
			screech("eval_call", 0, "malloc failed\n");
		}
		(*result)->var_class = CLASS_TEMP;
		(*result)->var_type = DATA_NUM;
		(*result)->var_name.name_ptr = NULL;
		(*result)->var_data.var_num = 0;
	} 
	else
	{
		screech("do_eval", 
			0, 
			"\nStack error! stack_index = %d\n", 
			stack_index);
	}


	return(++expr_index);
}

TBL_INDEX eval_call(stack, stack_index_ptr, call_index)
VAR_ENTRY	*stack[];
TBL_INDEX	*stack_index_ptr;
TBL_INDEX	call_index;
{
	CALL_ENTRY	*call_ptr;
	FUNCT_ENTRY	*funct_ptr;
	VAR_ENTRY	*result;

	if ((result = (VAR_ENTRY *) malloc(sizeof(VAR_ENTRY))) == NULL)
	{
		screech("eval_call", 0, "malloc failed\n");
	}
	result->var_class = CLASS_TEMP;
	result->var_name.name_ptr = NULL;

	call_ptr = &Prgm.CallTbl[call_index];
	funct_ptr = &Funct[call_ptr->call_funct_index];

	switch (SideType)
	{
	  case SIDE_PITCHER:
		if (funct_ptr->do_expect)
		{
			send_EOT();
			kickout();
			put_flag(FLAG_PROCEED);
			(void) wait_flag(FLAG_PROCEED);
			put_flag(FLAG_PROCEED);
		}

		if (funct_ptr->funct_p_call)
		{
			if (ExecHead.do_check && Option.hard)
			{
				vs_expecting();
			}
			else
			{
				vs_no_expecting();
			}

			(*(funct_ptr->funct_p_conf))(funct_ptr->funct_p_call, 
						     funct_ptr->front_end,
						     call_ptr, 
						     result);

			vs_no_expecting();
		}
		else
		{
			result->var_type = DATA_NUM;
			result->var_data.var_num = 0;
		}

		if (funct_ptr->do_expect)
		{
			send_EOT();
			kickout();
			put_flag(FLAG_PROCEED);
			(void) wait_flag(FLAG_PROCEED | FLAG_END);
		}
		break;
	
	  case SIDE_CATCHER:
		if (funct_ptr->do_expect)
		{
			put_flag(FLAG_PROCEED);
			(void) collect(FLAG_PROCEED); /* Don't call "judge()" */
			put_flag(FLAG_PROCEED);
			wait_flag(FLAG_PROCEED);
			(void) sleep(1);
			write_kb_buff();
		}

		if (funct_ptr->funct_c_call)
		{

# ifdef TRACE
			if (Trace(10, 0))
			{
				fprintf(stderr, "%s", funct_ptr->funct_name);
			}

# endif /* TRACE */

			(*(funct_ptr->funct_c_conf))(funct_ptr->funct_c_call, 
						     funct_ptr->front_end,
						     call_ptr, 
						     result);
		}
		else
		{
			result->var_type = DATA_NUM;
			result->var_data.var_num = 0;

			if (funct_ptr->do_expect)
			{
				free(KB_Buffer);
				KB_Buffer = NULL;
			}
		}

		if (funct_ptr->do_expect)
		{
			(void) collect(FLAG_PROCEED); /* Don't call "judge()" */
		}
		break;

	  default:
		screech("do_eval", 0, "Bad SideType = %d\n", SideType);
	}

	push_stack(stack, stack_index_ptr, result);

	return(call_ptr->call_expr_next);
}

eval_op(stack, stack_index_ptr, operator)
VAR_ENTRY	*stack[];
TBL_INDEX	*stack_index_ptr;
OPERATOR_TYPE	operator;
{
	VAR_ENTRY	*op_left;
	VAR_ENTRY	*op_right;
	VAR_ENTRY	*op_result;
	VAR_ENTRY	*pop_stack();

	op_right = pop_stack(stack, stack_index_ptr);

	if (operator == OP_ARRAY)
	{
		op_left = pop_stack(stack, stack_index_ptr);
		op_right = convert(op_right, DATA_NUM);

		array_elem(op_left, op_right, &op_result);
	}
	else
	{
		if ((op_result = (VAR_ENTRY *) malloc(sizeof(VAR_ENTRY))) == NULL)
		{
			screech("eval_call", 0, "malloc failed\n");
		}
		op_result->var_class = CLASS_TEMP;
		op_result->var_name.name_ptr = NULL;
		op_result->var_type = DATA_NUM;

		switch(operator)
		{
		  case OP_PREINCR:
		  case OP_POSTINCR:
		  case OP_PREDECR:
		  case OP_POSTDECR:
		  case OP_NOT:
		  case OP_UMINUS:
			op_left = NULL;
			break;

		  default:
			op_left = pop_stack(stack, stack_index_ptr);
			break;
		}

		pre_op(&op_left, &op_right, operator, op_result);
		operate(op_left, op_right, operator, op_result);
	}

	if ((op_left != NULL) && (op_left->var_class == CLASS_TEMP))
	{
		free_text(op_left);
		free_var(op_left);
	}

	if (op_right->var_class == CLASS_TEMP)
	{
		free_text(op_right);
		free_var(op_right);
	}

	push_stack(stack, stack_index_ptr, op_result);
}

array_elem(op_left, op_right, op_result)
VAR_ENTRY	*op_left;
VAR_ENTRY	*op_right;
VAR_ENTRY	**op_result;
{
	int		array_index;
	VAR_ENTRY	*array_var;
	unsigned	byte_size;

	int		old_size;
	int		new_size;
	int		i;

	array_index = op_right->var_data.var_num;
	old_size = op_left->var_data.var_array.array_size;
	array_var = op_left->var_data.var_array.array_var;

	if (old_size == 0)
	{
		new_size = array_index + 1;
		byte_size = sizeof(VAR_ENTRY) * new_size;
		if ((array_var = (VAR_ENTRY *) malloc(byte_size)) == NULL)
		{
			screech("array_elem", 0, "malloc failed\n");
		}
	}
	else if (array_index + 1 > old_size)
	{
		new_size = array_index + 1;
		byte_size = sizeof(VAR_ENTRY) * new_size;
		if ((array_var = (VAR_ENTRY *) realloc(array_var, byte_size)) == NULL)
		{
			screech("array_elem", 0, "realloc failed\n");
		}
	}
	else
	{
		new_size = old_size;
	}

	for (i = old_size; i < new_size; i++)
	{
		array_var[i].var_class = CLASS_VAR;
		array_var[i].var_type = DATA_NUM;
		array_var[i].var_name.name_ptr = NULL;
		array_var[i].var_data.var_num = 0;
	}

	op_left->var_data.var_array.array_var = array_var;
	op_left->var_data.var_array.array_size = new_size;

	*op_result = &(op_left->var_data.var_array.array_var[array_index]);
}

pre_op(op_left, op_right, operator, op_result)
VAR_ENTRY	**op_left;
VAR_ENTRY	**op_right;
OPERATOR_TYPE	operator;
VAR_ENTRY	*op_result;
{
	DATA_TYPE	conform_type;
	DATA_TYPE 	compare_as();

	switch (operator)
	{
	  case OP_ADD:
	  case OP_SUB:
	  case OP_DIV:
	  case OP_MULT:
	  case OP_MOD:
	  case OP_BIT_OR:
	  case OP_BIT_AND:
	  case OP_UMINUS:
		op_result->var_type = DATA_NUM;
		conform_type = DATA_NUM;
		break;

	  case OP_CAT:
		op_result->var_type = DATA_TEXT;
		conform_type = DATA_TEXT;
		op_result->var_data.var_text_ptr = "";
		break;

	  case OP_NOT:
	  case OP_AND:
	  case OP_OR:
		op_result->var_type = DATA_BOOL;
		conform_type = DATA_BOOL;
		break;

	  case OP_PREINCR:
	  case OP_POSTINCR:
	  case OP_PREDECR:
	  case OP_POSTDECR:
		if ((*op_right)->var_class == CLASS_CONST)
		{
			screech("pre_op", 0, "Writing to read_only variable\n");
		}
		op_result->var_type = DATA_NULL;
		conform_type = DATA_NULL;
		break;

	  case OP_ADDASSIGN:
	  case OP_SUBASSIGN:
	  case OP_MULTASSIGN:
	  case OP_DIVASSIGN:
	  case OP_MODASSIGN:
	  case OP_ASSIGN:
		if ((*op_left)->var_class == CLASS_CONST)
		{
			screech("pre_op", 0, "Writing to read_only variable\n");
		}
		op_result->var_type = DATA_NULL;
		conform_type = DATA_NULL;
		break;

	  default:
		op_result->var_type = DATA_BOOL;
		op_result->var_data.var_bool = FALSE;
		conform_type = compare_as(*op_left, *op_right);
	}

	if (*op_left != NULL)
	{
		*op_left = convert(*op_left, conform_type);
	}
	*op_right = convert(*op_right, conform_type);
}

DATA_TYPE compare_as(vp1, vp2)
VAR_ENTRY	*vp1;
VAR_ENTRY	*vp2;
{
	NUMBER_TYPE	junk = 0;

	if (vp1->var_type == vp2->var_type)
	{
		return(vp1->var_type);
	}

	if (vp1->var_type == DATA_TEXT)
	{
		if ((! text_num(text_addr(vp1), &junk)) || 
		    (vp2->var_type == DATA_CHAR))
		{
			return(DATA_TEXT);
		}
	}

	if (vp2->var_type == DATA_TEXT)
	{
		if ((! text_num(text_addr(vp2), &junk)) || 
		    (vp1->var_type == DATA_CHAR))
		{
			return(DATA_TEXT);
		}
	}

	if ((vp1->var_type == DATA_WINDOW) || (vp2->var_type == DATA_WINDOW))
	{
		return(DATA_WINDOW);
	}

	if ((vp1->var_type == DATA_SCREEN) || (vp2->var_type == DATA_SCREEN))
	{
		return(DATA_SCREEN);
	}

	return(DATA_NUM);
}

BOOLEAN short_circuit(stack_top, expr_index)
VAR_ENTRY	**stack_top;
TBL_INDEX	*expr_index;
{
	TBL_INDEX	short_circuit_index;
	OPERATOR_TYPE	op;
	BOOLEAN		result;

	result = FALSE;
	short_circuit_index = Prgm.ExprTbl[*expr_index].expr_short_circuit;

	if (short_circuit_index != NIL)
	{
		op = Prgm.ExprTbl[short_circuit_index].expr_data.expr_op;

		*stack_top = convert(*stack_top, DATA_BOOL);

		if ((  (*stack_top)->var_data.var_bool) && (op == OP_OR) ||
		    (! (*stack_top)->var_data.var_bool) && (op == OP_AND))
		{
			result = TRUE;
			*expr_index = short_circuit_index + 1;
		}
	}

	return(result);
}

push_stack(stack, stack_index_ptr, var_ptr)
VAR_ENTRY	*stack[];
TBL_INDEX	*stack_index_ptr;
VAR_ENTRY	*var_ptr;
{
	if ((*stack_index_ptr) + 1 == STACK_LEN)
	{
		screech("push_stack", 0, "Stack overflow\n");
	}

	*stack_index_ptr = *stack_index_ptr + 1;
	stack[*stack_index_ptr] = var_ptr;
}

VAR_ENTRY *pop_stack(stack, stack_index)
VAR_ENTRY	*stack[];
TBL_INDEX	*stack_index;
{
	VAR_ENTRY	*result;

	if (*stack_index == NIL)
	{
		screech("pop_stack", 0, "Stack underflow\n");
	}

	result = stack[*stack_index];
	*stack_index = *stack_index - 1;

	return(result);
}
