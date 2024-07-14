/*
** SCCS Reporting
*/

# include <sv_base.h>

/*
** Include files specific for "lib_pitch.c"
*/

# include "main.h"
# include "eval_arg.h"
# include "lib_util.h"
# include "cursehead.h"

extern int	printf();
extern int	scanf();

SCCSID("@(#)lib_pitch.c	3.2 (SVID) 12/24/86");

/*
** SVVS Reporting (cont.)
*/

# include <sv_env.h>
# include <svvs.h>
# include <sv_macro.h>

BOOLEAN		InitTIE = FALSE;
char		*DeferScrDump = NULL;

int	p_v_call();
int	p_wv_call();
int	p_r_call();
int	p_wr_call();
void	get_work_str();
int	fake_scan();

int p_adch(ch)
VAR_ENTRY	*ch;
{
	return(Zaddch(ch->var_data.var_chtype));
}

int p_adstr(str)
VAR_ENTRY	*str;
{
	return(Zaddstr(text_addr(str)));
}

int p_offattr(attrs)
VAR_ENTRY	*attrs;
{
	return(Zattroff(attrs->var_data.var_num));
}

int p_onattr(attrs)
VAR_ENTRY	*attrs;
{
	return(Zattron(attrs->var_data.var_num));
}

/*
** "scr_dump()" is non-trivial when the program wants to do the "scr_dump()"
** outside the "initscr()"/"endwin()" pair.  Because of time stamping of both
** the dump file as well as the output device, it is imperative that the 
** "scr_dump()" is done after ALL output is finished on the output device. 
** This means we have to defer the "scr_dump()" until AFTER we send our 
** last protocal marker in the routine "split_up()"
**
** The variable "DeferScrDump" acts as both a flag to tell us that the
** "scr_dump()" must be defered as well as holding the file name we want
** to dump to.
*/

int p_scrdump(file_name)
char	*file_name;
{
	if (DeferScrDump != NULL)
	{
		Zscr_dump(DeferScrDump);
	}
	else if (InitTIE)
	{
		Zscr_dump(file_name);
	}
	else
	{
		DeferScrDump = 
			(char *) malloc((unsigned) (strlen(file_name) + 1));
		if (DeferScrDump == NULL)
		{
			screech("c_scrdump", 0, "malloc failed\n");
		}
		strcpy(DeferScrDump, file_name);
	}
}

int p_setattr(attrs)
VAR_ENTRY	*attrs;
{
	return(Zattrset(attrs->var_data.var_num));
}

int p_system(sh_cmd)
char	*sh_cmd;
{
	return(system(sh_cmd));
}

int p_barber(ch)
char	ch;
{
	return(wbarber(stdscr, LINES, COLS, ch));
}

int p_clear()
{
	return(Zclear());
}

int p_clrbot()
{
	return(Zclrtobot());
}

int p_clreol()
{
	return(Zclrtoeol());
}

int p_dlch()
{
	return(Zdelch());
}

int p_deleteln()
{
	return(Zdeleteln());
}

int p_charecho(ch)
VAR_ENTRY	*ch;
{
	return(Zechochar(ch->var_data.var_chtype));
}

void p_endwin()
{
	Zendwin();
	InitTIE = FALSE;
}

int p_erase()
{
	return(Zerase());
}

int p_flushinp()
{
	return(Zflushinp());
}

int p_gtch()
{
	return(Zgetch());
}

int p_gtstr(str)
VAR_ENTRY	*str;
{
	int	result;

	result = Zgetstr(str->var_data.var_text_ptr);

	return(result);
}

int p_getyx(win, y, x)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zgetyx(win->var_data.var_window,& 		     y->var_data.var_num,&  		     x->var_data.var_num));
}

int p_getbegyx(win, y, x)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zgetbegyx(win->var_data.var_window,& 			y->var_data.var_num,&  			x->var_data.var_num));
}

int p_getmaxyx(win, y, x)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zgetmaxyx(win->var_data.var_window,& 			y->var_data.var_num,&  			x->var_data.var_num));
}

WINDOW *p_initscr()
{
	WINDOW	*result;

	result = Zinitscr();
	InitTIE = TRUE;
	reload_var();

	return(result);
}

chtype p_inc()
{
	return(Zinch());
}

int p_insch(ch)
VAR_ENTRY	*ch;
{
	return(Zinsch(ch->var_data.var_chtype));
}

int p_insertln()
{
	return(Zinsertln());
}

int p_move(y, x)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmove(y->var_data.var_num, x->var_data.var_num));
}

int p_addchmv(y, x, ch)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*ch;
{
	return(Zmvaddch(y->var_data.var_num, 
		       x->var_data.var_num, 
		       ch->var_data.var_chtype));
}

int p_addstrmv(y, x, str)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*str;
{
	return(Zmvaddstr(y->var_data.var_num, 
			x->var_data.var_num, 
			text_addr(str)));
}

int p_delchmv(y, x)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmvdelch(y->var_data.var_num, x->var_data.var_num));
}

int p_getchmv(y, x)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmvgetch(y->var_data.var_num, x->var_data.var_num));
}

int p_getstrmv(y, x, str)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*str;
{
	int	result;

	result = Zmvgetstr(y->var_data.var_num,
			  x->var_data.var_num,
			  str->var_data.var_text_ptr);

	return(result);
}

chtype p_inchmv(y, x)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmvinch(y->var_data.var_num, x->var_data.var_num));
}

int p_mvinsch(y, x, ch)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*ch;
{
	return(Zmvinsch(y->var_data.var_num,
		       x->var_data.var_num, 
		       ch->var_data.var_chtype));
}

int p_mvprintw(y, x, fmt, arg)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	if (FirstCall)
	{
		(void) Zmove(y->var_data.var_num, x->var_data.var_num);
	}
	return(p_v_call(printw, fmt, arg));
}

int p_mvscanw(y, x, fmt, arg)
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static	char	*work_str = NULL;

	if (FirstCall)
	{
		(void) Zmove(y->var_data.var_num, x->var_data.var_num);
		get_work_str(stdscr, &work_str);
	}
	return(fake_scan(&work_str, fmt, arg));
}

int p_setscrreg(top, bot)
VAR_ENTRY	*top;
VAR_ENTRY	*bot;
{
	return(Zsetscrreg(top->var_data.var_num, bot->var_data.var_num));
}

int p_chaddmvw(win, y, x, ch)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*ch;
{
	return(Zmvwaddch(win->var_data.var_window, 
			y->var_data.var_num, 
			x->var_data.var_num, 
			ch->var_data.var_chtype));
}

int p_straddmvw(win, y, x, str)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*str;
{
	return(Zmvwaddstr(win->var_data.var_window, 
			 y->var_data.var_num, 
			 x->var_data.var_num, 
			 text_addr(str)));
}

int p_chdelmvw(win, y, x)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmvwdelch(win->var_data.var_window,
			y->var_data.var_num, 
			x->var_data.var_num));
}

int p_chgetmvw(win, y, x)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmvwgetch(win->var_data.var_window, 
			y->var_data.var_num,
			x->var_data.var_num));
}

int p_strgetmvw(win, y, x, str)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*str;
{
	int	result;

	result = Zmvwgetstr(win->var_data.var_window,
			   y->var_data.var_num,
			   x->var_data.var_num,
			   str->var_data.var_text_ptr);

	return(result);
}

chtype p_chinmvw(win, y, x)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
{
	return(Zmvwinch(win->var_data.var_window,
		       y->var_data.var_num, 
		       x->var_data.var_num));
}

int p_mvwinsch(win, y, x, ch)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*ch;
{
	return(Zmvwinsch(win->var_data.var_window, 
			y->var_data.var_num,
			x->var_data.var_num,
			ch->var_data.var_chtype));
}

int p_mvwprintw(win, y, x, fmt, arg)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	if (FirstCall)
	{
		(void) Zwmove(win->var_data.var_window,
			     y->var_data.var_num, 
			     x->var_data.var_num);
	}
	return(p_wv_call(wprintw, win, fmt, arg));
}

int p_mvwscanw(win, y, x, fmt, arg)
VAR_ENTRY	*win;
VAR_ENTRY	*y;
VAR_ENTRY	*x;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static	char	*work_str = NULL;

	if (FirstCall)
	{
		(void) Zwmove(win->var_data.var_window,
			     y->var_data.var_num, 
			     x->var_data.var_num);
		get_work_str(win->var_data.var_window, &work_str);
	}
	return(fake_scan(&work_str, fmt, arg));
}

int p_wsetscrreg(win, top, bot)
VAR_ENTRY	*win;
VAR_ENTRY	*top;
VAR_ENTRY	*bot;
{
	return(Zwsetscrreg(win->var_data.var_window, 
			  top->var_data.var_num, 
			  bot->var_data.var_num));
}

int p_refresh()
{
	return(Zrefresh());
}

int p_endstand()
{
	return(Zstandend());
}

int p_standout()
{
	return(Zstandout());
}

char *p_temp_path()
{
	char		*tmp_str;
	char		*result;
	unsigned	tmp_len;

	tmp_str = temp_path();
	tmp_len = strlen(tmp_str) + 1;

	if ((result = malloc(tmp_len)) == NULL)
	{
		screech("c_temp_path", 0, "malloc failed\n");
	}

	(void) strcpy(result, tmp_str);
}

char *p_unctrl(ch)
VAR_ENTRY	*ch;
{
	char	tmp_char;

	tmp_char = ch->var_data.var_chtype;
	return(Zunctrl(tmp_char));
}

int p_ungetch(ch)
VAR_ENTRY	*ch;
{
	return(Zungetch(ch->var_data.var_chtype));
}

int p_wgetstr(win, str)
VAR_ENTRY	*win;
VAR_ENTRY	*str;
{
	int	result;

	result = Zwgetstr(win->var_data.var_window, 
			 str->var_data.var_text_ptr);

	return(result);
}

int p_wchin(win)
VAR_ENTRY	*win;
{
	return(Zwinch(win->var_data.var_window));
}

/*
**
*/

int p_printf(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	return(p_v_call(printf, fmt, arg));
}

int p_printw(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	return(p_v_call(printw, fmt, arg));
}

int p_wprintw(win, fmt, arg)
VAR_ENTRY	*win;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	return(p_wv_call(wprintw, win, fmt, arg));
}

int p_scanf(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	return(p_r_call(scanf, fmt, arg));
}

int p_scanw(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*work_str = NULL;

	if (FirstCall)
	{
		get_work_str(stdscr, &work_str);
	}
	return(fake_scan(&work_str, fmt, arg));
}

int p_wscanw(win, fmt, arg)
VAR_ENTRY	*win;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*work_str = NULL;

	if (FirstCall)
	{
		get_work_str(win->var_data.var_window, &work_str);
	}
	return(fake_scan(&work_str, fmt, arg));
}
