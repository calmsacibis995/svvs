# include "main.h"

SCCSID("@(#)c_copywin.c	3.1 (SVID) 12/1/86");

c_copywin(srcwin,dstwin,sminrow,smincol,dminrow,dmincol,dmaxrow,dmaxcol,overlay)
TERMINAL *srcwin, *dstwin;
int sminrow, smincol, dminrow, dmincol, dmaxrow, dmaxcol, overlay;
{
	int i1,i2,j1,j2;

	/*
	** This routines allows a portion of srcwin (delineated by sminrow and smincol)
	** to be copied onto a particular area of dstwin, specified by dminrow, dmincol,
	** dmaxrow, and dmaxcol.
	*/

	for (j1 = dminrow, j2 = sminrow;
	(j1 <= dmaxrow) && (j2 < srcwin->term_height);
	j1++,j2++)
	{
		for (i1 = dmincol, i2 = smincol;
		(i1 <= dmaxcol) && (i2 < srcwin->term_width);
		i1++, i2++)
		{
			if (!((srcwin->term_screen[j2][i2] == ' ') && (overlay)))
			{
				dstwin->term_screen[j1][i1] = srcwin->term_screen[j2][i2];
				dstwin->term_bits[j1][i1] = srcwin->term_bits[j2][i2];
# ifdef TRACE
				if (Dbg)
				{   
					printf("dstwin->term_bits[%d][%d] = '%ld'\n",j1,i1,dstwin->term_bits[j1][i1]);
				}
# endif	/* TRACE */
			}
		}
	}

	return(OK);
}

				
	
