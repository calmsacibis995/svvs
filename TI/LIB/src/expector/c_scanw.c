# include "main.h"
# include <malloc.h>

SCCSID("@(#)c_scanw.c	3.1 (SVID) 12/1/86");
static char *LocalCopy, *NewCopy;

/*
** This routine reads in an input string and writes it to the given window
*/
echo_input(win) 
TERMINAL *win;
{
char *tmp_str;
VAR_ENTRY winvar, strvar;

/*
** tmp_str will contain the string read in by "getstr"
*/
	tmp_str = (char *) malloc((unsigned) (strlen(TypeAhead) + strlen(KB_Buffer) + 1));
	die(tmp_str,"echo_input (called from c_scanw): malloc of tmp_str failed\n");

# ifdef TRACE
	if (Dbg)
	{
		printf("in echo_input: tmp_str = %x \n",tmp_str);
	}
# endif	/* TRACE */

	winvar.var_data.var_term = win;
	strvar.var_data.var_text_ptr = tmp_str;

	(void) c_wgetstr(&winvar,&strvar);
	LocalCopy = strvar.var_data.var_text_ptr;

/* 
** For purposes of scanning, the string is terminated with a newline.
*/

	LocalCopy = realloc(LocalCopy,(unsigned) (strlen(LocalCopy) + 2));
	die(LocalCopy, "echo_input(called by c_wscanw): realloc of LocalCopy failed\n");

	(void) strcat(LocalCopy,"\n");
}

c_wscanw(winvar,fmtvar,argvar)
VAR_ENTRY *winvar, *fmtvar, *argvar;
{
	char *new_fmt;
	char *fmt, ch;
	int	result;

	fmt = fmtvar->var_data.var_text_ptr;

	if ((new_fmt = malloc((unsigned) (strlen(fmt) + 7))) == NULL)
	{
		screech("c_scanw",0,"ERROR in c_scanw: malloc  of new_fmt failed\n");
	}

/*
** For the first call to scanw, we must read in the input string and
** (if the echo flag is set) copy it to the given window.
*/

	if (FirstCall)
	{
		echo_input(winvar->var_data.var_term);
	}

	if (!LocalCopy)
	{
		screech("c_scanw",0,"ERROR in c_scanw: No input left to read\n");
	}

	if ((LocalCopy)[strlen(LocalCopy) - 1] != '\n')
	{
		screech("c_scanw",0,"ERROR in scanw: Input not \\n terminated\n");
	}
 
/*
** The "new_fmt" indicates that we will scan up to, but not including, the
** newline character.
*/

	(void) strcpy(new_fmt,fmt);
	(void) strcat(new_fmt,"%[^\n]");

/*
** "NewCopy" will contain the unscanned portion of the input.
*/

	NewCopy = malloc((unsigned) (strlen(LocalCopy) + 3));
	die(NewCopy, "c_wscanw: malloc of NewCopy failed\n");

	switch (argvar->var_type)
	{
		case DATA_NULL:
			break;

		case DATA_NUM:
			result = sscanf(LocalCopy,new_fmt,&(argvar->var_data.var_num),NewCopy);
			break;

		case DATA_BOOL:
			result = sscanf(LocalCopy,new_fmt,&(argvar->var_data.var_bool),NewCopy);
			break;

		case DATA_TEXT:

			result = sscanf(LocalCopy,new_fmt,argvar->var_data.var_text_ptr,NewCopy);
			break;

		case DATA_CHAR:
			result = sscanf(LocalCopy,new_fmt,&ch,NewCopy);
			argvar->var_data.var_chtype = (chtype) ch;
			break;

		case DATA_WINDOW:
			result = sscanf(LocalCopy,new_fmt,argvar->var_data.var_window,NewCopy);
			break;

		case DATA_SCREEN:
			result = sscanf(LocalCopy,new_fmt,argvar->var_data.var_screen,NewCopy);
			break;

		case DATA_TERM:
			result = sscanf(LocalCopy,new_fmt,argvar->var_data.var_term,NewCopy);
			break;

	}

	free(new_fmt);

	switch (result)
	{
	  case 2:
	
		/* This is the "regular" case. We take the unscanned
		** portion of the input, make it the "new input",
		** and terminate it with a newline, in order to prepare
		** it for the next call to scan.
		*/

		free(LocalCopy);
		LocalCopy = NewCopy;
		(void) strcat(LocalCopy,"\n");
		break;

	  case 1:
# ifdef TRACE
		if (Dbg)
		{
			printf("done scanning the input string \n");
		}
# endif	/* TRACE */

		/*
		** In this case, There is no unused portion of input,
		** so we're through.
		*/

		free(LocalCopy);
		LocalCopy = NULL;
		break;

	  case 0:

		/* 
		** In this case, nothing was scanned, so we just send the      
		** whole thing (i.e. "LocalCopy") back to scanw for another
		** try.
		*/

		free(NewCopy);
		break;

	  default:

		/* 
		** In this case, something is irreparably wrong, so
		** we abort.
		*/
		screech("c_scanw",0,"undetermined ERROR in c_scanw -- ABORT\n");
		break;
	}

	return(OK);
}

c_scanw(fmtvar,argvar)
VAR_ENTRY *fmtvar, *argvar;
{
	VAR_ENTRY std_var;

	std_var.var_data.var_term = std;

	(void) c_wscanw(&std_var,fmtvar,argvar);

	return(OK);
}

c_mvscanw(yvar,xvar,fmtvar,argvar)
VAR_ENTRY *yvar, *xvar, *fmtvar, *argvar;
{
	if (FirstCall)
	{
		(void) c_move(yvar,xvar);
	}

	(void) c_scanw(fmtvar,argvar);
	return(OK);
}

c_mvwscanw(winvar,yvar,xvar,fmtvar,argvar)
VAR_ENTRY *winvar, *yvar, *xvar, *fmtvar, *argvar;
{
	int y, x;
	TERMINAL *win;

	if (FirstCall)
	{
		y = yvar->var_data.var_num;
		x = xvar->var_data.var_num;
		win = winvar->var_data.var_term;

		(void) c_wmove(win,y,x);
	}

	(void) c_wscanw(winvar,fmtvar,argvar);
	return(OK);
}

