/*
** SCCSID("@(#)construct.h	3.1 (SVID) 12/1/86");
*/

extern TBL_INDEX	set_block_stmt();
extern void		rset_block_stmt();
extern void		set_misc_stmt();
extern void		assign_stmt();
extern TBL_INDEX	set_compare_stmt();
extern void		rset_compare_stmt();
extern TBL_INDEX	set_loop_stmt();
extern void		rset_loop_stmt();
extern TBL_INDEX	set_goto_stmt();
extern void		rset_goto_stmt();

extern TBL_INDEX	add_op_to_expr();
extern TBL_INDEX	add_var_to_expr();
extern TBL_INDEX	add_call_to_expr();
extern TBL_INDEX	rset_call_expr();
extern void		junk_expr();
extern void		keep_expr();
