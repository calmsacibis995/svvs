# include "main.h"

SCCSID("@(#)c_scroll.c	3.1 (SVID) 12/1/86");
c_scroll(win)
TERMINAL *win;
{
	TERMINAL *target;

	target = win;

/*
** If the given window is a subwindow, we must find the parent window, since
** that is where the text to be scrolled is.
*/

	while (target->container != NULL)
	{
		target = target->container;
	}

/*
** scroll_win is found in c_addch.c
*/

	if (term_on(win,WIN_SCRLOK))
	{
		scroll_win(target,win);
		return(OK);
	}
	else
	{
		msg_drop("ERROR in scroll: attempt to scroll window without calling scrollok on window\n");
		return(ERR);
	}

}

