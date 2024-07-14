# include "main.h"

SCCSID("@(#)c_clearok.c	3.1 (SVID) 12/1/86");

/*
** Calling this routine will cause the screen to be cleared and redrawn
** upon the next call to wrefresh with the given window.
*/

c_clrok(win,bf)	/* catcher's version of clearok */ 
TERMINAL *win;
int bf;
{

	if (bf) 
	{
		term_set(win,WIN_CLROK);
	}
	else
	{
		term_clr(win,WIN_CLROK);
	}

	return(OK);
}

