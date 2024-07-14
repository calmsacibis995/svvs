# include "main.h"
# include "option.h"
# include <exec_head.h>

SCCSID("@(#)reload.c	3.1 (SVVS) 12/1/86");

void	win_reload();
void	term_reload();
void	num_reload();
void	char_reload();

void reload_var()
{
	switch (SideType)
	{
	  case SIDE_PITCHER:
		win_reload(ExecHead.index_stdscr, stdscr);
		win_reload(ExecHead.index_curscr, curscr);

		num_reload(ExecHead.index_LINES, LINES);
		num_reload(ExecHead.index_COLS, COLS);

		char_reload(ExecHead.index_SideType, 'P');
		break;

	  case SIDE_CATCHER:
		if (Option.run_mimic)
		{
			term_reload(ExecHead.index_stdscr, std);
			term_reload(ExecHead.index_curscr, std);

			num_reload(ExecHead.index_LINES, c_LINES);
			num_reload(ExecHead.index_COLS, c_COLS);

			char_reload(ExecHead.index_SideType, 'C');
		}
		break;

	  default:
		screech("reload_var", 0, "Bad SideType = %d\n", SideType);
	}
}

void win_reload(tbl_index, win_data)
TBL_INDEX	tbl_index;
WINDOW		*win_data;
{
	Prgm.VarTbl[tbl_index].var_data.var_window = win_data;
}

void term_reload(tbl_index, term_data)
TBL_INDEX	tbl_index;
TERMINAL	*term_data;
{
	Prgm.VarTbl[tbl_index].var_data.var_term = term_data;
}

void num_reload(tbl_index, num_data)
TBL_INDEX	tbl_index;
NUMBER_TYPE	num_data;
{
	Prgm.VarTbl[tbl_index].var_data.var_num = num_data;
}

void char_reload(tbl_index, char_data)
TBL_INDEX	tbl_index;
chtype		char_data;
{
	Prgm.VarTbl[tbl_index].var_data.var_chtype = char_data;
}
