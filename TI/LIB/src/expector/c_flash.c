
# include "main.h"
# include <curses.h>
# include <stdio.h>

SCCSID("@(#)c_flash.c	3.1 (SVID) 12/1/86");

/*
** This routine sets a flag indicating that the terminal has flashed.
** If the current terminal type does not allow for flashing, we set a
** flag saying the screen has beeped. If this is not possible, we do nothing.
*/
c_flash()
{
	char *ch;
	extern char *tigetstr();

	if ((ch = tigetstr("flash")) != NULL)
	{
# ifdef TRACE
		if (Dbg)
		{
	 		printf(ch);
		} 
# endif	/* TRACE */
		term_set(DISPLAY,TERM_VB);
	}
	else if ((ch = tigetstr("bel")) != NULL)
	{
# ifdef TRACE
		if (Dbg)
		{
	 		printf(ch);
		} 
# endif
		term_set(DISPLAY,TERM_BELL);
	}
	return(OK);
}


