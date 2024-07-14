# include	<bless.h>
# include	<common.h>
# include	<macros.h>

SCCSID("@(#)function1.c	3.2 (SVID) 12/30/86");

/*
** insert_char
**	Insert a character.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Character to insert.
*/
int
/*ARGSUSED*/
insert_char(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	char	buf[MAXX + 1];
	char	*start;
	char	*sp;
	int	len;

	start = getline(term);
	sp = getpos(term);
	len = getwidth(term) - abs(start - sp);
	len--;
	(void) strncpy(buf,sp,len);
	*sp = arg->arg_char;
	(void) strncpy((sp + 1),buf,len);
}
/*
** move_func
**	Direct cursor motion.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	X and Y coordinates.
*/
int
/*ARGSUSED*/
move_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term->term_yloc = arg->arg_int;
	arg = arg->arg_next;
	term->term_xloc = arg->arg_int;
}
/*
** cl_func
**	Clear the screen.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
cl_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clear(term);
}
/*
** home_func
**	Home the cursor.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
home_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a home sequence has been received",term);
	}

	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a home cursor sequence has been received",term);
	}

	term->term_xloc = 0;
	term->term_yloc = 0;
}
/*
** up_func
**	Move the cursor up one line.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
up_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a cursor up  sequence has been received",term);
	}
	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a up cursor sequence has been received",term);
	}
	if ( term->term_yloc )
		term->term_yloc--;
}
/*
** down_func
**	Move the cursor down one line.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
down_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a up cursor  sequence has been received",term);
	}

	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a down cursor sequence has been received",term);
	}

	gety(term) += 1;

	/*
	setx(term) = 0;
	*/

	if ( gety(term) >= getheight(term) )
	{
		scroll_term(term);
		gety(term) = getheight(term) - 1;
	}
}
/*
** left_func
**	Move the cursor left one line.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
left_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a left cursor sequence has been received",term);
	}
	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a left cursor sequence has been received",term);
	}
	if ( term->term_xloc )
		term->term_xloc--;
}
/*
** right_func
**	Move the cursor right one column.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
right_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a right cursor sequence has been received",term);
	}
	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a right cursor sequence has been received",term);
	}
	if ( term->term_xloc < term->term_width )
		term->term_xloc++;
}
/*
** cb_func
**	Clear to the beginning of the line (inclusive).
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
cb_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	char	*str;
	int	x,i;

	str = getline(term);
	x = getx(term);
	for ( i = 0 ; i <= x ; i++ )
		str[i] = ' ';
}
/*
** ce_func
**	Clear to the end of the line.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
ce_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	char	*str;
	long	*bits;
	int	len;

	str = term->term_screen[term->term_yloc];
	str = &str[term->term_xloc];
	len = (term->term_width - term->term_xloc);
	bits = &term->term_bits[gety(term)][getx(term)];
	(void) memset(str,' ',len);
	(void) memset(bits,'\0',sizeof (long) * len);
}
/*
** cd_func
**	Clear to the end of display.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
cd_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	char	*str;
	long	*bits;
	int	len;
	int	y;

	y = term->term_yloc;
	str = term->term_screen[y];
	str = &str[term->term_xloc];
	bits = &term->term_bits[gety(term)][getx(term)];
	len = (term->term_width - term->term_xloc);
	(void) memset(str,' ',len);
	(void) memset(bits,'\0',sizeof (long) * len);
	y++;
	for ( ; y < term->term_height ; y++ )
	{
		str = term->term_screen[y];
		bits = term->term_bits[y];
		(void) memset(str,' ',term->term_width);
		(void) memset(bits,'\0',sizeof (long) * getwidth(term));
	}
}
/*
** bell_func
**	Ring an audible bell.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
bell_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term->term_flag |= TERM_BELL;
}
/*
** nw_func
**	Print a carriage return and new line.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
nw_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term->term_yloc++;
	term->term_xloc = 0;
	if ( gety(term) >= getheight(term) )
	{
		scroll_term(term);
		gety(term) = getheight(term) - 1;
	}
}
/*
** cr_func
**	Print a carriage return.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
cr_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term->term_xloc = 0;
}
/*
** ch_func
**	Move the cursor to an absolute horizontal location.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	Just a place holder.
*/
int
/*ARGSUSED*/
ch_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a ch cursor motion sequence has been received",term);
	}
	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a ch cursor sequence has been received",term);
	}
	term->term_yloc = arg->arg_int;
}

/*
** bt_func
**	Back tab function.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
bt_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	i;

	/*
	** If there are no tabs of any kind set, ignore
	** backtabs, just like we ignore tabs.
	*/
	i = getwidth(term) - 1;
	while ( i )
	{
		if ( term->term_tabs[i] == 1 )
			break;
		i--;
	}
	if ( term->term_tabs[i] == 0 )
		return;
	/*
	** backtabs are simpler than regular tabs, since they do not cause 
	** scrolling, nor do they wrap back to the next line.
	*/
	if (term->term_onstatus)
		return;		/*punt!*/

	i = getx(term);
	if (Bool_xt) 
	{
		do
		{
			getchr(term) = ' ';
			i--;
			setx(term) = i;
		} while (i > 0 && term->term_tabs[i] == 0);
		getchr(term) = ' ';
	} 
	else 
	{
		i--;
		while (i > 0 && term->term_tabs[i] == 0)
			i--;
		setx(term) = i;
	}
			
}
/*
** cs_func
**	Change Scroll region.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
cs_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	top, bot;

	bot = arg->arg_int;
	arg = arg->arg_next;
	top = arg->arg_int;
	if ( bot > getheight(term) || top > getheight(term) )
		errormsg("cs_func: setting scrolling region to not be screen\n");
}
/*
** rP_func
**	Pad character.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
rP_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("rP_func: This routine should not be called",term);
}
/*
** ct_func
**	Clear all tab stops.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ct_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int i;

	for ( i = 0; i == MAXX; i++)
		term->term_tabs[i] = 0;
}
/*
** MC_func
**	Clear left and right soft margins.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
MC_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	getlmargin(term) = 0;
	getrmargin(term) = 0;
}
/*
** vi_func
**	Turn the cursor invisible.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
vi_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_CURINV);
}
/*
** CM_func
**	Memory relative cursor addressing.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
CM_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a memory relative cursor (CM) sequence has been received",term);
	}
	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a memory relative cursor (CM) sequence has been received",term);
	}
	term->term_yloc = arg->arg_int;
	arg = arg->arg_next;
	term->term_xloc = arg->arg_int;
}
/*
** ve_func
**	Makes the cursor visible.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ve_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_CURSO|TERM_CURINV);
}
/*
** ll_func
**	Go to the last line, first column.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ll_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term_on(term,TERM_SO|TERM_UL|TERM_REV|TERM_HALF|TERM_BOLD) &&
		(Bool_ms == 0) )
	{
		errormsg("The terminal is in standout mode, the terminal is configured with ms off, and a ll command sequence has been received",term);
	}
	if ( term_on(term,TERM_INSERT) && (Bool_mi == 0) )
	{
		errormsg("The terminal is in insert mode, the terminal is configured with mi off, and a ll command sequence has been received",term);
	}
	setx(term) = 0;
	gety(term) = getheight(term) - 1;
}
/*
** vs_func
**	Cursur standout.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
vs_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_CURSO);
}
/*
** dc_func
**	Delete character.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
dc_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	x;
	char	*str;
	long	*bits;
	char	buf[MAXX + 1];
	long	bits_buf[MAXX + 1];

	str = getline(term);
	bits = term->term_bits[gety(term)];
	x = getx(term);
	(void) memcpy(buf, &str[x + 1], abs(getwidth(term) - x));
	(void) memcpy(&str[x],buf,abs(getwidth(term) - x));
	(void) memcpy(bits_buf, &bits[x + 1], sizeof (long) * abs(getwidth(term) - x));
	(void) memcpy(&bits[x],bits_buf,sizeof (long) * abs(getwidth(term) - x));
	str[getwidth(term) - 1] = ' ';
	bits[getwidth(term) - 1] = 0;

}
/*
** dl_func
**	Delete a line.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
dl_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	long	*this_bits,
		*next_bits;
	char	*this_line,
		*next_line;
	int	y;

	this_line = getline(term);
	this_bits = term->term_bits[term->term_yloc];
	y = gety(term) + 1;

	while ( y < getheight(term) )
	{
		next_line = getstring(term,y);
		next_bits = term->term_bits[y];
		(void) memcpy(this_line, next_line, getwidth(term));
		(void) memcpy(this_bits, next_bits, sizeof (long) * getwidth(term));
		this_line = next_line;
		this_bits = next_bits;
		y++;
	}
	(void) memset(this_line,' ',getwidth(term));
	(void) memset(this_bits,'\0',sizeof (long) * getwidth(term));
}
/*
** ds_func
**	Disable status line.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ds_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	/*
	** sets status line boolean to no, and returns from status line
	** if it is currently on it
	** Note: there is no function defined to re-enable the status
	** line once it has been turned off
	*/
	Bool_hs = 0;
	if ( term->term_onstatus )
	{
		setx(term) = term->term_savx;
		gety(term) = term->term_savy;
		/*
		term->term_statusx = 0;
		*/
		term->term_onstatus = 0;
	}
}
/*
** hd_func
**	Forward a half line.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
hd_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("hd_func", term);
}
/*
** eA_func
**	Enable alternate character set.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
eA_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_ALTCH);
}
/*
** as_func
**	Start alternate charcter set.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
as_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_STALTCH);
}
/*
** SA_func
**	Enable automatic margins.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
SA_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	Bool_am = 1;
}
/*
** mb_func
**	Turn on blinking.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
mb_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_BLINK);
}
/*
** md_func
**	Turn on bold.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
md_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_BOLD);
}
/*
** ti_func
**	Initilization string for cursor motion.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ti_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_CMINIT);
}
/*
** dm_func
**	Enter delete mode (this is just a setup for dch1).
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
dm_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
}
/*
** mh_func
**	Turn on half bright mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
mh_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_HALF);
}
/*
** im_func
**	Enter insert mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
im_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_INSERT);
}
/*
** mp_func
**	Turn on protected mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
mp_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("mp_func", term);
}
/*
** mr_func
**	Turn on reverse video mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
mr_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_REV);
}

