# include "main.h"
extern TERMINAL *saved_scr;

SCCSID("@(#)c_endwin.c	3.1 (SVID) 12/1/86");

c_endwin()
{
/*
** Here we just set the virtual cursor to the bottom right-hand
** corner of the screen, clear the screen, and delete our two main windows.
*/

	DISPLAY->term_yloc = DISPLAY->term_height - 1;
	DISPLAY->term_xloc = 0;
	(void) c_delwin(std);
	(void) c_delwin(saved_scr);
	(void) c_delwin(DISPLAY);
	return(OK);
}
 
