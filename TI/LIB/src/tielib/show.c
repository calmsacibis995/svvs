# include <bless.h>
# include <parse.h>
# include <funct_def.h>

extern FUNCT_ENTRY	Funct[];

SCCSID("@(#)show.c	3.1 (SVID) 12/1/86");

static int	IndentVal = 0;

void shw_op();
void shw_var();

void var_num_show();
void var_bool_show();
void var_chtype_show();
void var_text_show();
void var_window_show();
void var_screen_show();
void var_term_show();
void var_array_show();

void shw_side_type();

void stmt_assign_show();
void stmt_cmp_show();
void stmt_loop_show();
void stmt_goto_show();
void stmt_block_show();

void shw_size(base_name)
char	*base_name;
{
	FILE	*fp;
	char	*file_name[1000];

	(void) sprintf(file_name, "%s.%d", base_name, getpid());

	if (fp = fopen(file_name, "a"))
	{
		(void) fprintf(fp, "Head %d\n", Prgm.HeadStat.next_free);
		(void) fprintf(fp, "Expr %d\n", Prgm.ExprStat.next_free);
		(void) fprintf(fp, "Var  %d\n", Prgm.VarStat.next_free);
		(void) fprintf(fp, "Call %d\n", Prgm.CallStat.next_free);
		(void) fprintf(fp, "Name %d\n", Prgm.NameStat.next_free);
		(void) fprintf(fp, "Stmt %d\n", Prgm.StmtStat.next_free);
		(void) fprintf(fp, "Text %d\n", Prgm.TextStat.next_free);

		if (fclose(fp))
		{
			screech("shw_size", 0, "Can't close report file\n");
		}
	}
}

TBL_INDEX shw_expr(expr_index)
TBL_INDEX	expr_index;
{
	EXPR_ENTRY	*expr_ptr;
	TBL_INDEX	tmp_index;

	if (expr_index == NIL)
	{
		return(NIL);
	}

# ifdef EXPRDETAIL
	(void) fprintf(stderr, "\n");
# endif

	for (expr_ptr = &Prgm.ExprTbl[expr_index];
	     expr_ptr->expr_type != EXPR_END;
	     expr_index++, expr_ptr = &Prgm.ExprTbl[expr_index])
	{
		switch (expr_ptr->expr_type)
		{
		  case EXPR_OP:

# ifdef EXPRDETAIL
			(void) fprintf(stderr, "%16d  %d  ", 
				      expr_index, 
				      expr_ptr->expr_short_circuit);
# endif

			shw_op(expr_ptr->expr_data.expr_op);
			break;

		  case EXPR_VAR:

# ifdef EXPRDETAIL
			(void) fprintf(stderr, "%16d  %d  ", 
				       expr_index, 
				       expr_ptr->expr_short_circuit);
# endif

			shw_var(expr_ptr->expr_data.expr_var, 
				 (VAR_ENTRY *) NULL);
			break;

		  case EXPR_CALL:

# ifdef EXPRDETAIL
			(void) fprintf(stderr, "%16d  %d  ", 
				      expr_index, 
				      expr_ptr->expr_short_circuit);
# endif

			tmp_index = shw_call(expr_ptr->expr_data.expr_call,
					      (CALL_ENTRY *) NULL);

			if (tmp_index != NIL)
			{
				expr_index = tmp_index - 1;
				expr_ptr = &Prgm.ExprTbl[expr_index];
			}
			break;

		  default:
			screech("shw_expr", 0, 
				"Bad expr type %c\n", expr_ptr->expr_type);
		}

# ifdef EXPRDETAIL
		(void) fprintf(stderr, "\n");
# else
	     	if (expr_ptr->expr_type != EXPR_END)
		{
			(void) fprintf(stderr, " ");
		}
# endif

	}
	return(++expr_index);
}

void shw_op(operator)
OPERATOR_TYPE	operator;
{
	switch (operator)
	{
	  case OP_BIT_OR:
		(void) fprintf(stderr, "|");
		break;

	  case OP_BIT_AND:
		(void) fprintf(stderr, "&");
		break;

	  case OP_OR:
		(void) fprintf(stderr, "||");
		break;

	  case OP_AND:
		(void) fprintf(stderr, "&&");
		break;

	  case OP_NE:
		(void) fprintf(stderr, "!=");
		break;

	  case OP_EQ:
		(void) fprintf(stderr, "==");
		break;

	  case OP_GT:
		(void) fprintf(stderr, ">");
		break;

	  case OP_LT:
		(void) fprintf(stderr, "<");
		break;

	  case OP_GE:
		(void) fprintf(stderr, ">=");
		break;

	  case OP_LE:
		(void) fprintf(stderr, "<=");
		break;

	  case OP_UMINUS:
		(void) fprintf(stderr, "-");
		break;

	  case OP_PREINCR:
		(void) fprintf(stderr, "++.");
		break;

	  case OP_POSTINCR:
		(void) fprintf(stderr, ".++");
		break;

	  case OP_PREDECR:
		(void) fprintf(stderr, "--.");
		break;

	  case OP_POSTDECR:
		(void) fprintf(stderr, ".--");
		break;

	  case OP_ASSIGN:
		(void) fprintf(stderr, "=");
		break;

	  case OP_ADDASSIGN:
		(void) fprintf(stderr, "+=");
		break;

	  case OP_SUBASSIGN:
		(void) fprintf(stderr, "-=");
		break;

	  case OP_MULTASSIGN:
		(void) fprintf(stderr, "*=");
		break;

	  case OP_DIVASSIGN:
		(void) fprintf(stderr, "/=");
		break;

	  case OP_MODASSIGN:
		(void) fprintf(stderr, "%=");
		break;

	  case OP_ADD:
		(void) fprintf(stderr, "+");
		break;

	  case OP_SUB:
		(void) fprintf(stderr, "-");
		break;

	  case OP_MULT:
		(void) fprintf(stderr, "*");
		break;

	  case OP_DIV:
		(void) fprintf(stderr, "/");
		break;

	  case OP_MOD:
		(void) fprintf(stderr, "%");
		break;

	  case OP_CAT:
		(void) fprintf(stderr, ":");
		break;

	  case OP_ARRAY:
		(void) fprintf(stderr, "[");
		break;

	  default:
		screech("shw_op", 0, "op code = %d\n", operator);
	}
}

void shw_class(var_index, var_ptr)
TBL_INDEX	var_index;
VAR_ENTRY	*var_ptr;
{
	if (var_index != NIL)
	{
		var_ptr = &Prgm.VarTbl[var_index];
	}

	switch (var_ptr->var_class)
	{
	  case CLASS_CONST:
		(void) fprintf(stderr, "CONST");
		break;

	  case CLASS_VAR:
		(void) fprintf(stderr, "VAR");
		break;

	  case CLASS_TEMP:
		(void) fprintf(stderr, "TEMP");
		break;

	  default:
		(void) fprintf(stderr, "???");
	}
}

void shw_var(var_index, var_ptr)
TBL_INDEX	var_index;
VAR_ENTRY	*var_ptr;
{
	if (var_index != NIL)
	{
		var_ptr = &Prgm.VarTbl[var_index];
	}

	switch (var_ptr->var_class)
	{
	  case CLASS_CONST:
		if (var_ptr->var_name.name_ptr != NULL)
		{
			(void) fprintf(stderr, 
				       "%s[",
				       var_ptr->var_name.name_ptr + 1);
		}
		else
		{
			(void) fprintf(stderr, "[");
		}
		break;

	  case CLASS_VAR:
		if (var_ptr->var_name.name_ptr != NULL)
		{
			(void) fprintf(stderr, 
				       "%s<",
				       var_ptr->var_name.name_ptr + 1);
		}
		else
		{
			(void) fprintf(stderr, "???<");
		}
		break;

	  case CLASS_TEMP:
		(void) fprintf(stderr, "{");
		break;

	  default:
		screech("shw_var", 0, "Bad class %d\n", var_ptr->var_class);
	}

	switch(var_ptr->var_type)
	{
	  case DATA_NUM:
	  case DATA_NULL:
		var_num_show(var_ptr);
		break;

	  case DATA_BOOL:
		var_bool_show(var_ptr);
		break;

	  case DATA_CHAR:
		var_chtype_show(var_ptr);
		break;

	  case DATA_TEXT:
		var_text_show(var_ptr);
		break;

	  case DATA_WINDOW:
		var_window_show(var_ptr);
		break;

	  case DATA_SCREEN:
		var_screen_show(var_ptr);
		break;

	  case DATA_TERM:
		var_term_show(var_ptr);
		break;

	  case DATA_ARRAY:
		var_array_show(var_ptr);
		break;

	  default:
		(void) fprintf(stderr, "shw_var: Bad type = '%c'\n", var_ptr->var_type);
	}

	switch (var_ptr->var_class)
	{
	  case CLASS_CONST:
		(void) fprintf(stderr, "]");
		break;

	  case CLASS_TEMP:
		(void) fprintf(stderr, "}");
		break;

	  case CLASS_VAR:
		(void) fprintf(stderr, ">");
	}

	switch(var_ptr->var_type)
	{
	  case DATA_NUM:
		(void) fprintf(stderr, ":N");
		break;

	  case DATA_BOOL:
		(void) fprintf(stderr, ":B");
		break;

	  case DATA_CHAR:
		(void) fprintf(stderr, ":C");
		break;

	  case DATA_TEXT:
		(void) fprintf(stderr, ":T");
		break;

	  case DATA_NULL:
		(void) fprintf(stderr, ":?");
		break;

	  case DATA_WINDOW:
		(void) fprintf(stderr, ":W");
		break;

	  case DATA_SCREEN:
		(void) fprintf(stderr, ":S");
		break;

	  case DATA_TERM:
		(void) fprintf(stderr, ":M");
		break;

	  case DATA_ARRAY:
		(void) fprintf(stderr, ":A");
		break;

	  default:
		(void) fprintf(stderr, "shw_var: Bad type = '%c'\n", var_ptr->var_type);
	}
}

TBL_INDEX shw_call(call_index, call_ptr)
TBL_INDEX	call_index;
CALL_ENTRY	*call_ptr;
{
	char		*name_ptr;
	TBL_INDEX	expr_index;
	int		arg_count;
	int		arg_index;

	if (call_index != NIL)
	{
		call_ptr = &Prgm.CallTbl[call_index];
	}

	name_ptr = Funct[call_ptr->call_funct_index].funct_name;
	(void) fprintf(stderr, "%s(", name_ptr);

	expr_index = call_ptr->call_expr;
	arg_count = call_ptr->call_expr_count;

	for (arg_index = 0; arg_index < arg_count; arg_index++)
	{
		expr_index = shw_expr(expr_index);
		if (arg_index + 1 < arg_count)
		{
			(void) fprintf(stderr, ", ");
		}
	}
	(void) fprintf(stderr, ")");

	return(expr_index);
}

void var_num_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) fprintf(stderr, "%d", var_ptr->var_data.var_num);
}

void var_bool_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) fprintf(stderr, "%s", var_ptr->var_data.var_bool ? "TRUE" : "FALSE");
}

void var_chtype_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) cfprintf(stderr, "'%c'", var_ptr->var_data.var_chtype);
}

void var_text_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) cfprintf(stderr, "\"%s\"", var_ptr->var_data.var_text_ptr);
}

void var_window_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) fprintf(stderr, "%x", var_ptr->var_data.var_window);
}

void var_screen_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) fprintf(stderr, "%x", var_ptr->var_data.var_screen);
}

void var_term_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) fprintf(stderr, "%x", var_ptr->var_data.var_term);
}

void var_array_show(var_ptr)
VAR_ENTRY	*var_ptr;
{
	(void) fprintf(stderr, "[%d]", var_ptr->var_data.var_array.array_size);
}

void shw_margin(stmt_index)
TBL_INDEX	stmt_index;
{
	int	indent_count;

	shw_side_type(stmt_index);
	(void) fprintf(stderr, "%5d  ", stmt_index);
	for (indent_count = 0; indent_count < IndentVal; indent_count++)
	{
		(void) fprintf(stderr, "        ");
	}
}

void shw_side_type(stmt_index)
TBL_INDEX	stmt_index;
{
	STMT_ENTRY	*stmt_ptr;

	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	switch (stmt_ptr->stmt_side_type)
	{
	  case SIDE_NULL:
		(void) fprintf(stderr, " ");
		break;

	  case SIDE_PITCHER:
		(void) fprintf(stderr, "<");
		break;

	  case SIDE_CATCHER:
		(void) fprintf(stderr, ">");
		break;

	  default:
		(void) fprintf(stderr, "shw_side_type: Bad type = %d\n", 
		       stmt_ptr->stmt_side_type);
	}
}

void shw_stmt()
{
	TBL_INDEX	stmt_index;
	STMT_ENTRY	*stmt_ptr;

	for (stmt_index = 0, stmt_ptr = &Prgm.StmtTbl[stmt_index];
	     stmt_index < Prgm.StmtStat.next_free; 
	     stmt_index++, stmt_ptr++)
	{
		switch (stmt_ptr->stmt_type)
		{
		  case STMT_ASSIGN:
			shw_margin(stmt_index);

			stmt_assign_show(stmt_ptr);
			break;

		  case STMT_COMPARE:
			shw_margin(stmt_index);
			IndentVal++;

			stmt_cmp_show(stmt_ptr);
			break;

		  case STMT_CONTINUE:
			IndentVal--;
			shw_margin(stmt_index);

			(void) fprintf(stderr, "continue\n");
			break;

		  case STMT_CONT_LOOP:
			IndentVal--;
			shw_margin(stmt_index);

			(void) fprintf(stderr, "continue-loop\n");
			break;

		  case STMT_BREAK:
			shw_margin(stmt_index);

			(void) fprintf(stderr, "break\n");
			break;

		  case STMT_EXIT:
			shw_margin(stmt_index);

			(void) fprintf(stderr, "exit\n");
			break;

		  case STMT_LOOP:
			shw_margin(stmt_index);
			IndentVal++;

			stmt_loop_show(stmt_ptr);
			break;

		  case STMT_GOTO:
			IndentVal--;
			shw_margin(stmt_index);

			stmt_goto_show(stmt_ptr);
			break;

		  case STMT_GOTO_ELSE:
			IndentVal--;
			shw_margin(stmt_index);
			IndentVal++;
			
			stmt_goto_show(stmt_ptr);
			break;

		  case STMT_START:
			shw_margin(stmt_index);
			IndentVal++;

			stmt_block_show(stmt_ptr);
			break;

		  case STMT_FINIS:
			IndentVal--;
			shw_margin(stmt_index);

			fprintf(stderr, "finis\n");
			break;

		  default:
			(void) fprintf(stderr, 
				       "Bad stmt type = '%c'\n", 
				       stmt_ptr->stmt_type);
		}
	}
}

void stmt_assign_show(stmt_ptr)
STMT_ENTRY	*stmt_ptr;
{
	TBL_INDEX	expr_index;

	expr_index = stmt_ptr->stmt_data.stmt_assign;
	(void) shw_expr(expr_index);

	(void) fprintf(stderr, "\n");
}

void stmt_cmp_show(stmt_ptr)
STMT_ENTRY	*stmt_ptr;
{
	TBL_INDEX	stmt_index;
	TBL_INDEX	expr_index;

	stmt_index = stmt_ptr->stmt_data.stmt_cmp.cmp_branch;
	expr_index = stmt_ptr->stmt_data.stmt_cmp.cmp_expr;

	(void) fprintf(stderr, "GOTO %d IF NOT ", stmt_index);

	(void) shw_expr(expr_index);

	(void) fprintf(stderr, "\n");
}

void stmt_loop_show(stmt_ptr)
STMT_ENTRY	*stmt_ptr;
{
	TBL_INDEX	stmt_index;
	TBL_INDEX	expr_index;

	stmt_index = stmt_ptr->stmt_data.stmt_loop.loop_branch;
	expr_index = stmt_ptr->stmt_data.stmt_loop.loop_expr;

	if (expr_index == NIL)
	{
		(void) fprintf(stderr, "NEVER GOTO %d ", stmt_index);
	}
	else
	{
		(void) fprintf(stderr, "GOTO %d IF NOT ", stmt_index);
	}
	(void) shw_expr(expr_index);

	expr_index = stmt_ptr->stmt_data.stmt_loop.loop_for3;
	if (expr_index != NIL)
	{
		(void) fprintf(stderr, "FOR3 CLAUSE ");
		(void) shw_expr(expr_index);
	}

	(void) fprintf(stderr, "\n");
}

void stmt_goto_show(stmt_ptr)
STMT_ENTRY *stmt_ptr;
{
	(void) fprintf(stderr, "GOTO %d\n", stmt_ptr->stmt_data.stmt_goto);
}

void stmt_block_show(stmt_ptr)
STMT_ENTRY *stmt_ptr;
{
	(void) fprintf(stderr, 
		       "start - ABORT to %d\n", stmt_ptr->stmt_data.stmt_block);
}

void shw_v_table()
{
	TBL_INDEX	var_index;

	for (var_index = 0; var_index < Prgm.VarStat.next_free; var_index++)
	{
		if (Prgm.VarTbl[var_index].var_class == CLASS_VAR)
		{
			shw_var(var_index, (VAR_ENTRY *) NULL);
			(void) fprintf(stderr, "\n");
		}
	}

	for (var_index = 0; var_index < Prgm.VarStat.next_free; var_index++)
	{
		if (Prgm.VarTbl[var_index].var_class == CLASS_CONST)
		{
			shw_var(var_index, (VAR_ENTRY *) NULL);
			(void) fprintf(stderr, "\n");
		}
	}
}

void show_c_table()
{
	TBL_INDEX	call_index;

	for (call_index = 0; 
	     call_index < Prgm.CallStat.next_free; 
	     call_index++)
	{
		(void) shw_call(call_index, (CALL_ENTRY *) NULL);
		(void) fprintf(stderr, "\n");
	}
}

void shw_tuple(op_left, op_right, operator, op_result)
VAR_ENTRY	*op_left;
VAR_ENTRY	*op_right;
OPERATOR_TYPE	operator;
VAR_ENTRY	*op_result;
{
	shw_var(NIL, op_left);
	(void) fprintf(stderr, " ");
	shw_op(operator);
	(void) fprintf(stderr, " ");
	shw_var(NIL, op_right);
	(void) fprintf(stderr, " ; ");
	shw_var(NIL, op_result);
	(void) fprintf(stderr, "\n");
}
