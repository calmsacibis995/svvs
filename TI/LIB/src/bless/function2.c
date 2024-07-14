# include	<bless.h>
# include	<common.h>
# include	<macros.h>

SCCSID("@(#)function2.c	3.2 (SVID) 12/30/86");

/*
** mk_func
**	Turn on blank mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
mk_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_INVIS);
}
/*
** so_func
**	Begin standout mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
so_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_SO);
}
/*
** us_func
**	Turn on underline mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
us_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_UL);
}
/*
** SX_func
**	Turn on xon/xoff handshaking.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
SX_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("SX_func", term);
}
/*
** ec_func
**	erase X number of characters.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number of characters to erase.
*/
/*ARGSUSED*/
ec_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	char	*str;
	int	count;

	str = getpos(term);
	count = arg->arg_int;
	if ( (count > abs((getx(term) + 1) - getwidth(term)) ) )
		count = abs( (getx(term) + 1) - getwidth(term));
	while ( count-- )
		*str++ = ' ';
}
/*
** ae_func
**	End alternate character set.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ae_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_ALTCH);
}
/*
** RA_func
**	Turn off automatic margins.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
RA_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	Bool_am = 0; 
}
/*
** me_func
**	Turn off all attributes.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
me_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_UL|TERM_SO|TERM_HALF|TERM_BLINK|TERM_BOLD|TERM_REV);
}
/*
** te_func
**	Reverse initialization for programs that use cursor motion.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
te_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_CMINIT);
}
/*
** ed_func
**	End delete mode (exit mode for dch1).
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ed_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
}
/*
** ei_func
**	End insert mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ei_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_INSERT);
}
/*
** se_func
**	End standout mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
se_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_SO);
}
/*
** ue_func
**	End underline mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ue_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_UL);
}
/*
** RX_func
**	Turn off XON/XOFF.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
RX_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("RX_func", term);
}
/*
** vb_func
**	Turn on visible bell.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
vb_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_VB);
}
/*
** ff_func
**	Form feed (hard copy terminal only).
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ff_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("ff_func", term);
}
/*
** fs_func
**	Return from status line.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
fs_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if (Bool_hs) 
	{
		if (term->term_onstatus) 
		{
			term->term_xloc = term->term_savx;
			term->term_yloc = term->term_savy;
			term->term_onstatus = 0;
		}
		else 
			errormsg("Return from status line, when not on it.", term);
	} 
	else 
		errormsg("Return from status line on terminal that has none.", term);
		
}
/*
** i1_func
**	Terminal initialization string.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
i1_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_INIT1);
}
/*
** is_func
**	Terminal initialization string.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
is_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_INIT2);
}
/*
** i3_func
**	Terminal initialization string.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
i3_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_INIT3);
}
/*
** al_func
**	Add a new blank line.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
al_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	char	*this_line, *next_line;
	char	spare[MAXX];
	char	spare2[MAXX];
	long	*this_bits, *next_bits;
	long	spare_bits[MAXX];
	long	spare2_bits[MAXX];
	int	y;

	/*
	** Figure out where the blank line goes.
	*/
	y = gety(term) + 1;

	this_line = getline(term);
	this_bits = term->term_bits[gety(term)];
	(void) memcpy(spare,this_line,getwidth(term));
	(void) memcpy(spare_bits,this_bits,sizeof (long) * getwidth(term));
	spare[getwidth(term)] = '\0';
	this_line = spare;
	this_bits = spare_bits;

	/*
	** Move the other lines down one.
	*/
	while ( y < getheight(term) )
	{
		next_line = getstring(term,y);
		next_bits = term->term_bits[y];
		if ( this_line == spare )
		{
			(void) memcpy(spare2,next_line,getwidth(term));
			(void) memcpy(spare2_bits,next_bits,sizeof (long) * getwidth(term));
		}
		else
		{
			(void) memcpy(spare,next_line,getwidth(term));
			(void) memcpy(spare_bits,next_bits,sizeof (long) * getwidth(term));
		}
		(void) memcpy(next_line, this_line, getwidth(term));
		(void) memcpy(next_bits, this_bits, sizeof (long) * getwidth(term));
		if ( this_line == spare )
		{
			this_line = spare2;
			this_bits = spare2_bits;
		}
		else
		{
			this_line = spare;
			this_bits = spare_bits;
		}
		y++;
	}
	this_line = getline(term);
	(void) memset(this_line,' ',getwidth(term));
	this_bits = term->term_bits[gety(term)];
	(void) memset(this_bits,'\0',sizeof (long) * getwidth(term));
}
/*
** ks_func
**	Put terminal into keypad transmit mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ks_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_set(term,TERM_KBT);
}
/*
** ke_func
**	Out of keypad transmit mode.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ke_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term_clr(term,TERM_KBT);
}
/*
** LF_func
**	Turn off soft labels.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
LF_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	Soft_label_keys_on = 0;
}
/*
** LO_func
**	Turn on soft labels.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
LO_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	Soft_label_keys_on = 1;
}
/*
** DC_func
**	Delete some number of characters.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number of character to delete.
*/
DC_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;
	while ( count-- )
		dc_func(term,(ARGS *) NULL);
}
/*
** DL_func
**	Delete some number of lines.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number of lines to delete.
*/
DL_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;
	while ( count-- )
		dl_func(term,(ARGS *) NULL);
}
/*
** DO_func
**	Move cursor down X lines.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number of lines to move.
*/
DO_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;
	while ( count-- )
		down_func(term,(ARGS *) NULL);
}
/*
** IC_func
**	Insert X blank characters.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number of characters to insert.
*/
IC_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;
	ARGS	blank;

	blank.arg_char = ' ';
	blank.arg_type = ARG_CHAR;
	count = arg->arg_int;

	while ( count-- )
		insert_char(term,&blank);
}
/*
** AL_func
**	Add X new blank lines
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number of blank lines to add.
*/
AL_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;

	while ( count-- )
		al_func(term,(ARGS *) NULL);
}
/*
** LE_func
**	Move cursor left X places.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number to move the cursor.
*/
LE_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;

	while ( count-- )
		left_func(term,(ARGS *) NULL);
}
/*
** RI_func
**	Move cursor right X places.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number to move the cursor.
*/
RI_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;

	while ( count-- )
		right_func(term,(ARGS *) NULL);
}
/*
** UP_func
**	Move cursor up X places.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Number to move the silly thing.
*/
UP_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	count;

	count = arg->arg_int;

	while ( count-- )
		up_func(term,(ARGS *) NULL);
}
/*
** r1_func
**	Reset terminal sequence.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
r1_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("r1_func", term);
}
/*
** r2_func
**	Reset terminal sequence.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
r2_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("r2_func", term);
}
/*
** r3_func
**	Reset terminal sequence.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
r3_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("r3_func", term);
}
/*
** rc_func
**	Restore cursor from last sc (save cursur).
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
rc_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if ( term->term_savx >= 0 )
	{
		setx(term) = term->term_savx;
		gety(term) = term->term_savy;
	}
}
/*
** cv_func
**	Move to an absolute vertical position.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Where to move to.
*/
/*ARGSUSED*/
cv_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	where;

	where = arg->arg_int;

	gety(term) = where;
}
/*
** sc_func
**	Save cursor position.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
sc_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term->term_savx = getx(term);
	term->term_savy = gety(term);
}
/*
** sa_func
**	Define video attributes.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
sa_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	if (arg->arg_int == 0)
		term_set(term, TERM_SO);
	else
		term_clr(term, TERM_SO);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_UL);
	else
		term_clr(term, TERM_UL);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_REV);
	else
		term_clr(term, TERM_REV);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_BLINK);
	else
		term_clr(term, TERM_BLINK);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_HALF);
	else
		term_clr(term, TERM_HALF);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_BOLD);
	else
		term_clr(term, TERM_BOLD);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_INVIS);
	else
		term_clr(term, TERM_INVIS);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_PROT);
	else
		term_clr(term, TERM_PROT);
	arg = arg->arg_next;
	if (arg->arg_int == 0)
		term_set(term, TERM_ALTCH);
	else
		term_clr(term, TERM_ALTCH);
	arg = arg->arg_next;
}
/*
** ML_func
**	Set soft left margin.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
ML_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	getlmargin(term) = arg->arg_int;
}
/*
** MR_func
**	Set soft right margin.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
MR_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	getrmargin(term) = arg->arg_int;
}
/*
** st_func
**	Set a tab.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
st_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	term->term_tabs[arg->arg_int] = 1;
}
/*
** ta_func
**	Go to the next tab stop.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ta_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	x;

	x = getx(term);

	x = ((x / TABLEN) * TABLEN) + TABLEN;
	setx(term) = x;
}
/*
** ts_func
**	Go to the status line.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
ts_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{

	if (Bool_hs) 
	{
		term->term_savx = term->term_xloc; 
		term->term_savy = term->term_yloc;
		term->term_yloc = getheight(term);
		term->term_xloc = 0;
		term->term_onstatus = 1;
		/*
		term->term_statusx = 0;
		*/
	} 
	else 
		errormsg("Attempt to move to status line on terminal that has none", term);

}
/*
** uc_func
**	Underscore one character, and move past it.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
uc_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int locy, locx;

	locy = gety(term);
	locx = getx(term);

	if (term->term_onstatus)
	{
		getbits(term) = t_bit_on(term, locy, locx, TERM_UL);
		setx(term) += 1;
		/*
		term->term_statusx++;
		*/
		if ( getx(term) >= getwidth(term) )
			setx(term) = getwidth(term) - 1;
		return;
	}

	getbits(term) = t_bit_on(term, locy, locx, TERM_UL);
	setx(term) += 1;
	if ( getx(term) >= getwidth(term) )
	{
		if ( Bool_am )
		{
			setx(term) = 0;
			gety(term) += 1;
			if ( gety(term) >= getheight(term) )
			{
				scroll_term(term);
				gety(term) = getheight(term) - 1;
			}
		}
		else
			errormsg("Moving past end of line, without automatic margins.", term);
	}
}
/*
** hu_func
**	Half line up.
**
** Arguments
**	term	Current TERMINAL screen.
**	arg	Argument list.
*/
/*ARGSUSED*/
hu_func(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	errormsg("hu_func", term);
}
