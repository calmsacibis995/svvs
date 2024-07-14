# include "main.h"
# include "store.h"
# include <exec_head.h>

SCCSID("@(#)load_var.c	3.2 (SVID) 12/23/86");

TBL_INDEX	num_preload();
TBL_INDEX	win_preload();
TBL_INDEX	char_preload();

void load_var()
{
	ExecHead.index_stdscr = win_preload("stdscr", 
					    CLASS_VAR, 
					    (WINDOW *) NULL);
	ExecHead.index_curscr = win_preload("curscr", 
					    CLASS_VAR, 
					    (WINDOW *) NULL);

	ExecHead.index_LINES = num_preload("LINES", CLASS_CONST, NULL);
	ExecHead.index_COLS = num_preload("COLS", CLASS_CONST, NULL);

	ExecHead.index_SideType = char_preload("SideType", CLASS_CONST, NULL);

	(void) num_preload("TRACE_BLESS",	CLASS_CONST, TRACE_BLESS);
	(void) num_preload("TRACE_TIETL",	CLASS_CONST, TRACE_TIETL);

	(void) num_preload("OK",		CLASS_CONST, OK);
	(void) num_preload("ERR",		CLASS_CONST, ERR);

	(void) num_preload("A_STANDOUT",	CLASS_CONST, A_STANDOUT);
	(void) num_preload("A_UNDERLINE",	CLASS_CONST, A_UNDERLINE);
	(void) num_preload("A_REVERSE",		CLASS_CONST, A_REVERSE);
	(void) num_preload("A_BLINK",		CLASS_CONST, A_BLINK);
	(void) num_preload("A_DIM",		CLASS_CONST, A_DIM);
	(void) num_preload("A_BOLD",		CLASS_CONST, A_BOLD);
	(void) num_preload("A_ALTCHARSET",	CLASS_CONST, A_ALTCHARSET);
	(void) num_preload("A_CHARTEXT",	CLASS_CONST, A_CHARTEXT);
	(void) num_preload("A_ATTRIBUTES",	CLASS_CONST, A_ATTRIBUTES);

	(void) num_preload("KEY_BREAK",		CLASS_CONST, KEY_BREAK);
	(void) num_preload("KEY_DOWN",		CLASS_CONST, KEY_DOWN);
	(void) num_preload("KEY_UP",		CLASS_CONST, KEY_UP);
	(void) num_preload("KEY_LEFT",		CLASS_CONST, KEY_LEFT);
	(void) num_preload("KEY_RIGHT",		CLASS_CONST, KEY_RIGHT);
	(void) num_preload("KEY_HOME",		CLASS_CONST, KEY_HOME);
	(void) num_preload("KEY_BACKSPACE",	CLASS_CONST, KEY_BACKSPACE);
	(void) num_preload("KEY_F0",		CLASS_CONST, KEY_F0);
	(void) num_preload("KEY_DL",		CLASS_CONST, KEY_DL);
	(void) num_preload("KEY_IL",		CLASS_CONST, KEY_IL);
	(void) num_preload("KEY_DC",		CLASS_CONST, KEY_DC);
	(void) num_preload("KEY_IC",		CLASS_CONST, KEY_IC);
	(void) num_preload("KEY_EIC",		CLASS_CONST, KEY_EIC);
	(void) num_preload("KEY_CLEAR",		CLASS_CONST, KEY_CLEAR);
	(void) num_preload("KEY_EOS",		CLASS_CONST, KEY_EOS);
	(void) num_preload("KEY_EOL",		CLASS_CONST, KEY_EOL);
	(void) num_preload("KEY_SF",		CLASS_CONST, KEY_SF);
	(void) num_preload("KEY_SR",		CLASS_CONST, KEY_SR);
	(void) num_preload("KEY_NPAGE",		CLASS_CONST, KEY_NPAGE);
	(void) num_preload("KEY_PPAGE",		CLASS_CONST, KEY_PPAGE);
	(void) num_preload("KEY_STAB",		CLASS_CONST, KEY_STAB);
	(void) num_preload("KEY_CTAB",		CLASS_CONST, KEY_CTAB);
	(void) num_preload("KEY_CATAB",		CLASS_CONST, KEY_CATAB);
	(void) num_preload("KEY_ENTER",		CLASS_CONST, KEY_ENTER);
	(void) num_preload("KEY_SRESET",	CLASS_CONST, KEY_SRESET);
	(void) num_preload("KEY_RESET",		CLASS_CONST, KEY_RESET);
	(void) num_preload("KEY_PRINT",		CLASS_CONST, KEY_PRINT);
	(void) num_preload("KEY_LL",		CLASS_CONST, KEY_LL);
	(void) num_preload("KEY_A1",		CLASS_CONST, KEY_A1);
	(void) num_preload("KEY_A3",		CLASS_CONST, KEY_A3);
	(void) num_preload("KEY_B2",		CLASS_CONST, KEY_B2);
	(void) num_preload("KEY_C1",		CLASS_CONST, KEY_C1);
	(void) num_preload("KEY_C3",		CLASS_CONST, KEY_C3);
	(void) num_preload("KEY_BTAB",		CLASS_CONST, KEY_BTAB);
	(void) num_preload("KEY_BEG",		CLASS_CONST, KEY_BEG);
	(void) num_preload("KEY_CANCEL",	CLASS_CONST, KEY_CANCEL);
	(void) num_preload("KEY_CLOSE",		CLASS_CONST, KEY_CLOSE);
	(void) num_preload("KEY_COMMAND",	CLASS_CONST, KEY_COMMAND);
	(void) num_preload("KEY_COPY",		CLASS_CONST, KEY_COPY);
	(void) num_preload("KEY_CREATE",	CLASS_CONST, KEY_CREATE);
	(void) num_preload("KEY_END",		CLASS_CONST, KEY_END);
	(void) num_preload("KEY_EXIT",		CLASS_CONST, KEY_EXIT);
	(void) num_preload("KEY_FIND",		CLASS_CONST, KEY_FIND);
	(void) num_preload("KEY_HELP",		CLASS_CONST, KEY_HELP);
	(void) num_preload("KEY_MARK",		CLASS_CONST, KEY_MARK);
	(void) num_preload("KEY_MESSAGE",	CLASS_CONST, KEY_MESSAGE);
	(void) num_preload("KEY_MOVE",		CLASS_CONST, KEY_MOVE);
	(void) num_preload("KEY_NEXT",		CLASS_CONST, KEY_NEXT);
	(void) num_preload("KEY_OPEN",		CLASS_CONST, KEY_OPEN);
	(void) num_preload("KEY_OPTIONS",	CLASS_CONST, KEY_OPTIONS);
	(void) num_preload("KEY_PREVIOUS",	CLASS_CONST, KEY_PREVIOUS);
	(void) num_preload("KEY_REDO",		CLASS_CONST, KEY_REDO);
	(void) num_preload("KEY_REFERENCE",	CLASS_CONST, KEY_REFERENCE);
	(void) num_preload("KEY_REFRESH",	CLASS_CONST, KEY_REFRESH);
	(void) num_preload("KEY_REPLACE",	CLASS_CONST, KEY_REPLACE);
	(void) num_preload("KEY_RESTART",	CLASS_CONST, KEY_RESTART);
	(void) num_preload("KEY_RESUME",	CLASS_CONST, KEY_RESUME);
	(void) num_preload("KEY_SAVE",		CLASS_CONST, KEY_SAVE);
	(void) num_preload("KEY_SBEG",		CLASS_CONST, KEY_SBEG);
	(void) num_preload("KEY_SCANCEL",	CLASS_CONST, KEY_SCANCEL);
	(void) num_preload("KEY_SCOMMAND",	CLASS_CONST, KEY_SCOMMAND);
	(void) num_preload("KEY_SCOPY",		CLASS_CONST, KEY_SCOPY);
	(void) num_preload("KEY_SCREATE",	CLASS_CONST, KEY_SCREATE);
	(void) num_preload("KEY_SDC",		CLASS_CONST, KEY_SDC);
	(void) num_preload("KEY_SDL",		CLASS_CONST, KEY_SDL);
	(void) num_preload("KEY_SELECT",	CLASS_CONST, KEY_SELECT);
	(void) num_preload("KEY_SEND",		CLASS_CONST, KEY_SEND);
	(void) num_preload("KEY_SEOL",		CLASS_CONST, KEY_SEOL);
	(void) num_preload("KEY_SEXIT",		CLASS_CONST, KEY_SEXIT);
	(void) num_preload("KEY_SFIND",		CLASS_CONST, KEY_SFIND);
	(void) num_preload("KEY_SHELP",		CLASS_CONST, KEY_SHELP);
	(void) num_preload("KEY_SHOME",		CLASS_CONST, KEY_SHOME);
	(void) num_preload("KEY_SIC",		CLASS_CONST, KEY_SIC);
	(void) num_preload("KEY_SLEFT",		CLASS_CONST, KEY_SLEFT);
	(void) num_preload("KEY_SMESSAGE",	CLASS_CONST, KEY_SMESSAGE);
	(void) num_preload("KEY_SMOVE",		CLASS_CONST, KEY_SMOVE);
	(void) num_preload("KEY_SNEXT",		CLASS_CONST, KEY_SNEXT);
	(void) num_preload("KEY_SOPTIONS",	CLASS_CONST, KEY_SOPTIONS);
	(void) num_preload("KEY_SPREVIOUS",	CLASS_CONST, KEY_SPREVIOUS);
	(void) num_preload("KEY_SPRINT",	CLASS_CONST, KEY_SPRINT);
	(void) num_preload("KEY_SREDO",		CLASS_CONST, KEY_SREDO);
	(void) num_preload("KEY_SREPLACE",	CLASS_CONST, KEY_SREPLACE);
	(void) num_preload("KEY_SRIGHT",	CLASS_CONST, KEY_SRIGHT);
	(void) num_preload("KEY_SRSUME",	CLASS_CONST, KEY_SRSUME);
	(void) num_preload("KEY_SSAVE",		CLASS_CONST, KEY_SSAVE);
	(void) num_preload("KEY_SSUSPEND",	CLASS_CONST, KEY_SSUSPEND);
	(void) num_preload("KEY_SUNDO",		CLASS_CONST, KEY_SUNDO);
	(void) num_preload("KEY_SUSPEND",	CLASS_CONST, KEY_SUSPEND);
	(void) num_preload("KEY_UNDO",		CLASS_CONST, KEY_UNDO);
}

TBL_INDEX num_preload(var_name, var_class, var_data)
char		*var_name;
CLASS_TYPE	var_class;
int		var_data;
{
	TBL_INDEX	var_index;
	VAR_ENTRY	*var_ptr;

	var_index = sav_var(var_name);
	var_ptr = &Prgm.VarTbl[var_index];
	var_ptr->var_class = var_class;
	var_ptr->var_type = DATA_NUM;
	var_ptr->var_data.var_num = var_data;

	return(var_index);
}

TBL_INDEX char_preload(var_name, var_class, var_data)
char		*var_name;
CLASS_TYPE	var_class;
chtype		var_data;
{
	TBL_INDEX	var_index;
	VAR_ENTRY	*var_ptr;

	var_index = sav_var(var_name);
	var_ptr = &Prgm.VarTbl[var_index];
	var_ptr->var_class = var_class;
	var_ptr->var_type = DATA_CHAR;
	var_ptr->var_data.var_chtype = var_data;

	return(var_index);
}

TBL_INDEX win_preload(var_name, var_class, var_data)
char		*var_name;
CLASS_TYPE	var_class;
WINDOW		*var_data;
{
	TBL_INDEX	var_index;
	VAR_ENTRY	*var_ptr;

	var_index = sav_var(var_name);
	var_ptr = &Prgm.VarTbl[var_index];
	var_ptr->var_class = var_class;
	var_ptr->var_type = DATA_WINDOW;
	var_ptr->var_data.var_window = var_data;

	return(var_index);
}
