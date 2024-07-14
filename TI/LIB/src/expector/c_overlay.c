# include "main.h"

SCCSID("@(#)c_overlay.c	3.1 (SVID) 12/1/86");

/*
** These routines simply copy one window (srcwin) on top of another (dstwin).
** The only difference is that "ovrwrite" also copies blanks.
*/

c_ovrlay(srcwin,dstwin)	/* catcher's version of overlay */
TERMINAL *srcwin, *dstwin;
{
	int i1, i2, j1, j2;

	if ( (dstwin->term_winy > (srcwin->term_winy+srcwin->term_height-1)) ||		((dstwin->term_winy+dstwin->term_height-1) < srcwin->term_winy) ||		(dstwin->term_winx > (srcwin->term_winx+srcwin->term_width-1)) ||		((dstwin->term_winx+dstwin->term_width-1) < srcwin->term_winx) )
	{
		return(OK);
	}

	for (j1 = 0;
	(j1 < dstwin->term_height) && ((j1 + dstwin->term_winy) < (srcwin->term_winy + srcwin->term_height));
	j1++)
	{
		for (i1 = 0;
		(i1 < dstwin->term_width) && ((i1 + dstwin->term_winx) < (srcwin->term_winx + srcwin->term_width));
		i1++)
		{
			if (((j1 + dstwin->term_winy) >= srcwin->term_winy) && 				((i1 + dstwin->term_winx) >= srcwin->term_winx) &&				(srcwin->term_screen[dstwin->term_winy + j1 - srcwin->term_winy][dstwin->term_winx + i1 - srcwin->term_winx] != ' '))	
			{
				dstwin->term_screen[j1][i1] = srcwin->term_screen[dstwin->term_winy + j1 - srcwin->term_winy][dstwin->term_winx + i1 - srcwin->term_winx];

				dstwin->term_bits[j1][i1] = srcwin->term_bits[dstwin->term_winy + j1 - srcwin->term_winy][dstwin->term_winx + i1 - srcwin->term_winx]; 
			}
		}
	}

	return(OK);
}

c_ovrwrite(srcwin,dstwin)	/* catcher's version of overwrite */
TERMINAL *srcwin, *dstwin;
{
	int i1, i2, j1, j2;

	if ( (dstwin->term_winy > (srcwin->term_winy+srcwin->term_height-1)) ||		((dstwin->term_winy+dstwin->term_height-1) < srcwin->term_winy) ||		(dstwin->term_winx > (srcwin->term_winx+srcwin->term_width-1)) ||		((dstwin->term_winx+dstwin->term_width-1) < srcwin->term_winx) )
	{
		return(OK);
	}

	for (j1 = 0;
	(j1 < dstwin->term_height) && ((j1 + dstwin->term_winy) < (srcwin->term_winy + srcwin->term_height));
	j1++)
	{
		for (i1 = 0;
		(i1 < dstwin->term_width) && ((i1 + dstwin->term_winx) < (srcwin->term_winx + srcwin->term_width));
		i1++)
		{
			if (((j1 + dstwin->term_winy) >= srcwin->term_winy) && 				((i1 + dstwin->term_winx) >= srcwin->term_winx)) 
			{
				dstwin->term_screen[j1][i1] = srcwin->term_screen[dstwin->term_winy + j1 - srcwin->term_winy][dstwin->term_winx + i1 - srcwin->term_winx]; 

				dstwin->term_bits[j1][i1] = srcwin->term_bits[dstwin->term_winy + j1 - srcwin->term_winy][dstwin->term_winx + i1 - srcwin->term_winx]; 
			}
		}
	}

	return(OK);
}

