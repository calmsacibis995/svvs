# include "main.h"

SCCSID("@(#)c_erase.c	3.1 (SVID) 12/1/86");

/*
** This routine is just like c_clear (which blanks out a window and its     
** attributes) except that it doesn't arrange for the screen to be redrawn
** upon the next refresh of the window.
*/
c_erase()
{

	ARGS *arg;
	int i,j;

	arg = NULL;

/*
** "cl_func" (found in "bless.a") simply blanks out the given window. 
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

	return(OK);
}

c_werase(win)
TERMINAL *win;
{
	(void) c_wclear(win);

	(void) c_clrok(win,FALSE);

	return(OK);

}
 
