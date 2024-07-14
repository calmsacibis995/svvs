# include "main.h"
# include <curses.h> 

SCCSID("@(#)c_slk_init.c	3.1 (SVID) 12/1/86");

c_slkinit(fmt) 
int fmt;
{
	extern TERMINAL *c_newwin();
	int i, j;

/*
** The status line is defined as a window that's one character high,
** and as wide as the terminal screen.
*/

	status_line = c_newwin(1,tgetnum("co"),tgetnum("li") - 1,0);

	Labelwidth = (status_line->term_width / 8) - 1;
	if (Labelwidth > 8)
	{
		Labelwidth = 8; 
	}

	(void) c_wclear(status_line);

/*
** Whether the label format is 4-4 or 3-2-3, the first, second, third, sixth,
** seventh, and eighth labels have the same positions on the line, so we
** can compute those positions now (relative to the width of the terminal
** screen).
*/

	labeltbl[1].pos = 0; 
	labeltbl[2].pos = Labelwidth + 1; 
	labeltbl[3].pos = (Labelwidth + 1) * 2; 
	labeltbl[6].pos = status_line->term_width - (labeltbl[2].pos*3);
	labeltbl[7].pos = status_line->term_width - (labeltbl[2].pos*2); 
	labeltbl[8].pos = status_line->term_width - labeltbl[2].pos;

	if (fmt == 1) /* 4-4 format */
	{
		labeltbl[4].pos = labeltbl[2].pos * 3; 
		labeltbl[5].pos = status_line->term_width - (labeltbl[2].pos*4); 
	}
	else if (fmt == 0) /* 3-2-3 format */
	{
		labeltbl[5].pos = (status_line->term_width)/2;
		labeltbl[4].pos = labeltbl[5].pos - labeltbl[2].pos;
	}

	if ( tigetstr("rev") != NULL)
	{
		for (i = 1; i <=8; i++)
		{
			for (j = 0; j <= Labelwidth - 1; j++)
			{
				set_char_attrs(status_line,0,labeltbl[i].pos + j,A_REVERSE);
			}
		}
	}

	if ( tigetstr("dim") != NULL)
	{
		for (i = 1; i <=8; i++)
		{
			for (j = 0; j <= Labelwidth - 1; j++)
			{
				set_char_attrs(status_line,0,labeltbl[i].pos + j,A_DIM);
			}
		}
	}

	Status_line_initialized = TRUE;
	return(OK);
}

