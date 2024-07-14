# include "main.h"

SCCSID("@(#)c_clrtoeol.c	3.1 (SVID) 12/1/86");

c_clreol()	/* catcher's version of clrtoeol */
{
	int i;

	/*
	** "ce_func" (found in "bless.a") clears from the cursor to end of the
	** current line.
	*/

	for (i = std->term_xloc; i < std->term_width; i++)
	{
		std->term_screen[std->term_yloc][i] = ' ';
		std->term_bits[std->term_yloc][i] = 0L; 
	}

	return(OK);
}

c_weolclr(win)	/* catcher's version of wclrtoeol */
TERMINAL *win;
{
	TERMINAL *target;
	int i, lim_i;

	target = win;

	/*
	** If the given window is a subwindow, we must find the parent window,
	** since that's where the actual window text is.
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

	/*
	** We can't use "ce_func" here since ce_func clears all the way to the edge
	** of the screen, while here we want only to clear to the edge of the window,
	** which is usually a small area within  the screen.
	*/

	for (i = target->term_xloc; i < lim_i; i++)
	{
		target->term_screen[target->term_yloc][i] = ' ';
		target->term_bits[target->term_yloc][i] = 0L; 
	}

	if (target != win)
	{
		win->term_yloc = target->term_yloc + target->term_winy - win->term_winy;
		win->term_xloc = target->term_xloc + target->term_winx - win->term_winx;
		(void) c_wmove(target,target->term_savy,target->term_savx);
	}

	return(OK);
}

