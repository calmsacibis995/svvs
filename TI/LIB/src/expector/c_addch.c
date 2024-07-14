# include "main.h"
# include <curses.h>
# include <ctype.h>
# include <memory.h>

SCCSID("@(#)c_addch.c	3.1 (SVID) 12/1/86");

/*
** Some psuedo-functions to ease typing.
*/
# define	getline(trm)	trm->term_screen[trm->term_yloc]
# define	getstring(trm,y) trm->term_screen[y]
# define	getpos(trm)	&trm->term_screen[trm->term_yloc][trm->term_xloc]
# define	getchr(trm)	trm->term_screen[trm->term_yloc][trm->term_xloc]
# define	getbits(trm)	trm->term_bits[trm->term_yloc][trm->term_xloc]
# define	getheight(trm)	trm->term_height
# define	getwidth(trm)	trm->term_width
# define	getx(trm)	trm->term_xloc
# define	gety(trm)	trm->term_yloc

/*
** "set_char_attrs" sets the term_bits field of location (y,x) in window "win"
** to the attributes in "atrs", without erasing any other attributes.
** If, however, the capability for a given attribute is not present on the
** given terminal type, we use the default attribute, which was set in c_initscr
*/

set_char_attrs(win,y,x,attrs)
TERMINAL *win;
int y,x,attrs;
{
	extern char *tigetstr();

	if (attrs & A_STANDOUT)
	{
		if (tigetstr("smso") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has STANDOUT set\n",y,x);
			}
# endif	/* TRACE */		
			t_bit_set(win,y,x,Default_attr);
		}
	}

	if (attrs & A_REVERSE)
	{
		if (tigetstr("rev") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has REVERSE set\n",y,x);
			}
# endif	/* TRACE */
			t_bit_set(win,y,x,TERM_REV);
		}
		else
		{
			t_bit_set(win,y,x,Default_attr);
		}
	}

	if (attrs & A_BOLD)
	{
		if (tigetstr("bold") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has BOLD set\n",y,x);
			}
# endif	/* TRACE */
			t_bit_set(win,y,x,TERM_BOLD);
		}
		else
		{
			t_bit_set(win,y,x,Default_attr);
		}
	}

	if (attrs & A_DIM)
	{
		if (tigetstr("dim") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has DIM set\n",y,x);
			}
# endif	/* TRACE */
			t_bit_set(win,y,x,TERM_HALF);
		}
		else
		{
			t_bit_set(win,y,x,Default_attr);
		}
	}

	if (attrs & A_BLINK)
	{
		if (tigetstr("blink") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has BLINK set\n",y,x);
			}
# endif	/* TRACE */
			t_bit_set(win,y,x,TERM_BLINK);
		}
		else
		{
			t_bit_set(win,y,x,Default_attr);
		}
	}
	
	if (attrs & A_UNDERLINE)
	{
		if (tigetstr("smul") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has UNDERLINE set\n",y,x);
			}
# endif	/* TRACE */
			t_bit_set(win,y,x,TERM_UL);
		}
		else
		{
			t_bit_set(win,y,x,Default_attr);
		}
	}

	if (attrs & A_ALTCHARSET)
	{
		if (tigetstr("smacs") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("win(%d,%d) has ALTCHARSET set\n",y,x);
			}
# endif /* TRACE */
			t_bit_set(win,y,x,TERM_ALTCH);
		}
		else
		{
			t_bit_set(win,y,x,Default_attr);
		}
	}

}

/*
** "scroll_win" scrolls the window "win" (which is a subwindow of "term")
** up one line. If "win" is not a subwindow, then win=term.
*/

scroll_win(term,win)
TERMINAL	*term, *win;
{
	char	*from;		/* copy strings from here */
	char	*to;		/* to here */
	long	*from_bits;	/* copy attributes from here */
	long	*to_bits;	/* to here */
	int	height;		/* height of the screen. */
	int	width;		/* width of the screen */
	int	next;		/* index to the next string */
	extern	char	*memcpy();
	extern	char	*memset();

	width = getwidth(win);
	to = &(	term->term_screen[(win->term_winy - term->term_winy) + win->scrl_top][win->term_winx - term->term_winx]);
	to_bits = &(term->term_bits[(win->term_winy - term->term_winy) + win->scrl_top][win->term_winx - term->term_winx]);
	from = &(term->term_screen[(win->term_winy - term->term_winy) + win->scrl_top + 1][win->term_winx - term->term_winx]);
	from_bits = &(term->term_bits[(win->term_winy - term->term_winy) + win->scrl_top + 1][win->term_winx - term->term_winx]);
	next = (win->term_winy - term->term_winy) + win->scrl_top + 2;
	height = win->scrl_bot - win->scrl_top;

# ifdef TRACE
if (Dbg)
{
	printf("top of scroll region = %d \n",(win->term_winy - term->term_winy) + win->scrl_top);
	printf("bottom of scroll region = %d \n", (win->term_winy - term->term_winy) + win->scrl_bot);
}
# endif	/* TRACE */

	while ( height-- )
	{
		(void) memcpy(to,from,width);
		(void) memcpy( (char *) to_bits,(char *) from_bits,(int) sizeof (long) * width);
		to = from;
		from = &(term->term_screen[next][win->term_winx - term->term_winx]);
		to_bits = from_bits;
		from_bits = &(term->term_bits[next][win->term_winx - term->term_winx]);
		next++;
	}
	(void) memset(to,' ',width);
	(void) memset( (char *) to_bits,'\0',(int) sizeof (long) * width);
}

c_adch(ch)	/* catcher's version of addch */
VAR_ENTRY *ch;
{
	(void) c_wchadd(std,ch->var_data.var_chtype);

	return(OK);
}

/* c_wchadd puts the character cht at the current location of the cursor for
** the window "win".
*/

c_wchadd(win,cht)	/* catcher's version of waddch */
TERMINAL *win;
chtype cht;
{
	TERMINAL *target;
	char ch;
	char	*str;
	int	saved_y, saved_x, sav_top, sav_bot, i, j, len, switch1, switch2, switch3, switch4, switch5, switch6;
	char	buf[MAXX + 1];
	long	buf_bits[MAXX + 1];
	VAR_ENTRY chvar;
	extern int c_wstradd();
	extern char *c_unctrl();


	ch = cht & A_CHARTEXT;

/*
** If the character to be added is a control character, we add its printable
** version (^X format) to the given window.
*/

	if (!((isascii(ch) && isprint(ch)) || (ch == '\t') || (ch == '\n') || (ch == '\0')))
	{
		chvar.var_data.var_chtype = cht;
		(void) c_wstradd(win,c_unctrl(&chvar));
		return(OK);
	}

/*
** If the given window is a subwindow, we need to get to the parent window,
** since that's where the text is stored.
*/

	target = win;
	while (target->container != NULL)
	{
		target = target->container;
	}

	if (target != win)
	{
		saved_y = target->term_yloc;
		target->term_yloc = (win->term_yloc + win->term_winy) - target->term_winy;
		saved_x = target->term_xloc;		
		target->term_xloc = (win->term_xloc + win->term_winx) - target->term_winx;
	}

/* 
** If the character  to be added is the newline character, then blank out
** the rest of the line and set the cursor to the next row, scrolling if
** it is requested.
*/

	if ( ch == '\n' )
	{
# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"adding newline at location (%d,%d)\n",gety(target),getx(target));
		}
# endif	/* TRACE */
		str = getpos(target);
		for ( len = getx(target) ; len < (win->term_winx - target->term_winx) + win->term_width ; len++ )
			*str++ = ' ';
		
		getx(target) = win->term_winx - target->term_winx;
		gety(target) += 1;
		if ( gety(target) >=  (win->term_winy - target->term_winy) + win->term_height)
		{
			if (term_on(win,WIN_SCRLOK)) 
			{
# ifdef TRACE
				if (Dbg)
				{
					(void) fprintf(stderr,"scrolling\n");
				}
# endif	/* TRACE */
				scroll_win(target,win);
			}
			gety(target) -= 1;
		}
		return(OK);
	}

/*
** If we're in insert mode, we move the rest of the line over by one,
** "bumping off" the last character on the line.
*/

	if ( term_on(target,TERM_INSERT) )
	{
		str = getline(target);
		(void)memcpy(buf,&str[getx(target)],abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));

		(void)memcpy( (char *) buf_bits,(char *) &target->term_bits[gety(target)][getx(target)], (int) sizeof(long) * abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));
			
		getchr(target) = ch;
			
		getbits(target) = term_on(win,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL);

		set_char_attrs(target,gety(target),getx(target),(int) cht & A_ATTRIBUTES);

		(void)memcpy(&str[getx(target) + 1],buf,abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));
		
		(void)memcpy( (char *) &target->term_bits[gety(target)][getx(target) + 1],(char *) buf_bits, (int) sizeof(long) * abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));
		
	}
	else
	{

	/* 
	** For tabs, we go over the prescribed number of spaces
	*/

		if ( ch == '\t' )
		{
			i = (((getx(win) / TABLEN) + 1) * TABLEN) - getx(win);
			for ( j = 0; j < i; j++ )
			{
				ch = ' ';
				(void) c_wchadd(win,(chtype) ch);
			}
			
			return(OK);
		}
		else
		{
		
# ifdef TRACE
			if (Dbg)
			{
				printf("sending %c to location (%d, %d) \n", ch,target->term_yloc,target->term_xloc);
			}
# endif	/* TRACE */
		/* 
		** In most cases, however, the character at the current cursor
		** location is simply overwritten by the new character.
		*/

			getchr(target) = ch;
			
/*
** For attributes, we set the current cursor position to the attributes
** set for the window as a whole, as well as the attributes passed to us
** along with the character.
*/

			getbits(target) = term_on(win,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL);


			set_char_attrs(target,gety(target),getx(target),(int) cht & A_ATTRIBUTES);
		}
	}
 
/*
** If we're at the end of the last line in the window (or the last line in
** the scrolling region) and SCRL_OK is on for that window, we scroll up
** a line, losing the top line.
*/

	getx(target) += 1;
	if ( getx(target) == ((win->term_winx - target->term_winx) + win->term_width))
	{
		getx(target) = (win->term_winx - target->term_winx);
		gety(target) += 1;

		switch1 = gety(target) >= target->term_height;
		switch2 = gety(target) > (win->term_winy - target->term_winy) + win->scrl_bot;
		switch3 = gety(target) > target->scrl_bot;
		switch4 = term_on(target,WIN_SCRLOK);
		switch5 = term_on(win,WIN_SCRLOK);
 		switch6 = gety(target) >= (win->term_winy - target->term_winy) + win->term_height;

		if (((switch1 || switch3) && switch4) || ((switch2 || switch6) && switch5))
		{
			if (switch1 && switch3)	
			{ sav_top = target->scrl_top;
				sav_bot = target->scrl_bot;

				target->scrl_top = 0;
				target->scrl_bot = target->term_height - 1;

				scroll_win(target,target);
		
				target->scrl_top = sav_top;
				target->scrl_bot = sav_bot;	
			}
			else if (switch2 && switch6)
			{
				sav_top = win->scrl_top;
				sav_bot = win->scrl_bot;

				win->scrl_top = 0;
				win->scrl_bot = win->term_height - 1;

				scroll_win(target,win);
				
				win->scrl_top = sav_top;
				win->scrl_bot = sav_bot;
			}
			else if ((switch2 || switch6) && switch5)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("scrolling subwindow \n");
				}
# endif	/* TRACE */
		scroll_win(target,win);

			}
			else
			{
				scroll_win(target,target);
			}
			gety(target) -=1;
		}
		else if (switch1 || switch6)
		{
			getx(target) = (win->term_winx - target->term_winx) + win->term_width - 1;
			gety(target) -=1;
		}
	}

	if (target != win)
	{
		win->term_yloc = target->term_yloc + target->term_winy - win->term_winy;
		win->term_xloc = target->term_xloc + target->term_winx - win->term_winx;

		target->term_yloc = saved_y;
		target->term_xloc = saved_x;;
	}

	return(OK);

}

/* 
** c_addchmv allows us to choose the spot on the standard screen where
** our character is to be added.
*/

c_addchmv(vary,varx,chvar)	/* catcher's version of mvaddch */
VAR_ENTRY *vary, *varx, *chvar;
{
	(void) c_move(vary,varx);
	(void) c_adch(chvar);
	
	return(OK);
}

/* 
** c_chaddmvw allows us to choose the spot on the window  where
** our character is to be added.
*/

c_chaddmvw(winvar,vary,varx,chvar)	/* catcher's version of mvwaddch */
VAR_ENTRY *winvar, *vary, *varx, *chvar;
{
	TERMINAL *win;
	int y,x;
	chtype ch;

	win = winvar->var_data.var_term;
	y = vary->var_data.var_num;
	x = varx->var_data.var_num;
	ch = chvar->var_data.var_chtype;

	(void) c_wmove(win,y,x);
	(void) c_wchadd(win,ch);

	return(OK);
}

