# include "main.h"
# include <exec_head.h>
# include "device.h"
# include "wvs.h"

SCCSID("@(#)execute.c	3.1 (SVID) 12/1/86");

void		exec_expr();
BOOLEAN		exec_compare();
STMT_TYPE	exec_loop();
void		pre_exec();
void		post_exec();

BOOLEAN		JumpFinis = FALSE;

STMT_TYPE execute(level, stmt_start)
int		level;
TBL_INDEX	stmt_start;
{
	TBL_INDEX	stmt_index;
	STMT_ENTRY	*stmt_ptr;
	TBL_INDEX	end_of_program;
	STMT_TYPE	result;

	if (level == 0)
	{
		pre_exec();
	}

	result = STMT_NULL;
	end_of_program = Prgm.StmtStat.next_free;

	for (stmt_index = stmt_start, stmt_ptr = &Prgm.StmtTbl[stmt_index]; 
	     ((result == STMT_NULL) && (stmt_index < end_of_program));
	     stmt_ptr = &Prgm.StmtTbl[stmt_index])
	{	
		if ((stmt_ptr->stmt_side_type != SIDE_NULL) &&
		    (stmt_ptr->stmt_side_type != SideType))
		{
			stmt_index++;
			continue;
		}

		switch (stmt_ptr->stmt_type)
		{
		  case STMT_ASSIGN:
			exec_expr(stmt_ptr->stmt_data.stmt_assign);
			stmt_index++;
			break;

		  case STMT_COMPARE:
			if (exec_compare(stmt_ptr->stmt_data.stmt_cmp.cmp_expr))
			{
				stmt_index++;
			}
			else
			{
				stmt_index = 
				    stmt_ptr->stmt_data.stmt_cmp.cmp_branch;
			}
			break;

		  case STMT_GOTO:
		  case STMT_GOTO_ELSE:
			stmt_index = stmt_ptr->stmt_data.stmt_goto;
			break;

		  case STMT_LOOP:
			result = exec_loop(level, stmt_index, stmt_ptr);
			stmt_index = stmt_ptr->stmt_data.stmt_loop.loop_branch;
			break;

		  case STMT_CONTINUE:
		  case STMT_CONT_LOOP:
		  case STMT_BREAK:
			if (level)
			{
				result = stmt_ptr->stmt_type;
			}
			break;

		  case STMT_FINIS:
		  case STMT_EXIT:
			result = stmt_ptr->stmt_type;
			break;

		  case STMT_START:
			result = exec_block(level + 1, stmt_index + 1);
			stmt_index = stmt_ptr->stmt_data.stmt_block;
			break;

		  default:
			screech("execute", 0, 
				"Bad stmt type = %d\n", stmt_ptr->stmt_type);
		}

		if (JumpFinis)
		{
			result = STMT_FINIS;
		}
	}

	if (level == 0)
	{
		post_exec();
	}

	return(result);
}

void exec_expr(expr_index)
TBL_INDEX	expr_index;
{
	VAR_ENTRY	*tmp_var;

	if (expr_index != NIL)
	{
		(void) do_eval(expr_index, &tmp_var);
		free_text(tmp_var);
		free_var(tmp_var);
	}
}

BOOLEAN exec_compare(expr_index)
TBL_INDEX	expr_index;
{
	VAR_ENTRY	*tmp_var;
	BOOLEAN		result;

	if (expr_index != NIL)
	{
		(void) do_eval(expr_index, &tmp_var);
		tmp_var = convert(tmp_var, DATA_BOOL);

		result = tmp_var->var_data.var_bool;
		free_var(tmp_var);
	}
	else
	{
		result = TRUE;
	}

	return(result);
}

STMT_TYPE exec_loop(level, stmt_index, stmt_ptr)
int		level;
TBL_INDEX	stmt_index;
STMT_ENTRY	*stmt_ptr;
{
	STMT_TYPE	result = STMT_NULL;
	BOOLEAN		break_loop = FALSE;

	while (exec_compare(stmt_ptr->stmt_data.stmt_loop.loop_expr))
	{
		switch (result = execute(level + 1, stmt_index + 1))
		{
		  case STMT_EXIT:
		  case STMT_FINIS:
			break_loop = TRUE;
			break;

		  case STMT_BREAK:
			result = STMT_NULL;
			break_loop = TRUE;
			break;

		  case STMT_CONTINUE:
		  case STMT_CONT_LOOP:
			result = STMT_NULL;
			break;

		  default:
			screech("exec_loop", 0, "Bad result = %d\n", result);
		}

		if (break_loop)
		{
			break;
		}

		exec_expr(stmt_ptr->stmt_data.stmt_loop.loop_for3);
	}

	return(result);
}

void pre_exec()
{
	if (ExecHead.do_initscr) 
	{
		switch (SideType)
		{
		  case SIDE_PITCHER:
			(void) p_initscr();
			break;

		  case SIDE_CATCHER:
			cx_initscr();
			break;

		  default:
			screech("pre_exec", 0, "Bad SideType = %d\n", SideType);
		}
	}
}

void post_exec()
{
	if ((ExecHead.do_endwin) && (SideType == SIDE_PITCHER))
	{
		p_endwin();
	}
}
