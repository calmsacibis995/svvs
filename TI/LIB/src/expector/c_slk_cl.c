# include "main.h"

struct status  
{
	char screen[MAXX];
	long bits[MAXX];
};
typedef struct status SAVED_STATUS;
SAVED_STATUS saved_line;

SCCSID("@(#)c_slk_cl.c	3.1 (SVID) 12/1/86");

/*
** This routine clears the status line from the screen. However,
** since we may want to restore it (with c_scrrestore), we save it
** in saved.lines, which is a static structure.
*/
c_slkclear()
{
	int i;
	extern TERMINAL *c_newwin();
	extern int c_slk_refresh();
	
	for (i = 0; i < status_line->term_width; i++)
	{
		saved_line.screen[i] = status_line->term_screen[0][i];
		saved_line.bits[i] = status_line->term_bits[0][i];
		
		status_line->term_screen[0][i] = ' '; 
		status_line->term_bits[0][i] = 0L;
	}

 	(void) c_slk_refresh();

	return(OK);
}

