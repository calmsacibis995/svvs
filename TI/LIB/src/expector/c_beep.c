# include "main.h"
# include <curses.h>
# include <stdio.h>

SCCSID("@(#)c_beep.c	3.1 (SVID) 12/1/86");

/*
** This routine will set a flag indicating that the screen has beeped.
** If the given terminal type does not allow for beeping, we set a flag
** saying the screen has flash. If that's not possible, we do nothing  
*/

c_beep()
{
	extern char *tigetstr();
	char *ch;

	if ((ch = tigetstr("bel")) != NULL)
	{
# ifdef TRACE
		if (Dbg)
		{
			printf(ch);
		}
# endif	/* TRACE */
		term_set(DISPLAY,TERM_BELL);
	}
	else if ((ch = tigetstr("flash")) != NULL)
	{
# ifdef TRACE
		if (Dbg)
		{	
			printf(ch);
		}
# endif	/* TRACE */
		term_set(DISPLAY,TERM_VB);
	}
	return(OK);
}


