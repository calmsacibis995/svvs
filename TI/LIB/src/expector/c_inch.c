# include "main.h"
# include <curses.h>

SCCSID("@(#)c_inch.c	3.1 (SVID) 12/1/86");

c_inc()	/* catcher's version of inch */
{
	chtype value;

/*
** Here, we just take whatever's at the current cursor location 
** on the screen, OR its attributes into itself, and return it.
*/

	value = (chtype) std->term_screen[std->term_yloc][std->term_xloc];

	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_SO))
	{
		value |= A_STANDOUT;
	}

	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_REV))
	{
		value |= A_REVERSE;
	}

	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_BOLD))
	{
		value |= A_BOLD;
	}

	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_HALF))
	{
		value |= A_DIM;
	}

	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_BLINK))
	{
		value |= A_BLINK;
	}
	
	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_UL))
	{
		value |= A_UNDERLINE;
	}

	if (t_bit_on(std,std->term_yloc,std->term_xloc,TERM_ALTCH))
	{
		value |= A_ALTCHARSET;
	}

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"c_inc is returning '%c'\n",(char ) (value & A_CHARTEXT));
	}
# endif	/* TRACE */

	return(value);
}

c_wchin(winvar)	/* catcher's version of winch */
VAR_ENTRY *winvar;
{
	TERMINAL *win, *target;
	chtype value;
	int x_current, y_current;

	win = winvar->var_data.var_term;
	target = win;

/*
** If the given window is a subwindow, we must find the parent window, since
** that's where the text is stored.
*/

	while (target->container != NULL)
	{
		target = target->container;
	}


	y_current = (win->term_winy - target->term_winy) + win->term_yloc;
	x_current = (win->term_winx - target->term_winx) + win->term_xloc;

	value = (chtype ) target->term_screen[y_current][x_current];


	if (t_bit_on(target, y_current, x_current, TERM_SO))
	{
		value |= A_STANDOUT;
	}

	if (t_bit_on(target, y_current, x_current, TERM_REV))
	{
		value |= A_REVERSE;
	}

	if (t_bit_on(target, y_current, x_current, TERM_BOLD))
	{
		value |= A_BOLD;
	}

	if (t_bit_on(target, y_current, x_current, TERM_HALF))
	{
		value |= A_DIM;
	}

	if (t_bit_on(target, y_current, x_current, TERM_BLINK))
	{
		value |= A_BLINK;
	}
	
	if (t_bit_on(target, y_current, x_current, TERM_UL))
	{
		value |= A_UNDERLINE;
	}

	if (t_bit_on(target, y_current, x_current, TERM_ALTCH))
	{
		value |= A_ALTCHARSET;
	}

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"c_wchin is returning '%c'\n",(char ) (value & A_CHARTEXT));
	}
# endif	/* TRACE */

	return(value);
}

c_inchmv(yvar,xvar)	/* catcher's version of mvinch */ 
VAR_ENTRY *yvar, *xvar;
{
		(void) c_move(yvar,xvar);

	return(c_inc());
}

c_chinmvw(winvar,yvar,xvar)	/* catcher's version of mvwinch */
VAR_ENTRY *winvar, *yvar, *xvar;
{
	int y,x;
	TERMINAL *win;

	win = winvar->var_data.var_term;
	y = yvar->var_data.var_num;
	x = xvar->var_data.var_num;
	
	(void) c_wmove(win,y,x);

	return(c_wchin(winvar));

}

