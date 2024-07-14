# include "main.h"

SCCSID("@(#)c_subpad.c	3.1 (SVID) 12/1/86");
TERMINAL *c_subpad(orig,nlines,ncols,begin_y,begin_x) 
TERMINAL *orig;
int nlines, ncols, begin_y, begin_x;
{
	extern TERMINAL *c_subwin();
	TERMINAL *sub;

/*
** This routine is to "c_newpad" as "c_subwin" is to "c_newwin"
*/
	sub = c_subwin(orig, nlines, ncols, begin_y, begin_x);
	sub->is_a_pad = TRUE;
	sub->last_pminrow = 0;
	sub->last_pmincol = 0;
	sub->last_sminrow = 0;
	sub->last_smincol = 0;
	sub->last_smaxrow = 0;
	sub->last_smaxcol = 0;
	sub->pminrow = 0;
	sub->pmincol = 0;
	sub->sminrow = 0;
	sub->smincol = 0;
	sub->smaxrow = 0;
	sub->smaxcol = 0;

	return(sub);
}

