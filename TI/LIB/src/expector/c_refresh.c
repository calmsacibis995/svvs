# include "main.h"

SCCSID("@(#)c_refresh.c	3.1 (SVID) 12/1/86");
# define	MAXWINS	50 

int next_empty = 0;
TERMINAL *windows[MAXWINS];
extern TERMINAL *saved_scr;

/*
** This routine clears all characters and attributes from the
** screen, represented by DISPLAY.
*/
clrscr()
{
	int i,j;
	for (j = 0; j < (DISPLAY->term_height); j++)
	{
		for (i = 0; i < (DISPLAY->term_width); i++)
		{
			DISPLAY->term_bits[j][i] = 0L;
			DISPLAY->term_screen[j][i] = ' '; 
		}
	}
}

/*
** This routine writes the given window to the screen.
*/
c_wrefresh(win)
TERMINAL *win;
{
	if (win->is_a_pad)
	{
		msg_drop("ERROR--to refresh pad, use prefresh\n");
		return(ERR);
	}
	else
	{
		(void) c_wnoutrefresh(win);
		(void) c_doupdate();
		return(OK);
	}
}
	
c_refresh()
{
	(void) c_wrefresh(std);
}

/*
** This routine adds the given window to the list of windows waiting to be
** written to the screen.
*/

c_wnoutrefresh(win)
TERMINAL *win;
{

	if (next_empty < MAXWINS)
	{
		windows[next_empty] = win;
		next_empty +=1;
	}
	else
	{
		screech("c_wnoutrefresh",0,"ERROR in pnoutrefresh: window limit exceeded \n");
	}
}

/*
** This routine puts the status line in line to be written to the screen
*/
c_slknoutrefresh()
{
	(void) c_wnoutrefresh(status_line);
}

/*
** In c_doupdate, we take each window on the waiting list and copy it to 
** the screen in its proper place as specified.
*/
c_doupdate()
{

	int i;

	for (i = 0; i < next_empty; i++)
	{
		if (windows[i] != DISPLAY)
		{
			if (windows[i]->is_a_pad)
			{
				(void) pad_update(windows[i]);
			}
			else
			{
				(void) update(windows[i]);
			}
		}
	}		 	

	next_empty = 0;

	return(OK);
}

/*
** This routine copies the given window to the screen (i.e., DISPLAY)
*/
update(win)
TERMINAL *win;
{
	TERMINAL *target;
	int i1, i2, j1, j2, i3;
 
/*
** If the given window was initialized with "clear" rather than just
** "erase", the whole screen is cleared and redrawn before being updated.
*/

	if (term_on(win,WIN_CLROK))
	{
		clrscr(); 
		term_clr(win,WIN_CLROK);
		(void) redraw(); 
	}
 
/*
** If the given window is a subwindow, we need to find its parent window,
** since that's the window that contains the text to be copied.
*/

	target = win;
	while (target->container != NULL)
	{
		target = target->container;
	}

	for (j1 = win->term_winy,j2 = (win->term_winy - target->term_winy);
	(j1 < DISPLAY->term_height) && (j2 < (win->term_winy - target->term_winy) + win->term_height);
	j1++, j2++)
	{	
		for (i2 = (win->term_winx - target->term_winx); 
		(i2 < (win->term_winx - target->term_winx) + win->term_width);
		i2++)
		{

			if ((target->term_screen[j2][i2] != target->term_lastrefresh[j2][i2]) || (target->term_bits[j2][i2] != target->term_lastbits[j2][i2]))
			{
				for (i1 = win->term_winx, i3 = (win->term_winx - target->term_winx); 
				(i1 < DISPLAY->term_width) && (i3 < (win->term_winx - target->term_winx) + win->term_width);
				i1++, i3++)
				{

					target->term_lastrefresh[j2][i3] = target->term_screen[j2][i3];
					target->term_lastbits[j2][i3] = target->term_bits[j2][i3];

					saved_scr->term_screen[j1][i1] = target->term_screen[j2][i3];
					saved_scr->term_bits[j1][i1] = target->term_bits[j2][i3];
	 				DISPLAY->term_screen[j1][i1] = saved_scr->term_screen[j1][i1];
					DISPLAY->term_bits[j1][i1] = saved_scr->term_bits[j1][i1];
				}

				break;
			}
		}
	}

	DISPLAY->term_yloc = win->term_winy + win->term_yloc;
	DISPLAY->term_xloc = win->term_winx + win->term_xloc;
	
	return(OK);

}

/*
** This routine redraws the screen for a fresh start.
*/
redraw()
{
	int i, j;

/*
** "saved_scr" represents what the screen looked like after the last
** refresh (or wrefresh)
*/

	for (j = 0; j < (DISPLAY->term_height); j++)
	{
		for (i = 0; i < (DISPLAY->term_width); i++)
		{
				DISPLAY->term_screen[j][i] = saved_scr->term_screen[j][i];
				DISPLAY->term_bits[j][i] = saved_scr->term_bits[j][i];
		}
	}
	
	return(OK);

}

