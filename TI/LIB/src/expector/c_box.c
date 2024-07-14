# include "main.h"
# include <curses.h>

SCCSID("@(#)c_box.c	3.1 (SVID) 12/1/86");

/*
** "c_box" draws a border around the given window, with the character "hor"
** on the top and bottom, and "vert" on the sides.
*/

c_box(win, vert, hor)
TERMINAL *win;
chtype vert,hor;
{
	int y, x, y_lower_lim, x_lower_lim, y_upper_lim, x_upper_lim;
	TERMINAL *target;

	if (vert == 0)
	{
		vert = ACS_VLINE;
	}

	if (hor == 0)
	{
		hor = ACS_HLINE;
	}

	/*
	** If the given window is a subwindow, we must find the parent window, since
	** that's where the text is.
	*/

	target = win;
	while (target->container != NULL)
	{
		target = target->container;
	}

	/*
	** Here, the top and bottom borders of the window are marked with the "hor"
	** character.
	*/

	x_lower_lim = win->term_winx - target->term_winx;
	x_upper_lim = x_lower_lim + win->term_width;
	y_lower_lim = win->term_winy - target->term_winy;
	y_upper_lim = y_lower_lim + win->term_height;

	for (x = x_lower_lim;
	x < x_upper_lim;
	x++)
	{
		target->term_screen[y_lower_lim][x] = (hor & A_CHARTEXT);
		target->term_bits[y_lower_lim][x] = (hor & A_ATTRIBUTES);

		target->term_screen[y_upper_lim - 1][x] = (hor & A_CHARTEXT);
		target->term_bits[y_upper_lim - 1][x] = (hor & A_ATTRIBUTES);
	}

	/*
	** Here, the left and right borders of the window are marked with the "vert"
	** character.
	*/

	for (y = y_lower_lim;
	y  < y_upper_lim;
	y++)
	{
		target->term_screen[y][x_lower_lim] = (vert & A_CHARTEXT);
		target->term_bits[y][x_lower_lim] = (vert & A_ATTRIBUTES);

		target->term_screen[y][x_upper_lim - 1] = (vert & A_CHARTEXT);
		target->term_bits[y][x_upper_lim - 1] = (vert & A_ATTRIBUTES);
	}

	/*
	** The corners of the window are filled with '+'
	*/

	target->term_screen[y_lower_lim][x_lower_lim] = '+';
	target->term_screen[y_lower_lim][x_upper_lim - 1] = '+';
	target->term_screen[y_upper_lim - 1][x_lower_lim] = '+';
	target->term_screen[y_upper_lim - 1][x_upper_lim - 1] = '+';

	return(OK);

}

