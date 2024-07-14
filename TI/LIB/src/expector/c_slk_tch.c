# include "main.h"

SCCSID("@(#)c_slk_tch.c	3.1 (SVID) 12/1/86");
c_slktouch()
{
	(void) c_wintouch(status_line);
	return(OK);
}

