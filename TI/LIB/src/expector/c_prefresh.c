# include "main.h"
# define MAXWINS	50
# include <stdio.h>

SCCSID("@(#)c_prefresh.c	3.1 (SVID) 12/1/86");

extern TERMINAL *windows[MAXWINS];
extern int next_empty;
extern TERMINAL *saved_scr;

c_prefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol)
TERMINAL *pad;
int pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol;
{
	if (pminrow < 0)
	{
		pminrow = 0;
	}

	if (pmincol < 0)
	{
		pmincol = 0;
	}

	if (sminrow < 0)
	{
		sminrow = 0;
	}

	if (smincol < 0)
	{
		smincol = 0;
	}

	if ((pminrow + (smaxrow - sminrow) < pad->term_height) &&			(pmincol + (smaxcol - smincol) < pad->term_width) &&				(smaxrow < DISPLAY->term_height) &&						(smaxcol < DISPLAY->term_width))	
	{
		(void) c_pnoutrefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol);
		(void) c_doupdate();
		return(OK);
	}
	else
	{
		msg_drop("ERROR in prefresh -- rectangle not completely contained in structure:\n");
		return(ERR);
	}
}
	
c_pnoutrefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol)
TERMINAL *pad;
int pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol;
{

/*
** This adds the given pad to the list of pads waiting to be
** written to the screen.
*/

	if (pminrow < 0)
	{
		pminrow = 0;
	}

	if (pmincol < 0)
	{
		pmincol = 0;
	}

	if (sminrow < 0)
	{
		sminrow = 0;
	}

	if (smincol < 0)
	{
		smincol = 0;
	}

	if (next_empty < MAXWINS)
	{
		pad->pminrow = pminrow;
		pad->pmincol = pmincol;
		pad->sminrow = sminrow;
		pad->smincol = smincol;
		pad->smaxrow = smaxrow;
		pad->smaxcol = smaxcol;
		windows[next_empty] = pad;
		next_empty +=1;
	}
	else
	{
		screech("c_pnoutrefresh",0,"ERROR in pnoutrefresh: pad/window limit exceeded \n");
	}
}

pad_update(pad)
TERMINAL *pad;
{
	TERMINAL *target;
	int i1, i2, j1, j2, i3, j2_lower_lim, j2_upper_lim, i2_lower_lim, i2_upper_lim, i3_lower_lim, i3_upper_lim;

/*
** If the given pad was initialized with "clear" rather than just
** "erase", the whole screen is cleared and redrawn before being updated.
*/

	if (term_on(pad,WIN_CLROK))
	{
		clrscr(); 
		term_clr(pad,WIN_CLROK);
		(void) redraw(); 
	}
 
/*
** If the given pad is a subpad, we need to find its parent pad,
** since that's the pad that contains the text to be copied.
*/

	target = pad;
	while (target->container != NULL)
	{
		target = target->container;
	}

	j2_lower_lim = pad->pminrow + (pad->term_winy - target->term_winy);
	j2_upper_lim = pad->pminrow + (pad->smaxrow - pad->sminrow) + (pad->term_winy - target->term_winy);

	i2_lower_lim = pad->pmincol + (pad->term_winx - target->term_winx); 
	i2_upper_lim = pad->pmincol + (pad->smaxcol - pad->smincol) + (pad->term_winx - target->term_winx);

	i3_lower_lim = i2_lower_lim;
	i3_upper_lim = i2_upper_lim;

	for (j1 = pad->sminrow, j2 = j2_lower_lim;
	(j1 <= pad->smaxrow) && (j2 <= j2_upper_lim);
	j1++, j2++)
	{	
		for (i2 = i2_lower_lim; 
		(i2 <= i2_upper_lim);
		i2++)
		{
	
/*
** If a character in the pad has not been changed since the last refresh of
** that pad, we don't need to copy it over again.
*/
			if ( (target->term_screen[j2][i2] != target->term_lastrefresh[j2][i2]) || (target->term_bits[j2][i2] != target->term_lastbits[j2][i2]) || (pad->pminrow != pad->last_pminrow) || (pad->pmincol != pad->last_pmincol) || (pad->sminrow != pad->last_sminrow) || (pad->smincol !=pad->last_smincol) || (pad->smaxrow != pad->last_smaxrow) || (pad->smaxcol != pad->last_smaxcol) )
			{
				for (i1 = pad->smincol, i3 = i3_lower_lim; 
				(i1 <= pad->smaxcol) && (i3 <= i3_upper_lim);
				i1++, i3++)
				{
					target->term_lastrefresh[j2][i3] = target->term_screen[j2][i3];
					target->term_lastbits[j2][i3] = target->term_bits[j2][i3];

					saved_scr->term_screen[j1][i1] = target->term_screen[j2][i3];
					saved_scr->term_bits[j1][i1] = target->term_bits[j2][i3];
	 				DISPLAY->term_screen[j1][i1] = saved_scr->term_screen[j1][i1];
					DISPLAY->term_bits[j1][i1] = saved_scr->term_bits[j1][i1];
				}	

				break;
			}
		}
	}

	pad->last_pminrow = pad->pminrow;
	pad->last_pmincol = pad->pmincol;
	pad->last_sminrow = pad->sminrow;
	pad->last_smincol = pad->smincol;
	pad->last_smaxrow = pad->smaxrow;
	pad->last_smaxcol = pad->smaxcol;

	if ( (pad->term_yloc < 0) || (pad->term_yloc >= c_LINES) || (pad->term_xloc < 0) || (pad->term_xloc >= c_COLS) )
	{
		DISPLAY->term_yloc = j1;
		DISPLAY->term_xloc = i1;
	}
	else
	{
		DISPLAY->term_yloc = pad->term_yloc;
		DISPLAY->term_xloc = pad->term_xloc;
	}
	
	return(OK);

}

