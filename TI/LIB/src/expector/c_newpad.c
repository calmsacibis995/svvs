# include "main.h"

SCCSID("@(#)c_newpad.c	3.1 (SVID) 12/1/86");
TERMINAL *c_newpad(nlines,ncols)
int nlines, ncols;
{
	extern TERMINAL *c_newwin();
	TERMINAL *pad;

/*
** This routine just creates a new pad, which is basically a window
** which is not restricted in size to the size of the screen.
*/ 
	pad = c_newwin(nlines, ncols, 0, 0);
	pad->is_a_pad = TRUE;
	pad->last_pminrow = 0;
	pad->last_pmincol = 0;
	pad->last_sminrow = 0;
	pad->last_smincol = 0;
	pad->last_smaxrow = 0;
	pad->last_smaxcol = 0;
	pad->pminrow = 0;
	pad->pmincol = 0;
	pad->sminrow = 0;
	pad->smincol = 0;
	pad->smaxrow = 0;
	pad->smaxcol = 0;

	return(pad);
}

