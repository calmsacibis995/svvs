# include "main.h"

SCCSID("@(#)c_mvcur.c	3.1 (SVID) 12/1/86");
c_mvcur(oldrow, oldcol, newrow, newcol)
int oldrow, oldcol, newrow, newcol;
{
	if ( (newrow < 0) || (newrow >= c_LINES) || (newcol < 0) || (newcol >= c_COLS) )
	{
		msg_drop("ERROR in mvcur: attempted to move cursor off screen\n");
		return(ERR);
	}

	DISPLAY->term_yloc = newrow;
	DISPLAY->term_xloc = newcol;

	return(OK);
}
