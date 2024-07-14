# include "main.h"

SCCSID("@(#)c_pecho.c	3.1 (SVID) 12/1/86");
c_pechochar(pad,ch)
TERMINAL *pad;
chtype ch;
{
/*
** This routine is analagous to "c_charecho"
*/
	(void) c_wchadd(pad,ch);
	(void) c_prefresh(pad,pad->last_pminrow,pad->last_pmincol,pad->last_sminrow,pad->last_smincol,pad->last_smaxrow,pad->last_smaxcol);
	return(OK);
}

