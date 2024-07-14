# include "main.h"

SCCSID("@(#)c_scrollok.c	3.1 (SVID) 12/1/86");
c_okscroll(win,bf) 
TERMINAL *win;
int bf;
{

/*
** Setting this flag allows scrolling to be done when attempts are made
** to add text past the end of the scrolling region, window, or screen.
*/

	if (bf)
	{
		term_set(win,WIN_SCRLOK);
	}
	else
	{
		term_clr(win,WIN_SCRLOK);
	}

	return(OK);
}

