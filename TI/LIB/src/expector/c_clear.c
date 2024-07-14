# include "main.h"

SCCSID("@(#)c_clear.c	3.1 (SVID) 12/1/86");

/*
** This routine fills the standard screen with blanks, and arranges 
** for the screen to be redrawn upon the next c_refresh().
*/
c_clear()
{
	ARGS *arg;
	int i,j;

	arg = NULL; 

/*
** "cl_func" (found in "bless.a") just blanks out the given window. 
** "arg" is a dummy argument.
*/

	cl_func(std,arg);

	for (j = 0; j < std->term_height; j++)
	{
		for (i = 0; i < std->term_width; i++)
		{
			std->term_bits[j][i] = 0L;
		}
	}

/*
** "clrok" instructs the next call to "refresh" 
** to first clear the screen and redraw.
*/

	(void) c_clrok(std,TRUE);
	
	return(OK);

}

/*
** This routine fills the given window with blanks, and arranges 
** for the screen to be redrawn upon the next c_wrefresh(win).
*/
c_wclear(win)
TERMINAL *win; 
{
	TERMINAL *target;
	int i,j, lim_i, lim_j;

	target = win;

/*
** If the given window is a subwindow, we must find its parent window,
** since that's where the text is kept.
*/

	while (target->container != NULL)
	{
		target = target->container; 
	}

/*
** We can't use "cl_func" here because "cl_func" clears all the way to
** the borders of the screen, while here we want only to clear to the
** borders of the window, which is probably a small area within
** the screen.
*/

	lim_i = (win->term_winx - target->term_winx) + win->term_width;
	lim_j = (win->term_winy - target->term_winy) + win->term_height;

	for (j = win->term_winy - target->term_winy; j < lim_j; j++)
	{
		for (i = win->term_winx - target->term_winx; i < lim_i; i++)
		{
			target->term_screen[j][i] = ' ';
			target->term_bits[j][i] = 0L;
		}
	}

	(void) c_clrok(win,TRUE);

	return(OK);

}
 
