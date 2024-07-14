/*
** SCCSID("@(#)funct_def.h	3.1 (SVID) 12/1/86");
*/

typedef struct
{
	char		*funct_name;
	BOOLEAN		front_end;
	BOOLEAN		do_expect;
	BOOLEAN		expect_success;
	int		(*funct_p_call)();
	int		(*funct_c_call)();
	int		(*funct_p_conf)();
	int		(*funct_c_conf)();
	int		(*funct_special)();
} FUNCT_ENTRY;

