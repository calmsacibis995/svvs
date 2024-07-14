# include	<bless.h>
# include	<common.h>
# include	<macros.h>

SCCSID("@(#)function3.c	3.2 (SVID) 12/30/86");

/*
** getwidth
**	Return the width of the terminal, while taking into account
**	any soft margins that might be set.
**
** Return Value
**	The width of the screen (without margins).
**
** Arguments
**	trm	The TERMINAL we are interested in.
*/
getwidth(trm)
TERMINAL	*trm;
{
	int	width;

	/* 
	** if margins are set, compute width minus margins
	*/

	if ((trm->term_rmargin != trm->term_width) || (trm->term_lmargin != 0))
	{
		width = trm->term_width - (trm->term_lmargin + (trm->term_width - trm->term_rmargin));
		return width;
	}
	else
		return trm->term_width;
}

/*
** getx
**	Get the current x coordinate of the cursor (inside margins if set).
**
** Return Value
**	The X location.
**
** Arguments
**	trm	The TERMINAL we are interested in.
*/
getx(trm)
TERMINAL	*trm;
{
	int	xpos;

	/* 
	** If margins are set, compute x inside margins
	*/
	if ((trm->term_xloc < getlmargin(trm)) || (trm->term_xloc > getrmargin(trm)))
	{
		xpos = trm->term_xloc + getlmargin(trm);
# ifdef	TRACE
		if ( Trace(45,0) )
			print("getx: lm = %d rm = %d x = %d.\n", getlmargin(trm), getrmargin(trm), xpos);
# endif	/* TRACE */
		return (xpos);
	}
	else 
		return (trm->term_xloc);
}
/*
** direct
**	Put a character directly on the TERMINAL at the 
**	current cursor location. This takes into account
**	a lot of various things dealing with margins,
**	tabs, insert modes, etc.
**
** Arguments
**	term	Current TERMINAL to effect.
**	arg	The character to insert.
*/
int
/*ARGSUSED*/
direct(term,arg)
TERMINAL	*term;
ARGS	*arg;
{
	int	i;
	int	tab;
	int	xpos, ypos;
	char	buf[MAXX + 1];
	long	buf_bits[MAXX + 1];
	long	bits;

# ifdef	TRACE
	if ( Trace(40,0) )
		print("direct: (%d,%d) (%c)\n",getx(term),gety(term),arg->arg_char);
	if ( Trace(40,5) )
	{
		printf("direct: bits are ");
		printf("%o ",(term_on(term,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL)));
		if ( Trace(40,6) )
		{
			printf("\ndirect: flags are ");
			printflag(term,NULL);
		}
		printf("\n");
	}
# endif	/* TRACE */
	if ( term->term_onstatus )	/* on the status line */
	{
		/*
		** for now, this simply sticks the argument on the status
		** line and increments the x location on the status line
		*/

		/* special handling */
		if (arg->arg_char == '\n')
			return;
		if (arg->arg_char == '\t')
		{
			/*
			** We assume the status line has the same tabs as the
			** rest of the terminal. We now determine if there are
			** any tabs at all.
			*/
			for ( i = 0 ; i < MAXX ; i++ )
				if ( term->term_tabs[i] == 1 )
					break;
			if ( i == MAXX )	/* at the end */
				return;
				
			/* Search for the next tab stop */
			tab = getx(term) + 1;
			while ( term->term_tabs[tab] == 0 )
				tab++;

			/*
			** Destructive tabs are about like inserting spaces,
			** and deleteing whatever was there. We let the direct
			** routine handle scrolls.
			*/
			arg->arg_char = ' ';
			while ( getx(term) != tab )
				direct(term,arg);
			return;
		}
		/*
		getstat(term) = arg->arg_char;
		term->term_statusx++;
		*/
		getchr(term) = arg->arg_char;
		setx(term) += 1;
		return;
	}
	if ( getx(term) >= getwidth(term) || gety(term) >= getheight(term) )
		return;
	if ( arg->arg_char == '\n' )
	{
		setx(term) = 0;
		gety(term) += 1;
		if ( gety(term) >= getheight(term) )
		{
			scroll_term(term);
			gety(term) -= 1;
		}
		return;
	}
	/*
	** If INSERT is on, we have to move all the characters
	** after the current position right one space. We do this
	** with a memory copy.
	*/
	if ( term_on(term,TERM_INSERT) )
	{
		if ( getx(term) >= getwidth(term) )
		{
			setx(term) = 0;
			gety(term) += 1;
		}
		i = getwidth(term) - getx(term);

		if ( i < 0 )
			i = 0;
		(void)memcpy(buf,getpos(term),i);
		(void)memcpy(buf_bits,&term->term_bits[gety(term)][getx(term)],sizeof (long) * i);
		getchr(term) = arg->arg_char;
		getbits(term) = term_on(term,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL);
		setx(term) = getx(term) + 1;
		if ( i != 1 )
		{
			(void) memcpy(getpos(term),buf,i - 1);
			(void) memcpy(&term->term_bits[gety(term)][getx(term)],buf_bits,sizeof (long) * (i - 1));
		}

		return;
	}
	else
	{
		if ( getx(term) >= getwidth(term) )
		{
			if ( Bool_am )
			{
				setx(term) = 0;
				gety(term) += 1;
				if ( gety(term) >= getheight(term) )
				{
					scroll_term(term);
					gety(term) -= 1;
				}
			}
			else
				errormsg("direct: Insert after the end of a line, when there are no automatic margins.",term);
		}
		if ( arg->arg_char == '\t' )
		{
			for ( i = 0 ; i < MAXX ; i++ )
				if ( term->term_tabs[i] == 1 )
					break;
			if ( i == MAXX )
				return;
				
			/*
			** Search for the next tab stop, might be on the
			** next line.
			*/
			tab = getx(term) + 1;
			while ( term->term_tabs[tab] == 0 )
				tab = (tab + 1) % getwidth(term);

			/*
			** Destructive tabs are about like inserting spaces,
			** and deleteing whatever was there. We let the direct
			** routine handle scrolls.
			*/
			arg->arg_char = ' ';
			while ( getx(term) != tab )
				direct(term,arg);
			return;
		}
		else
		{
			getchr(term) = arg->arg_char;
			getbits(term) = term_on(term,TERM_REV|TERM_HALF|TERM_BLINK|TERM_ALTCH|TERM_BOLD|TERM_SO|TERM_UL);
		}
	}
	setx(term) += 1;
	if ( getx(term) == getwidth(term) && Bool_am )
	{
		setx(term) = 0;
		gety(term) += 1;
		if ( gety(term) >= getheight(term) )
		/* greater than or equal to? */
		{
			scroll_term(term);
			gety(term) -= 1;
		}
	}
}

/*
** num_endspaces
**	Return the number of spaces at the end of a string.
**
** Arguments
**	str	The string
**	len	The length of the string.
**
** Return value
**	The number of spaces at the end of the string.
*/
num_endspaces(str,len)
char	*str;
int	len;
{
	int	count;

	count = 0;

	while ( len )
	{
		len--;
		if ( str[len] == ' ' )
			count++;
		else
			break;
	}
	return (count);
}

/*
** scroll_term
**	Scroll the TERMINAL structure. Remove the first line, and
**	push everything else up. Leave a blank line at the bottom.
**
** Arguments
**	term	The TERMINAL to scroll.
*/
scroll_term(term)
TERMINAL	*term;
{
	char	*from;		/* copy strings from here */
	char	*to;		/* to here */
	long	*from_bits;	/* copy attributes from here */
	long	*to_bits;	/* to here */
	int	height;		/* height of the screen. */
	int	width;		/* width of the screen */
	int	next;		/* index to the next string */

	width = getwidth(term);
	height = getheight(term) - 1;

	to = getstring(term,0);
	from = getstring(term,1);
	to_bits = term->term_bits[0];
	from_bits = term->term_bits[1];
	next = 2;

	while ( height-- )
	{
		(void) memcpy(to,from,width);
		(void) memcpy(to_bits,from_bits,sizeof (long) * width);
		to = from;
		from = getstring(term,next);
		to_bits = from_bits;
		from_bits = term->term_bits[next];
		next++;
	}
	(void) memset(to,' ',width);
	(void) memset(to_bits,'\0',sizeof (long) * width);
}
