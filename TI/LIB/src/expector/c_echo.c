# include "main.h"

SCCSID("@(#)c_echo.c	3.1 (SVID) 12/1/86");

c_echo()
{
/*
** If this flag is set, c_gtch will echo a character when read
*/

	Echo_on = TRUE; /* echo_on is a global */
	return(OK);
}

