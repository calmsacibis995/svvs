# include "main.h"

SCCSID("@(#)c_noecho.c	3.1 (SVID) 12/1/86");
c_noecho()
{
/*
** If this flag is set to FALSE, c_gtch will not echo a character 
** when read.
*/

	Echo_on = FALSE;    /* echo_on is a global */
	return(OK);
}

