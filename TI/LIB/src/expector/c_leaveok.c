# include "main.h"

SCCSID("@(#)c_leaveok.c	3.1 (SVID) 12/1/86");

c_leaveok(win,bf)
TERMINAL *win;
int bf;
{

/*
** Setting this flag allows the screen cursor to be left wherever
** it's at after the next wrefresh with the given window.
*/ 

	if (bf)
	{
		term_set(win,WIN_LVOK);
	}
	else
	{
		term_clr(win,WIN_LVOK);
	}

	return(OK);
}

