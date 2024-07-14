# include "main.h"

SCCSID("@(#)c_stscrg.c	3.1 (SVID) 12/1/86");
/*
** These routines set scrolling regions to the desired values.
** If the cursor is at the bottom right-hand corner of one of
** these regions and an attempt is made to add text, the region 
** is scrolled up one line.
*/

c_stscrreg(topvar, botvar)	/* catcher's version of setscrreg */
VAR_ENTRY *topvar, *botvar;
{
	int top, bot;

	top = topvar->var_data.var_num;
	bot = botvar->var_data.var_num;

	std->scrl_top = top;
	std->scrl_bot = bot;

	return(OK);
}

c_wstscrreg(winvar, topvar, botvar)	/* catcher's version of wsetscrreg */ 
VAR_ENTRY *winvar, *topvar, *botvar;
{
	int top, bot;
	TERMINAL *win;

	win = winvar->var_data.var_term;
	top = topvar->var_data.var_num;
	bot = botvar->var_data.var_num;

	win->scrl_top = top;
	win->scrl_bot = bot;

	return(OK);
}

