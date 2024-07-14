# include "main.h"
# include <curses.h>
SCCSID("@(#)c_newwin.c	3.1 (SVID) 12/1/86");
extern char *malloc();

die(val,msg) 
char *msg;
char *val;
{
	if (val == NULL)
	{
		screech("c_newwin",0,msg);
	}
}

TERMINAL *c_newwin(nlines,ncols,begin_y,begin_x) 
int nlines, ncols, begin_y, begin_x;
{
	TERMINAL *term;
	int i;

/* 
** The type TERMINAL represents a window of any kind -- subwindow, parent
** window, terminal screen. It has space for each character on the window,
** plus attributes pertaining to each character, the cursor, and the 
** screen in general.
*/

	term = (TERMINAL *) malloc((unsigned) sizeof(TERMINAL));
	die(term,"c_newwin: malloc(term) died \n");	
	term->subws = (SUBWINDOWS *) malloc((unsigned) sizeof(SUBWINDOWS));

	die(term->subws,"c_newin: malloc(term->subws) died \n");	
	
	if (ncols == 0)
	{
		term->term_width = tgetnum("co") - begin_x;
	}
	else
	{
		term->term_width = ncols;
	}

	if (nlines == 0)
	{
		term->term_height = tgetnum("li") - begin_y;
	}
	else
	{
		term->term_height = nlines;
	}

/*
** The size of textual portion of the terminal (term->term_screen) is 
** dynmically allocated here for space efficiency.
*/

	term->term_screen = (char **) calloc((unsigned) term->term_height+1,sizeof(char *));
	die((char *) term->term_screen,"c_newwin: calloc(term->term_screen) died \n");	
	for (i = 0; i < term->term_height+1; i++)
	{
		term->term_screen[i] = (char *) calloc((unsigned) term->term_width+1,1);
		die((char *) term->term_screen[i],"c_newwin: calloc(term->term_screen[i]) died \n");	
	}

	term->term_lastrefresh = (char **) calloc((unsigned) term->term_height+1,sizeof(char *));
	die((char *) term->term_lastrefresh,"c_newwin: calloc(term->term_lastrefresh) died \n");	

	for (i = 0; i < term->term_height+1; i++)
	{
		term->term_lastrefresh[i] = (char *) calloc((unsigned) term->term_width+1,1);
		die((char *) term->term_lastrefresh[i],"c_newwin: calloc(term->term_lastrefresh[i]) died \n");	
	}
	
	term->term_bits = (long **) calloc((unsigned) term->term_height+1,sizeof(char *));
	die((char *) term->term_bits,"c_newwin: calloc(term->term_bits) died \n");	
	for (i = 0; i < term->term_height+1; i++)
	{
		term->term_bits[i] = (long *) calloc((unsigned) term->term_width+1,sizeof(char *));
		die((char *) term->term_bits[i],"c_newwin: calloc(term->term_bits[i]) died \n");	
	}	

	term->term_lastbits = (long **) calloc((unsigned) term->term_height+1,sizeof(char *));
	die((char *) term->term_lastbits,"c_newwin: calloc(term->term_lastbits) died \n");	

	for (i = 0; i < term->term_height+1; i++)
	{
		term->term_lastbits[i] = (long *) calloc((unsigned) term->term_width+1,sizeof(char *));
		die((char *) term->term_lastbits[i],"c_newwin: calloc(term->term_lastbits[i]) died \n");	
	}	
	
	term->term_flag = 0L;
	term->term_xloc = 0;
	term->term_yloc = 0;
	term->term_winx = begin_x;
	term->term_winy = begin_y;
	term->scrl_top = 0;
	term->scrl_bot = term->term_height-1;
	term->subws->subwin = NULL;
	term->subws->next = NULL;
	term->touched = FALSE;
	term->container = NULL;
	term->keypad_on = FALSE;
	term->is_a_pad = FALSE;
	(void) c_wintouch(term);
	(void) c_werase(term);
	return(term);
}

