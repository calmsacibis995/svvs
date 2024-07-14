# include "main.h"

SCCSID("@(#)c_touchwin.c	3.1 (SVID) 12/1/86");
c_wintouch(win)
TERMINAL *win;
{
	int i,j, i_lower, i_upper, j_lower, j_upper;
	TERMINAL *target;

	target = win; 

	/*
	** If the given window is a subwindow, we must find the parent window,
	** since that's where the text is stored.
	*/

	while (target->container != NULL)
	{
		target = target->container;
	}

	target->touched = TRUE;

	/*   
	** filling each element of the window "last_refresh" area with a non-ascii 
	** character and each element of "last_bits" with TERM_NULL (which is not
	** an attribute bit-value insures that when the next wrefresh is called for
	** this window, the whole window will be copied afresh.
	*/
 
	i_lower = win->term_winx - target->term_winx;
	i_upper = i_lower + win->term_width;

	j_lower = win->term_winy - target->term_winy;
	j_upper = j_lower + win->term_height;

	for (j = j_lower; j < j_upper;  j++)
	{
		for (i = i_lower; i < i_upper; i++)
		{
			target->term_lastrefresh[j][i] = -1;
			target->term_lastbits[j][i] = TERM_NULL;
		}
	}

	return(OK);
}

c_linetouch(win, start, count)
TERMINAL *win;
int start, count;
{
	int i, j, i_lower, i_upper, j_lower, j_upper;
	TERMINAL *target;

	target = win; 

/*
** If the given window is a subwindow, we must find the parent window,
** since that's where the text is stored.
*/

	while (target->container != NULL)
	{
		target = target->container;
	}

	target->touched = TRUE;

/*   
** filling the window's "last_refresh" area with a non-ascii 
** character and its "last_bits" field with TERM_NULL (which is not
** an attribute bit-value) for "count" lines starting at line "start"
** insures that when the next wrefresh is called for
** this window, those "count" lines will be copied afresh.
*/

	i_lower = win->term_winx - target->term_winx;
	i_upper = (win->term_winx - target->term_winx) + win->term_width;

	j_lower = start + win->term_winy - target->term_winy;
	j_upper = (win->term_winy - target->term_winy) + count;

	for (j = j_lower; j < j_upper;  j++)
	{
		for (i = i_lower; i < i_upper; i++)
		{
			target->term_lastrefresh[j][i] = -1;
			target->term_lastbits[j][i] = TERM_NULL;
		}
	}

	return(OK);
}

