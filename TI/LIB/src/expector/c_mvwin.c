# include "main.h"

SCCSID("@(#)c_mvwin.c	3.1 (SVID) 12/1/86");

c_winmv(win,y,x)
TERMINAL *win;
int x,y;

{
/*
** By changing the winy and winx coordinates, we change the position
** the window will be at when next it gets refreshed.
*/

	if (((y + win->term_height) <= DISPLAY->term_height) && 
	((x + win->term_width) <= DISPLAY->term_width))
	{
		win->term_winy = y;
		win->term_winx = x;
		return(OK);
	}
	else
	{
		msg_drop("ERROR: tried to move window outside of screen\n");
		return(ERR);
	}
}

