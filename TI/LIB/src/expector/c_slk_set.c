# include "main.h"
# include <curses.h>

SCCSID("@(#)c_slk_set.c	3.1 (SVID) 12/1/86");

int c_slkset(labnum,label,fmt)
int labnum, fmt;
char *label;
{
	extern char *c_slklabel();
	int i, j;
	labeltbl[labnum].text = label;
	labeltbl[labnum].fmt = fmt;

/*
** c_slklabel strips leading and trailing blanks from the label.
*/

	labeltbl[labnum].text = c_slklabel(labnum);
	   
# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"Labelwidth = %d\n",Labelwidth);
	}
# endif	/* TRACE */


/*
** fmt = 0: label is left-justified
** fmt = 1: label is centered 
** fmt = 2: label is right-justified
*/

	switch (fmt) {	
	case 0:
		labeltbl[labnum].disp = 0;
		break;
	case 1:
		labeltbl[labnum].disp = (Labelwidth - strlen(labeltbl[labnum].text))/2;
		break;
	case 2:
		labeltbl[labnum].disp = Labelwidth - strlen(labeltbl[labnum].text);
		break;
	}

	status_line->term_xloc = labeltbl[labnum].pos + labeltbl[labnum].disp;

	for (i = labeltbl[labnum].pos;
	i < (labeltbl[labnum].pos + Labelwidth);
	i++)
	{
		status_line->term_screen[0][i] = ' ';
	}

	(void) c_wstradd(status_line,labeltbl[labnum].text);

	if (tigetstr("rev") != NULL)
	{
		for (i = 1; i <=8; i++)
		{
			for (j = 0; j <=Labelwidth-1; j++)
			{
				set_char_attrs(status_line, 0, labeltbl[i].pos+j, A_REVERSE);
			}
		}
	}

	if (tigetstr("dim") != NULL)
	{
		for (i = 1; i <=8; i++)
		{
			for (j = 0; j <=Labelwidth-1; j++)
			{
				set_char_attrs(status_line, 0, labeltbl[i].pos+j, A_DIM);
			}
		}
	}

	return(OK);
}
