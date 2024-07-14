# include "main.h"
# include <curses.h>

SCCSID("@(#)c_echochar.c	3.1 (SVID) 12/1/86");

c_charecho(ch)
chtype ch;
{
	VAR_ENTRY chvar;

	chvar.var_data.var_chtype = ch;

	(void) c_adch(&chvar);
	c_refresh();

	return(OK);
} 

c_wechochar(win,ch) 
TERMINAL *win;
chtype ch;
{
	(void) c_wchadd(win,ch);

	(void) c_wrefresh(win);

	return(OK);
}

