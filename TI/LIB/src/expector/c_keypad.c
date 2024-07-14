# include "main.h"

SCCSID("@(#)c_keypad.c	3.1 (SVID) 12/1/86");
c_keypad(win,bf)
TERMINAL *win;
BOOLEAN bf;
{
/*
** When this flag is set to TRUE, function keys entered as input will
** return value representing that function. If FALSE, the sequence
** produced by the key will not be interpreted specially.
*/
	if (bf)
	{
		win->keypad_on = TRUE;
	}
	else
	{
		win->keypad_on = FALSE;
	}

	return(OK);
}
