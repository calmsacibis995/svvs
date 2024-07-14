# include "main.h"

SCCSID("@(#)c_slk_rest.c	3.1 (SVID) 12/1/86");

struct status  
{
	char screen[MAXX];
	long bits[MAXX];
};
typedef struct status SAVED_STATUS;

/*
** This routine re-fills the status line with what was saved from the
** last c_slkclear (and placed in the static structure saved_line)
** and refreshes it so it will re-appear on the screen.
*/
c_slkrestore()
{
	int i;
	extern SAVED_STATUS saved_line;

	for (i = 0; i < status_line->term_width; i++)
	{
		status_line->term_screen[0][i] = saved_line.screen[i];
		status_line->term_bits[0][i] = saved_line.bits[i];
	}

	(void) c_wrefresh(status_line);
	return(OK);
}

