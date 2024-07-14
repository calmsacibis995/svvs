/*
** SCCSID("@(#)comb_macro.h	3.3 (SVID) 12/24/86")
*/

# define	X_WIN		win_arg->var_data.var_window
# define	X_Y		y_arg->var_data.var_num
# define	X_X		x_arg->var_data.var_num
# define	X_FMT		fmt_arg->var_data.var_text_ptr
# define	X_TEXT(i)	var_arg_tbl[(i)]->var_data.var_text_ptr

# define	X_CHAR(i)	var_arg_tbl[(i)]->var_data.var_chtype
# define	X_NUM(i)	var_arg_tbl[(i)]->var_data.var_num
# define	X_FLOAT(i)	var_arg_tbl[(i)]->var_data.var_float

# define	X_C_PTR(i)	&(var_arg_tbl[(i)]->var_data.var_chtype)
# define	X_N_PTR(i)	&(var_arg_tbl[(i)]->var_data.var_num)
# define	X_F_PTR(i)	&(var_arg_tbl[(i)]->var_data.var_float)
