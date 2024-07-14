/*
** barber.c
**
**	contains wbarber() - fill a window with a 'barber pole'
**			     of the character set
*/
# include "main.h"
# include <curses.h>
# include <ctype.h>

SCCSID("@(#)c_barber.c	3.1 (SVID) 12/1/86");
/*
** wbarber()
**
**	fills a window with a 'barber pole' of the character set
**
**	parameters	- pointer to the window to fill
**			  number of lines to fill
**			  number of columns to fill
**			  character to begin first line with
**
**	returns		- 1 if successful
**			  0 if it failed
*/
c_wbarber(win, lin, col, startch)
TERMINAL *win;
int lin;
int col;
chtype startch;
{
	int l;
	int c;
	chtype firstch;
	chtype currch;
	extern int c_wchadd();

	(void) c_wmove(win, 0, 0);
	lin--;
	col--;
	firstch = startch;
	currch = startch;
	if (!isprint(startch))
		return(FALSE);
	for (l = 0; l <= lin; l++)
	{
		for (c = 0; c <= col; c++)
		{
			if (l == lin && c == col)
				break;
			if (c_wchadd(win, (chtype) currch) != OK)
				return(FALSE);
			if (c == col)
			{
				if (firstch == '\176')
					firstch = '\040';
				else
					firstch++;
				currch = firstch;
			}
			else
				if (currch == '\176')
					currch = '\040';
				else
					currch++;
		}
	}
	return(TRUE);
}

c_barber(ch)
chtype ch;
{
	(void) c_wbarber(std,c_LINES,c_COLS,(int) ch);
	return(OK);
}

