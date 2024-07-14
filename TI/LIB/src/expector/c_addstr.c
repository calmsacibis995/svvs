# include "main.h"
# include <curses.h>

SCCSID("@(#)c_addstr.c	3.1 (SVID) 12/1/86");
extern int Dbg;

c_adstr(str)	/* catcher's version of addstr */
VAR_ENTRY *str;
{
	extern int c_adch();
	char	*tmp_str;
	VAR_ENTRY argvar;

	tmp_str = text_addr(str);

/*
** c_adstr simply performs c_adch on successive characters of the given
** string until its end.
*/

	while (*tmp_str != NULL)
	{
		argvar.var_data.var_chtype = *tmp_str;

# ifdef TRACE
		if (Dbg)
		{
			printf("arg's address is %x\n",tmp_str);
			printf("about to call adch from adstr -- arg = '%c'\n",*tmp_str);
		}	
# endif	/* TRACE */
		
		(void) c_adch(&argvar);

		tmp_str++;
	}

	return(OK);

}

c_wstradd(win,str)	/* catcher's version of waddstr */
char *str;
TERMINAL *win;
{
	char *tmp_str;
	ARGS arg;
	VAR_ENTRY argvar;

	argvar.var_data.var_text_ptr = str; 
	tmp_str = text_addr(&argvar);


/*
** c_wstradd simply performs c_wchadd on successive characters of the given
** string until its end.
*/

	while (*tmp_str != NULL)
	{
		arg.arg_char = *tmp_str;
		
# ifdef TRACE
		if (Dbg)
		{
			printf("arg's address is %x\n",tmp_str);
			printf("about to call wchadd from wstradd -- arg = '%c'\n",*tmp_str);
		}	
# endif	/* TRACE */
		(void) c_wchadd(win,(chtype) (arg.arg_char));	

		tmp_str++;
	}

	return(OK);

}

/* 
** c_addstrmv allows us to choose where on the standard screen we
** would like our string to be placed.
*/

c_addstrmv(vary,varx,strvar)	/* catcher's version of mvaddstr */
VAR_ENTRY *vary, *varx, *strvar;
{
	(void) c_move(vary,varx);
	(void) c_adstr(strvar);

	return(OK);
}

/* 
** c_straddmvw allows us to choose where on the given window we
** would like our string to be placed.
*/

c_straddmvw(winvar,vary,varx,strvar)	/* catcher's version of mvwaddstr */
VAR_ENTRY *winvar, *vary, *varx, *strvar;
{
	TERMINAL *win;
	int y,x;
	char *str;

	win = winvar->var_data.var_term;
	y = vary->var_data.var_num;
	x = varx->var_data.var_num;
	str = strvar->var_data.var_text_ptr;

	(void) c_wmove(win,y,x);
	(void) c_wstradd(win,str);

	return(OK);
}


