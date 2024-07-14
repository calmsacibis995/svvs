# include "main.h"

SCCSID("@(#)c_clrtobot.c	3.1 (SVID) 12/1/86");

c_clrbot()	/* catcher's version of clrtobot */
{
	int i,j;

/*
** Starting at the current cursor location we clear out the rest of the window
** from there on down. Initially, we clear the rest of the current line.
*/

	for (i = std->term_xloc; i < std->term_width; i++)
	{
		std->term_screen[std->term_yloc][i] = ' ';
		std->term_bits[std->term_yloc][i] = 0L; 
	}

/*
** Then, we clear the remaining lines below that.
*/

	for (j = std->term_yloc+1; j < std->term_height; j++)
	{
		for (i = 0; i < std->term_width; i++)
		{
			std->term_screen[j][i] = ' ';
			std->term_bits[j][i] = 0L;
		}
	}

	return(OK);

}

c_wbotclr(win)	/* catcher's version of wclrtrobot */
TERMINAL *win; 
{
	TERMINAL *target;
	int i, j, lim_i, lim_j;

	target = win;

/*
** If the given window is a subwindow, we must find its parent window, since
** that's where the test is.
*/

	while (target->container != NULL)
	{
		target = target->container;
	}

	if (target != win)
	{	
		(void) c_wmove(target,(win->term_yloc + win->term_winy) - target->term_winy,(win->term_xloc + win->term_winx) - target->term_winx);
	}

	lim_i = (win->term_winx - target->term_winx) + win->term_width;
	lim_j = (win->term_winy - target->term_winy) + win->term_height;

/*
** First, we clear the rest of the current line.
*/

	for (i = target->term_xloc; i < lim_i; i++)
	{
		target->term_screen[target->term_yloc][i] = ' ';
		target->term_bits[target->term_yloc][i] = 0L; 
	}

/*
** Then, we clear the remaining lines below that.
*/

	for (j = target->term_yloc+1; j < lim_j; j++)
	{
		for (i = win->term_winx - target->term_winx; i < lim_i; i++)
		{
			target->term_screen[j][i] = ' ';
			target->term_bits[j][i] = 0L;
		}
	}

	if (target != win)
	{
		win->term_yloc = target->term_yloc + target->term_winy - win->term_winy;
		win->term_xloc = target->term_xloc + target->term_winx - win->term_winx;

		(void) c_wmove(target,target->term_savy,target->term_savx);
	}
	
	return(OK);

}
 
