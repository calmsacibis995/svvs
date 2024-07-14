# include "main.h"

SCCSID("@(#)c_getstr.c	3.1 (SVID) 12/1/86");

c_gtstr(str_var)	/* catcher's version of getstr */
VAR_ENTRY *str_var;
{
	char saved_char, *str, *tmp_str;
	
	str = str_var->var_data.var_text_ptr;

	tmp_str = str;
	/*
	** We read in characters from input until we encounter a newline, and
	** then we terminate the string with a NULL character.
	*/

	while ((saved_char = c_gtch()) != '\n')
	{
		*tmp_str = saved_char;
		tmp_str++;
	}

	*tmp_str = '\0';

# ifdef TRACE
	if (Dbg)
	{
		printf("gtstr got '%s' \n",str);
	}
# endif	/* TRACE */

	return(OK);

}

c_wgetstr(winvar,strvar)
VAR_ENTRY *winvar, *strvar;
{
	char saved_char, *tmp_str;
	TERMINAL *win;
	char *str;

	win = winvar->var_data.var_term;
	str = strvar->var_data.var_text_ptr;

	tmp_str = str;

	/*
	** We read in characters from input until we encounter a newline, and
	** then we terminate the string with a NULL character.
	*/

	while ((saved_char = c_wchget(win)) != '\n')	
	{
		*tmp_str = saved_char;
		tmp_str++;
	}

	*tmp_str = '\0';

# ifdef TRACE
	if (Dbg)
	{	
		printf("str = '%s' \n",str);
	}
# endif	/* TRACE */

	return(OK);
}

c_getstrmv(yvar,xvar,strvar)	/* catcher's version of mvgetstr */
VAR_ENTRY *yvar,*xvar,*strvar;
{
		(void) c_move(yvar,xvar);

	(void) c_gtstr(strvar);

	return(OK);
}

c_strgetmvw(winvar,yvar,xvar,strvar)	/* catcher's version of mvwgetstr */
VAR_ENTRY *winvar, *yvar, *xvar, *strvar;
{ 
	TERMINAL *win;
	int y,x;

		win = winvar->var_data.var_term;
		y = yvar->var_data.var_num;
		x = xvar->var_data.var_num;

	  	(void) c_wmove(win,y,x);

	(void) c_wgetstr(winvar, strvar);

	return(OK);
}

