# include "main.h"
# include <curses.h>

SCCSID("@(#)c_insch.c	3.1 (SVID) 12/1/86");

# define	getline(trm)	trm->term_screen[trm->term_yloc]
# define	getstring(trm,y) trm->term_screen[y]
# define	getpos(trm)	&trm->term_screen[trm->term_yloc][trm->term_xloc]
# define	getchr(trm)	trm->term_screen[trm->term_yloc][trm->term_xloc]
# define	getbits(trm)	trm->term_bits[trm->term_yloc][trm->term_xloc]
# define	getheight(trm)	trm->term_height
# define	getwidth(trm)	trm->term_width
# define	getx(trm)	trm->term_xloc
# define	gety(trm)	trm->term_yloc

c_insch(chvar)
VAR_ENTRY *chvar;
{
	chtype cht;

	cht = chvar->var_data.var_chtype;

	(void) c_winsch(std,cht);

	return(OK);
}

c_winsch(win,cht) 
TERMINAL *win;
chtype cht;
{
	char ch, *str;
	TERMINAL *target;
	int len, i, j, saved_y2, saved_x2, saved_y, saved_x;
	char buf[MAXX + 1];
	long buf_bits[MAXX + 1];

	extern char *memcpy();
	/*
	** If the given window is a subwindow, we need to get to the parent window,
	** since that's where the text is stored.
	*/

	ch = (cht & A_CHARTEXT);

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
	** the rest of the line and set the cursor to the next row.
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
		{
			*str++ = ' ';
			target->term_bits[gety(target)][len] = term_on(win,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL);
			set_char_attrs(target,gety(target),len,(int) cht & A_ATTRIBUTES);
		}
	}
	else if (ch == '\t')
	{   
			saved_y2 = win->term_yloc;
			saved_x2 = win->term_xloc;
			term_set(target,TERM_INSERT);

			i = (((getx(win) / TABLEN) + 1) * TABLEN) - getx(win);
			
			for ( j = 0; j < i; j++ )
			{
				cht = (' '| (cht & A_ATTRIBUTES));
				(void) c_wchadd(win,cht);
			}

			term_clr(target,TERM_INSERT);
			win->term_yloc = saved_y2;
			win->term_xloc = saved_x2;

			return(OK);
	}
	else
	{

	/*
	** In most cases, however, we first save the portion of line that will
	** be moved over as part of the insertion.
	*/

		str = getline(target);
		
		(void)memcpy(buf,&str[getx(target)],abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));

		(void)memcpy((char *) buf_bits,(char *) &target->term_bits[gety(target)][getx(target)], (int) sizeof(long) * abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));
		
	/*
	** Then, we place the new character at the current cursor position.
	*/

		getchr(target) = ch;
	 			
	/*
	** We set the attributes for that character according to the attributes that
	** have been set for the window as a whole and also the attributes passed 
	** with the character itself
	*/

		getbits(target) = term_on(win,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL);
	

		set_char_attrs(target,gety(target),getx(target),(int) cht & A_ATTRIBUTES);

			(void)memcpy(&str[getx(target) + 1],buf,abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));

	/*
	** Finally, we copy back the rest of the line, only one space over-- losing
	** the last character in the process.
	*/


		(void)memcpy((char *) &target->term_bits[gety(target)][getx(target) + 1],(char *) buf_bits, (int) sizeof(long) * abs((win->term_winx - target->term_winx) + win->term_width - (1 + getx(target))));
		

	}
			
	if (target != win)
	{
		target->term_yloc = saved_y;
		target->term_xloc = saved_x;
	}

	return(OK);
}

c_mvinsch(vary,varx,chvar)
VAR_ENTRY *vary,*varx,*chvar;
{
	(void) c_move(vary,varx);
	(void) c_insch(chvar);

	return(OK);
}

c_mvwinsch(winvar,vary,varx,chvar)
VAR_ENTRY *winvar, *vary, *varx, *chvar;
{
	int y,x;
	TERMINAL *win;
	chtype ch;

	win = winvar->var_data.var_term;
	y = vary->var_data.var_num;
	x = varx->var_data.var_num;
	ch = chvar->var_data.var_chtype;

	(void) c_wmove(win,y,x);
	(void) c_winsch(win,ch);

	return(OK);
}

