# include "main.h"

SCCSID("@(#)c_delch.c	3.1 (SVID) 12/1/86");

/*
** This routine simply deletes the character at the current cursor position,
** pulling the rest of the line over it.
*/
c_dlch()	/* catcher's version of delch */
{
	int x;

	for (x = std->term_xloc; x < (std->term_width - 1); x++)
	{
		std->term_screen[std->term_yloc][x] = std->term_screen[std->term_yloc][x + 1];
		std->term_bits[std->term_yloc][x] = std->term_bits[std->term_yloc][x + 1];
	}

/*
** The last character on the line is assigned a blank.
*/

	std->term_screen[std->term_yloc][std->term_width - 1] = ' ';
	std->term_bits[std->term_yloc][std->term_width - 1] = 0L;

	return(OK);
}

/*
** This routine simply deletes the character at the current cursor position
** on the given window, pulling the rest of the line over it.
*/
c_wchdel(win)  /* catcher's version of wdelch */ 
TERMINAL *win; 
{
	TERMINAL *target;
	int x, lim_x;

	target = win;

/*
** If the given window is a subwindow, we must find the parent window, 
** since that's where the actual text is.
*/

	while (target->container != NULL)
	{
		target = target->container;
	}

	if (target != win)
	{	
		(void) c_wmove(target,(win->term_yloc + win->term_winy) - target->term_winy,(win->term_xloc + win->term_winx) - target->term_winx);
	}

/*
** First, we delete the character under the current cursor position by    
** pulling the rest of the line over it.
*/

	lim_x = (win->term_winx - target->term_winx) + win->term_width;

	for (x = target->term_xloc; x < (lim_x - 1); x++)
	{
		target->term_screen[target->term_yloc][x] = target->term_screen[target->term_yloc][x + 1];
		target->term_bits[target->term_yloc][x] = target->term_bits[target->term_yloc][x + 1];
	}

/*
** Since this leaves a space at the end of the line, we fill it with a blank
*/

	target->term_screen[target->term_yloc][(win->term_width + win->term_winx - target->term_winx) - 1] = ' ';
	target->term_bits[target->term_yloc][(win->term_width + win->term_winx - target->term_winx) - 1] = 0L;
	
	if (target != win)
	{
		(void) c_wmove(target,target->term_savy,target->term_savx);
	}

	return(OK);
}

/*
** This routine allows us to choose the location on the standard screen
** where we want a character deleted.
*/
c_delchmv(vary,varx)	/* catcher's version of mvdelch */
VAR_ENTRY *vary, *varx;
{
	(void) c_move(vary,varx);
	(void) c_dlch();

	return(OK);
}

/*
** This routine allows us to choose the location on the given window   
** where we want a character deleted.
*/
c_chdelmvw(winvar,vary,varx)	/* catcher's version of mvwdelch */
VAR_ENTRY *winvar, *vary, *varx;
{
	int y,x;
	TERMINAL *win;

	win = winvar->var_data.var_term;
	y = vary->var_data.var_num;
	x = varx->var_data.var_num;

	(void) c_wmove(win,y,x);
	(void) c_wchdel(win);

	return(OK);
}

