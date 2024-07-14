# include "main.h"

SCCSID("@(#)c_flushinp.c	3.1 (SVID) 12/1/86");

c_flushinp()
{
/*
** This routine simply gets rid of all unread input and
** frees up the space it was taking up.
*/
	if (TypeAhead)
	{
		free(TypeAhead);
		TypeAhead = NULL;
	}	
	return(OK);
}

