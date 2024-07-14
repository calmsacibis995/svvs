# include "main.h"

SCCSID("@(#)c_slk_refr.c	3.1 (SVID) 12/1/86");

c_slk_refresh()
{
	(void) c_slknoutrefresh();
	(void) c_doupdate();
	(void) c_slktouch();
	return(OK);
}

	
