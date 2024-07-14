# include "main.h"

SCCSID("@(#)construct.c	3.2 (SVID) 12/30/86");

TBL_INDEX set_block_stmt()
{
	STMT_ENTRY	*stmt_ptr;
	TBL_INDEX	stmt_index;

	stmt_index = tbl_alloc((GENERIC *) &Prgm.StmtTbl, &Prgm.StmtStat, 1);
	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	stmt_ptr->stmt_type = STMT_START;
	stmt_ptr->stmt_side_type = SideType;
	stmt_ptr->stmt_data.stmt_block = NIL;

	return(stmt_index);
}

void rset_block_stmt(stmt_index, branch_index)
TBL_INDEX	stmt_index;
TBL_INDEX	branch_index;
{
	STMT_ENTRY	*stmt_ptr;

	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	if (branch_index == NEXT_FREE_STMT)
	{
		branch_index = Prgm.StmtStat.next_free;
	}

	stmt_ptr->stmt_data.stmt_block = branch_index;
}

void set_misc_stmt(stmt_type)
STMT_TYPE	stmt_type;
{
	STMT_ENTRY	*stmt_ptr;
	TBL_INDEX	stmt_index;

	stmt_index = tbl_alloc((GENERIC *) &Prgm.StmtTbl, &Prgm.StmtStat, 1);
	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	stmt_ptr->stmt_type = stmt_type;
	stmt_ptr->stmt_side_type = SideType;
}

void assign_stmt(expr_index)
TBL_INDEX	expr_index;
{
	STMT_ENTRY	*stmt_ptr;
	TBL_INDEX	stmt_index;

	stmt_index = tbl_alloc((GENERIC *) &Prgm.StmtTbl, &Prgm.StmtStat, 1);
	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	stmt_ptr->stmt_type = STMT_ASSIGN;
	stmt_ptr->stmt_side_type = SideType;
	stmt_ptr->stmt_data.stmt_assign = expr_index;
}

TBL_INDEX set_compare_stmt(expr_index)
TBL_INDEX	expr_index;
{
	STMT_ENTRY	*stmt_ptr;
	TBL_INDEX	stmt_index;

	stmt_index = tbl_alloc((GENERIC *) &Prgm.StmtTbl, &Prgm.StmtStat, 1);
	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	stmt_ptr->stmt_type = STMT_COMPARE;
	stmt_ptr->stmt_side_type = SideType;
	stmt_ptr->stmt_data.stmt_cmp.cmp_expr = expr_index;

	return(stmt_index);
}

void rset_compare_stmt(stmt_index, branch_index)
TBL_INDEX	stmt_index;
TBL_INDEX	branch_index;
{
	STMT_ENTRY	*stmt_ptr;

	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	if (branch_index == NEXT_FREE_STMT)
	{
		branch_index = Prgm.StmtStat.next_free;
	}

	stmt_ptr->stmt_data.stmt_cmp.cmp_branch = branch_index;
}

TBL_INDEX set_loop_stmt(expr_index)
TBL_INDEX	expr_index;
{
	STMT_ENTRY	*stmt_ptr;
	TBL_INDEX	stmt_index;

	stmt_index = tbl_alloc((GENERIC *) &Prgm.StmtTbl, &Prgm.StmtStat, 1);
	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	stmt_ptr->stmt_type = STMT_LOOP;
	stmt_ptr->stmt_side_type = SideType;
	stmt_ptr->stmt_data.stmt_loop.loop_expr = expr_index;

	return(stmt_index);
}

void rset_loop_stmt(stmt_index, branch_index, for3_index)
TBL_INDEX	stmt_index;
TBL_INDEX	branch_index;
TBL_INDEX	for3_index;
{
	STMT_ENTRY	*stmt_ptr;

	stmt_ptr = &Prgm.StmtTbl[stmt_index];

	if (branch_index == NEXT_FREE_STMT)
	{
		branch_index = Prgm.StmtStat.next_free;
	}

	stmt_ptr->stmt_data.stmt_loop.loop_branch = branch_index;
	stmt_ptr->stmt_data.stmt_loop.loop_for3 = for3_index;
}

TBL_INDEX set_goto_stmt(branch_index, special)
TBL_INDEX	branch_index;
int		special;
{
	STMT_ENTRY	*new_ptr;
	TBL_INDEX	new_index;

	new_index = tbl_alloc((GENERIC *) &Prgm.StmtTbl, &Prgm.StmtStat, 1);
	new_ptr = &Prgm.StmtTbl[new_index];

	if (special)
	{
		new_ptr->stmt_type = STMT_GOTO_ELSE;
	}
	else
	{
		new_ptr->stmt_type = STMT_GOTO;
	}

	if (branch_index == NEXT_FREE_STMT)
	{
		branch_index = Prgm.StmtStat.next_free;
	}
	new_ptr->stmt_data.stmt_goto = branch_index;
	new_ptr->stmt_side_type = SideType;

	return(new_index);
}

void rset_goto_stmt(stmt_index, branch_index)
TBL_INDEX	stmt_index;
TBL_INDEX	branch_index;
{
	if (branch_index == NEXT_FREE_STMT)
	{
		branch_index = Prgm.StmtStat.next_free;
	}
	Prgm.StmtTbl[stmt_index].stmt_data.stmt_goto = branch_index;
}

TBL_INDEX add_op_to_expr(operator)
OPERATOR_TYPE	operator;
{
	EXPR_ENTRY	*expr_ptr;
	TBL_INDEX	expr_index;

	expr_index = tbl_alloc((GENERIC *) &Prgm.ExprTbl, &Prgm.ExprStat, 1);
	expr_ptr = &Prgm.ExprTbl[expr_index];

	expr_ptr->expr_data.expr_op = operator;
	expr_ptr->expr_type = EXPR_OP;
	expr_ptr->expr_short_circuit = NIL;

	return(expr_index);
}

TBL_INDEX add_var_to_expr(var_index)
TBL_INDEX	var_index;
{
	EXPR_ENTRY	*expr_ptr;
	TBL_INDEX	expr_index;

	expr_index = tbl_alloc((GENERIC *) &Prgm.ExprTbl, &Prgm.ExprStat, 1);
	expr_ptr = &Prgm.ExprTbl[expr_index];

	expr_ptr->expr_data.expr_var = var_index;
	expr_ptr->expr_type = EXPR_VAR;
	expr_ptr->expr_short_circuit = NIL;

	return(expr_index);
}

TBL_INDEX add_call_to_expr(call_index)
TBL_INDEX	call_index;
{
	CALL_ENTRY	*call_ptr;
	EXPR_ENTRY	*expr_ptr;
	TBL_INDEX	expr_index;

	expr_index = tbl_alloc((GENERIC *) &Prgm.ExprTbl, &Prgm.ExprStat, 1);
	expr_ptr = &Prgm.ExprTbl[expr_index];

	expr_ptr->expr_data.expr_call = call_index;
	expr_ptr->expr_type = EXPR_CALL;
	expr_ptr->expr_short_circuit = NIL;

	call_ptr = &Prgm.CallTbl[call_index];

	call_ptr->call_expr = Prgm.ExprStat.next_free;
	call_ptr->call_expr_count = 0;

	return(expr_index);
}

void rset_call_expr(call_index, arg_count)
TBL_INDEX	call_index;
int		arg_count;
{
	CALL_ENTRY	*call_ptr;

	call_ptr = &Prgm.CallTbl[call_index];

	call_ptr->call_expr_count = arg_count;
	call_ptr->call_expr_next = Prgm.ExprStat.next_free;
}

void keep_expr()
{
	TBL_INDEX	expr_index;

	expr_index = tbl_alloc((GENERIC *) &Prgm.ExprTbl, &Prgm.ExprStat, 1);
	Prgm.ExprTbl[expr_index].expr_type = EXPR_END;

	Prgm.ExprStat.restart = Prgm.ExprStat.next_free;
	Prgm.VarStat.restart = Prgm.VarStat.next_free;
	Prgm.NameStat.restart = Prgm.NameStat.next_free;
}

void junk_expr()
{
	Prgm.ExprStat.next_free = Prgm.ExprStat.restart;
	Prgm.VarStat.next_free = Prgm.VarStat.restart;
	Prgm.NameStat.next_free = Prgm.NameStat.restart;
}
