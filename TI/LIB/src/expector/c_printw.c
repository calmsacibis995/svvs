# include "main.h"
# include <stdio.h>
# define BIG 10000

SCCSID("@(#)c_printw.c	3.1 (SVID) 12/1/86");
extern int Dbg;

c_printw(frmtvar,argument)
VAR_ENTRY *frmtvar, *argument;
{
	VAR_ENTRY stdvar;

	stdvar.var_data.var_term = std;

	(void) c_wprintw(&stdvar,frmtvar,argument);

	return(OK);
}

c_wprintw(winvar,frmtvar,argument)
VAR_ENTRY *winvar, *frmtvar, *argument;
{
	extern int c_wstradd();
	extern	int sprintf();
	extern	char *malloc();
	char *result;
	char bigarray[BIG];
	TERMINAL *win;
	char *frmt;

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"Entered wprintw\n");
	}
# endif	/* TRACE */

	win = winvar->var_data.var_term;
	frmt = frmtvar->var_data.var_text_ptr;

/*
** sprintf just writes its last argument to its first one, using its
** second one as a format descriptor. If there's no argument, then
** everything to be printed is in the frmt descriptor, so we just
** copy that over.
*/

	if (!argument)
	{
		(void) sprintf(bigarray,frmt);
	}
	else
	{
		switch (argument->var_type)
		{
			case DATA_NULL:
				break;

			case DATA_NUM:
				(void) sprintf(bigarray,frmt,argument->var_data.var_num);
				break;
				
			case DATA_BOOL:
				(void) sprintf(bigarray,frmt,argument->var_data.var_bool);
				break;

			case DATA_TEXT:

				(void) sprintf(bigarray,frmt,argument->var_data.var_text_ptr);
				break;

			case DATA_CHAR:
# ifdef TRACE
				if (Dbg)
				{
					(void) fprintf(stderr,"wprintw: char arg's value is '%c'\n",argument->var_data.var_chtype);
					(void) fprintf(stderr,"wprintw: char arg's format is '%s'\n",frmt);
				} 
# endif	/* TRACE */
				(void) sprintf(bigarray,frmt,(argument->var_data.var_chtype) & A_CHARTEXT);
# ifdef TRACE
				if (Dbg)
				{
					(void) fprintf(stderr,"wprintw: char read in: '%c'\n",bigarray);
				}
# endif	/* TRACE */
				break;

			case DATA_TERM:
				(void) sprintf(bigarray,frmt,argument->var_data.var_term);
				break;

			case DATA_SCREEN:
				(void) sprintf(bigarray,frmt,argument->var_data.var_screen);
				break;

			case DATA_WINDOW:
				(void) sprintf(bigarray,frmt,argument->var_data.var_window);
				break;

		}

	}

# ifdef TRACE
		if (Dbg)
		{
			printf("about to call wstradd from wprintw\n");
		}
# endif	/* TRACE */
		
		result = malloc( (unsigned) strlen(bigarray) + 1);
		(void) strcpy(result,bigarray);
		
		(void) c_wstradd(win,result);
	
# ifdef TRACE
		if (Dbg)
		{
			printf("just returned from c_wstradd\n");
		}
# endif	/* TRACE */

		free(result);

	return(OK);
}

c_mvprintw(yvar,xvar,fmtvar,argvar)
VAR_ENTRY *yvar, *xvar, *fmtvar, *argvar;
{
	if (FirstCall)
	{
		(void) c_move(yvar,xvar);
	}

	(void) c_printw(fmtvar,argvar);
	return(OK);
}
 
c_mvwprintw(winvar,yvar,xvar,fmtvar,argvar)
VAR_ENTRY *winvar, *yvar, *xvar, *fmtvar, *argvar;
{
	TERMINAL *win;
	int y,x;

	if (FirstCall)
	{
		win = winvar->var_data.var_term;
		y = yvar->var_data.var_num;
		x = xvar->var_data.var_num;

		(void) c_wmove(win,y,x);
	}

	(void) c_wprintw(winvar,fmtvar,argvar);
	return(OK);
}

