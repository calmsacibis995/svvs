# include "main.h"

SCCSID("@(#)c_move.c	3.1 (SVID) 12/1/86");
/*
** These routines just move the cursor, saving the old location. 
*/

c_move(vary,varx)
VAR_ENTRY *vary, *varx;
{
	int y,x;

	y = vary->var_data.var_num;
	x = varx->var_data.var_num;

	if ( (y < 0) || (y >= std->term_height) || (x < 0) || (x >= std->term_width) )
	{
		msg_drop("ERROR in move: attempted to move cursor outside of standard screen\n");
		return(ERR);
	}

	std->term_savy = std->term_yloc;
	std->term_savx = std->term_xloc;
	std->term_yloc = y;
	std->term_xloc = x;

	return(OK);
}

c_wmove(win,y,x)
TERMINAL *win;
int y,x;
{
	win->term_savy = win->term_yloc;
	win->term_savx = win->term_xloc;

	if ( (y < 0) || (y >= win->term_height) || (x < 0) || (x >= win->term_width) )
	{
		msg_drop("ERROR in wmove: attempted to move cursor outside of window\n");
		return(ERR);
	}

	win->term_yloc = y;
	win->term_xloc = x;
	
	return(OK);
}

