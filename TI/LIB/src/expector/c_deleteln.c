# include "main.h"

SCCSID("@(#)c_deleteln.c	3.1 (SVID) 12/1/86");

/*
** This routine deletes the current line by pulling the lines below it up over
** it.
*/
c_deleteln()
{
	int i,j;

	for (j = std->term_yloc; j < (std->term_height - 1); j++)
	{
		for (i = 0; i < std->term_width; i++)
		{
			std->term_screen[j][i] = std->term_screen[j+1][i];
			std->term_bits[j][i] = std->term_bits[j+1][i];
		}
	}

	/* 
	** This leaves a line at the bottom of the window, which we clear.
	*/

	for (i = 0; i < std->term_width; i++) 	
	{
		std->term_screen[j][i] = ' ';
		std->term_bits[j][i] = 0L;
	}

	return(OK);

}

/*
** This routine deletes the current line by pulling the lines below it up over
** it.
*/
c_wdeleteln(win)
TERMINAL *win;
{
	TERMINAL *target;
	int i, j, lim_i, lim_j;

	target = win;

	/*
	** If the given window is a subwindow, we must find the parent window,
	** since that's where the text is.
	*/ 

	while (target->container != NULL)
	{
		target = target->container;
	}

	/*
	** First, we delete the current line by pulling the lines below it up 
	** over it.  
	*/

	lim_j = (win->term_winy - target->term_winy) + win->term_height;
	lim_i = (win->term_winx - target->term_winx) + win->term_width;

	for (j = (win->term_yloc + win->term_winy) - target->term_winy;
	j < (lim_j - 1); 
	j++)
	{
		for (i = win->term_winx - target->term_winx;
		i < lim_i;
		i++)
		{
			target->term_screen[j][i] = target->term_screen[j+1][i];
			target->term_bits[j][i] = target->term_bits[j+1][i];
		}
	}

	/*
	** Then, since this leaves a line at the bottom of the screen, we clear it.
	*/

	for (i = win->term_winx - target->term_winx; i < lim_i; i++) 	
	{
		target->term_screen[j][i] = ' ';
		target->term_bits[j][i] = 0L;
	}

	return(OK);

}

