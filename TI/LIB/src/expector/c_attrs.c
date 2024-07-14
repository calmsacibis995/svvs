# include "main.h"
# include <curses.h>

SCCSID("@(#)c_attrs.c	3.2 (SVID) 12/30/86");

/*
** c_offattr turns off the given attributes for the standard screen
*/

c_offattr(attrs_var)	/* catcher's version  of attroff */
VAR_ENTRY *attrs_var;
{
	int attrs;

	attrs = attrs_var->var_data.var_num;
   
	c_woffattr(std,attrs);
}

/*
** c_woffattr turns off the given attributes for the given window 
*/

c_woffattr(win,attrs)
TERMINAL *win;
int attrs;
{

/*
** This routine turns off the attributes in "attrs" for the window "win",
** without turning off any others. If an attribute is not available with
** the terminal type being used, the default attribute (set in c_initscr)
** is used.
*/

	if (attrs & A_BOLD)
	{
		if (tigetstr("bold") != NULL)
		{
			term_clr(win,TERM_BOLD);
		}
		else
		{
			term_clr(win,Default_attr);
		}
	}

	if (attrs & A_STANDOUT)
	{
			term_clr(win,Default_attr);
	}

	if (attrs & A_REVERSE)
	{
		if (tigetstr("rev") != NULL)
		{
			term_clr(win,TERM_REV);
		}
		else
		{
			term_clr(win,Default_attr);
		}
	}

	if (attrs & A_DIM)
	{
		if (tigetstr("dim") != NULL)
		{
			term_clr(win,TERM_HALF);
		}
		else
		{
			term_clr(win,Default_attr);
		}
	}

	if (attrs & A_BLINK)
	{
		if (tigetstr("blink") != NULL)
		{
			term_clr(win,TERM_BLINK);
		}
		else
		{
			term_clr(win,Default_attr);
		}
	}

	if (attrs & A_UNDERLINE)
	{
		if (tigetstr("smul") != NULL)
		{
			term_clr(win,TERM_UL);
		}
		else
		{
			term_clr(win,Default_attr);
		}
	}

	if (attrs & A_ALTCHARSET)
	{
		if (tigetstr("smacs") != NULL)
		{
			term_clr(win,TERM_ALTCH);
		}
		else
		{
			term_clr(win,Default_attr);
		}
	}
}

/*
** This routine turns on the given attributes for the standard
** screen without turning off any others.
*/

c_onattr(attrs_var)	/* catcher's version of attron */
VAR_ENTRY *attrs_var;
{
	int attrs;

	attrs = attrs_var->var_data.var_num;

	c_wonattr(std,attrs);
}

/*
** This routine turns on the attributes in "attrs" for the window "win"
** If an attribute is not present in the terminal type being used, 
** then the default attribute (set int c_initscr) is used
*/

c_wonattr(win,attrs)	/* catcher's version of wattron */
TERMINAL *win;
int attrs;
{

	if (attrs & A_BOLD)
	{
		if (tigetstr("bold") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute BOLD\n");
			}
# endif	/* TRACE */
			term_set(win,TERM_BOLD);
		}
		else
		{
			term_set(win,Default_attr);
		}
	}

	if (attrs & A_STANDOUT)
	{
		if (tigetstr("smso") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute STANDOUT\n");
			}
# endif	/* TRACE */
			term_set(win,Default_attr);
		}
	}

	if (attrs & A_REVERSE)
	{
		if (tigetstr("rev") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute REVERSE\n");
			}
# endif	/* TRACE */
			term_set(win,TERM_REV);
		}
		else
		{
			term_set(win,Default_attr);
		}
	}

	if (attrs & A_DIM)
	{
		if (tigetstr("dim") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute DIM\n");
			}
# endif	/* TRACE */
			term_set(win,TERM_HALF);
		}
		else
		{
			term_set(win,Default_attr);
		}
	}

	if (attrs & A_BLINK)
	{
		if (tigetstr("blink") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute BLINK\n");
			}
# endif	/* TRACE */
			term_set(win,TERM_BLINK	);
		}
		else
		{
			term_set(win,Default_attr);
		}
	}

	if (attrs & A_UNDERLINE)
	{
		if (tigetstr("smul") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute UNDERLINE\n");
			}
# endif	/* TRACE */
			term_set(win,TERM_UL);
		}
		else
		{
			term_set(win,Default_attr);
		}
	}

	if (attrs & A_ALTCHARSET)
	{
		if (tigetstr("smacs") != NULL)
		{
# ifdef TRACE
			if (Dbg)
			{
				printf("window will have attribute ALTCHARSET\n");
			}
# endif	/* TRACE */
			term_set(win,TERM_ALTCH);
		}
		else
		{
			term_set(win,Default_attr);
		}
	}
}

/*
** This routine turns off all existing attributes for the standard screen
** and assigns the given attributes.
*/
c_setattr(attrs_var)	/* catcher's version of attrset */
VAR_ENTRY *attrs_var;
{
	std->term_flag = 0L;

	c_onattr(attrs_var);
}

/*
** This is just like wonattr, except the existing attributes for win
** are eliminated.
*/
c_wsetattr(win,attrs)	/* catcher's version of wattrset */
TERMINAL *win;
int attrs;
{

	win->term_flag = 0L;

	c_wonattr(win,attrs);
}

/*
** This routine puts the standard screen in standout mode.
*/
c_standout()
{
	VAR_ENTRY attrvar;

	attrvar.var_data.var_num = A_STANDOUT;
	c_onattr(&attrvar);
}

/*
** This routine puts the given window in standout mode.
*/
c_wstandout(win)
TERMINAL *win;
{
	c_wonattr(win,A_STANDOUT);
}

/*
** This routine clears all the stacndard screen's attributes
*/
c_endstand()	/* catcher's version of standend */
{
	std->term_flag = 0L;
}

/*
** This routine clears all the given window's attributes.
*/

c_wendstand(win)	/* catcher's version of wstandend */
TERMINAL *win;
{
	win->term_flag = 0L;
}

