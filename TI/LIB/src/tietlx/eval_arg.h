/*
** SCCSID("@(#)eval_arg.h	3.1 (SVID) 12/1/86");
*/

typedef struct VAR_ARG_LINK
{
	VAR_ENTRY		*var_arg_fmt;
	DATA_TYPE		var_arg_type;
	VAR_ENTRY		*var_arg_arg;
	BOOLEAN			inflated;

	struct VAR_ARG_LINK	*var_arg_next;
} VAR_ARG_ENTRY;

extern VAR_ARG_ENTRY	*eval_var_arg();
extern void		free_v_arg();

extern VAR_ENTRY	**eval_arg();
extern void		free_arg();
