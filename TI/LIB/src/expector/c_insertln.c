# include "main.h"

SCCSID("@(#)c_insertln.c	3.1 (SVID) 12/1/86");

c_insertln()
{
	int i,j;

	/*
	** First, we pull down all lines by one, starting from the line where the
	** cursor is at. The bottommost line is lost.
	*/

	for ( j = std->term_height - 1; j > std->term_yloc; j--)
	{
		for (i = 0; i < std->term_width; i++)
		{
			std->term_screen[j][i] = std->term_screen[j-1][i];
			std->term_bits[j][i] = std->term_bits[j-1][i];	
		}
	}

	/*
	** Then, we blank out the current line.
	*/

	for (i = 0; i < std->term_width; i++)
	{
		std->term_screen[std->term_yloc][i] = ' ';
		std->term_bits[std->term_yloc][i] = 0L;
	}

	return(OK);
}	

c_wlninsert(win)	/* catcher's version of winsertln */
TERMINAL *win;
{
	TERMINAL *target;
	int i,j;

	target = win;

	/*
	** If the given window is a subwindow, we must find the parent window, since 
	** that's where the text for the window is.
	*/

	while (target->container != NULL)
	{
		target = target->container;
	}

	if (target != win)
	{
		target->term_savy = target->term_yloc;

		target->term_yloc = (win->term_winy - target->term_winy) + win->term_yloc;
	}

	/*
	** Here, we perform the same actions as "c_insertln", except we pull down
	** lines from the window, which is usually a small area within the screen.
	*/

	for ( j = (win->term_winy - target->term_winy) + win->term_height -1;
	j > target->term_yloc; 
	j--)
	{
		for (i = win->term_winx - target->term_winx;
		i < (win->term_winx - target->term_winx) + win->term_width;
		i++)
		{
			target->term_screen[j][i] = target->term_screen[j-1][i];
			target->term_bits[j][i] = target->term_bits[j-1][i];	
		}
	}
	
	for (i = win->term_winx - target->term_winx;
	i < (win->term_winx - target->term_winx) + win->term_width;
	i++)
	{
		target->term_screen[target->term_yloc][i] = ' ';
		target->term_bits[target->term_yloc][i] = 0L;
	}

	if (target != win)
	{
		target->term_yloc = target->term_savy;
	}

	return(OK);
}
