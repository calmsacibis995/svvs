# include "main.h"
# include <curses.h>

SCCSID("@(#)c_initscr.c	3.1 (SVID) 12/1/86");

int Dbg;
char *TypeAhead;
TERMINAL *saved_scr;

set_default_attr()
{

/*
** This routine examines the command string for standout mode ("smso")
** and determines what attributes it is made up of, if that is the case.
** It then sets the variable "Default_Attr" accordingly.
*/

	char *str, *subj;
	int other_than_standout = 0;
	extern char *tigetstr();

	subj = tigetstr("smso");

	if (subj == NULL)
	{
# ifdef TRACE
		if (Dbg)
		{
			printf("Default_attr is 0\n");
		}
# endif	/* TRACE */
		Default_attr = 0L;
		return(OK);
	}

	if ((str = tigetstr("blink")) != NULL)
	{
		while (*subj != '\0')
		{
			if (strncmp(subj,str,strlen(str)) == 0)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("Default_attr contains BLINK\n");
				}
# endif	/* TRACE */
				Default_attr |= TERM_BLINK;
				other_than_standout = 1;
				break;
			}
			else
			{
				subj++;
			}
		}
	}

	subj = tigetstr("smso");

	if ((str = tigetstr("rev")) != NULL)
	{
		while (*subj != '\0')
		{
			if (strncmp(subj,str,strlen(str)) == 0)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("Default_attr contains REV\n");
				}
# endif	/* TRACE */
				Default_attr |= TERM_REV;
				other_than_standout = 1;
				break;
			}
			else
			{
				subj++;
			}
		}
	}
	
	subj = tigetstr("smso");

	if ((str = tigetstr("dim")) != NULL)
	{
		while (*subj != '\0')
		{
			if (strncmp(subj,str,strlen(str)) == 0)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("Default_attr contains DIM\n");
				}
# endif	/* TRACE */
				Default_attr |= TERM_HALF;
				other_than_standout = 1;
				break;
			}
			else
			{
				subj++;
			}
		}
	}

	subj = tigetstr("smso");

	if ((str = tigetstr("smul")) != NULL)
	{
		while (*subj != '\0')
		{
			if (strncmp(subj,str,strlen(str)) == 0)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("Default_attr contains UL\n");
				}
# endif	/* TRACE */
				Default_attr |= TERM_UL;
				other_than_standout = 1;
				break;
			}
			else
			{
				subj++;
			}
		}
	}

	subj = tigetstr("smso");

	if ((str = tigetstr("bold")) != NULL)
	{
		while (*subj != '\0')
		{
			if (strncmp(subj,str,strlen(str)) == 0)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("Default_attr contains BOLD\n");
				}
# endif	/* TRACE */
				Default_attr |= TERM_BOLD;
				other_than_standout = 1;
				break;
			}
			else
			{
				subj++;
			}
		}
	}

	subj = tigetstr("smso");

	if ((str = tigetstr("smacs")) != NULL)
	{
		while (*subj != '\0')
		{
			if (strncmp(subj,str,strlen(str)) == 0)
			{
# ifdef TRACE
				if (Dbg)
				{
					printf("Default_attr contains ALTCH\n");
				}
# endif	/* TRACE */
				Default_attr |= TERM_ALTCH;
				other_than_standout = 1;
				break;
			}
			else
			{
				subj++;
			}
		}
	}

	if (!other_than_standout)
	{
		Default_attr |= TERM_SO;
# ifdef TRACE
		if (Dbg)
		{
			printf("Default_attr is just plain STANDOUT\n");
		}
# endif	/* TRACE */
	}

	return(TRUE);
}

TERMINAL *c_initscr()
{
	extern TERMINAL *c_newwin();
	extern char *getenv();
	extern char *tigetstr();

	(void) c_echo();

	if (strcmp(getenv("debugging"),"YES") == 0)
	{
		Dbg = 1;
	}
	else
	{
		Dbg = 0;
	}

/*
** "TypeAhead" will contain the current state of the keyboard input.
** Characters are lopped off the front end when read, and appended to
** the back end when more characters are typed in.
*/

	TypeAhead = malloc((unsigned) 1);
	if (TypeAhead == NULL)
	{
		screech("c_initscr",0,"ERROR in c_initscr: initial malloc of TypeAhead failed\n");
	}
	*TypeAhead = '\0';

/*
** "DISPLAY" is our version of the virtual screen -- that is, it
** represents what the programmer wishes to see on the screen.
*/

	DISPLAY = c_newwin(tgetnum("li"),tgetnum("co"),0,0);
	saved_scr = c_newwin(tgetnum("li"),tgetnum("co"),0,0);
	std = c_newwin(tgetnum("li"),tgetnum("co"),0,0);
/*
** c_LINES and c_COLS are the height and width of the current terminal
** screen.
*/

	c_LINES = tgetnum("li");
	c_COLS = tgetnum("co");

	/*
	** Here we check if the terminal has its own status line. If so, the
	** virtual screen increases in height by one. Otherwise, the status line
	** occupies the bottom line of the standard screen.
	*/

	if (Status_line_initialized)
	{
		if (uses_sl() == OK)
		{
			DISPLAY->term_height += 1;
			saved_scr->term_height += 1;
			status_line->term_winy = DISPLAY->term_height - 1;
		}
		else
		{
			std->term_height -= 1;
			c_LINES -= 1;
		}
	}

	/*
	** "Standout" is the default attribute used when an attempt is made to
	** set an attribute that does not exist in the given terminal type.
	** However, "Standout" can be comprised of one or more different
	** attributes. "Set_Default_Attr" finds these attributes and sets
	** "Default_Attr" accordingly.
	*/

	(void) set_default_attr();
	(void) c_werase(DISPLAY);
	(void) c_wclear(std);
	 
	/*
	** We now check if the cursor motion      
	** initialization sequence have been sent, and set the corresponding
	** globals accordingly.
	*/

	if (tigetstr("smcup") != NULL)
	{
		term_set(DISPLAY,TERM_CMINIT);
	}

	return(std);

}

