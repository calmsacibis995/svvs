/*
** barber.c
**
**	contains wbarber() - fill a window with a 'barber pole'
**			     of the character set
*/

# include <curses.h>
# include <ctype.h>
# include <sccs.h>

SCCSID("@(#)wbarber.c	3.1 (SVVS) 12/1/86");

# define SPACE '\040'	/* first printable ascii char */
# define TILDE '\176'	/* last printable ascii char */

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
wbarber(win, lin, col, startch)
WINDOW *win;
int lin;
int col;
chtype startch;
{
	int l;
	int c;
	chtype firstch;
	chtype currch;

	wmove(win, 0, 0);
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
			if (waddch(win, currch) != OK)
				return(FALSE);
			if (c == col)
			{
				if (firstch == TILDE)
					firstch = SPACE;
				else
					firstch++;
				currch = firstch;
			}
			else
				if (currch == TILDE)
					currch = SPACE;
				else
					currch++;
		}
	}
	return(TRUE);
}
