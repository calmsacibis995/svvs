# include "main.h" 

SCCSID("@(#)c_getyx.c	3.1 (SVID) 12/1/86");

c_getyx(winvar,yvar,xvar)
VAR_ENTRY *winvar, *yvar, *xvar;
{
/*
** This routine simply returns (in yvar and xvar) the current
** cursor location of the given window.
*/
	TERMINAL *win;

	win = winvar->var_data.var_term;
	yvar->var_data.var_num = win->term_yloc;
	xvar->var_data.var_num = win->term_xloc;

	return(OK);
}

c_getbegyx(winvar,yvar,xvar)
VAR_ENTRY *winvar, *yvar, *xvar;
{
/*
** This routine returns(in yvar and xvar) the screen location at which
** the given window is to begin--that is, the place where its upper-left
** corner is located on the screen
*/
	TERMINAL *win;

	win = winvar->var_data.var_term;
	yvar->var_data.var_num = win->term_winy; 
	xvar->var_data.var_num = win->term_winx;

	return(OK);
}

c_getmaxyx(winvar,yvar,xvar)
VAR_ENTRY *winvar, *yvar, *xvar;
{
/*
** This routine returns (in yvar and xvar) the maximum line number
** and column number in the given window.
*/

	TERMINAL *win;

	win = winvar->var_data.var_term;
	yvar->var_data.var_num = win->term_height - 1;
	xvar->var_data.var_num = win->term_width - 1;

	return(OK);
}

