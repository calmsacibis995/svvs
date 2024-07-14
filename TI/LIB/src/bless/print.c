# include	<bless.h>
# include	<common.h>
# include	<ctype.h>
# include	<varargs.h>


SCCSID("@(#)print.c	3.2 (SVID) 12/30/86");

# define	LINELEN	1000		/* Maximum length of any printed line. */
# define	STRBUF	5000		/* hopefully enough for 4 80 character strings, + 2 lines of "Line: %d ..." info. */

extern	char	*tigetstr();

/*
** print
**	A version of printf that prints control characters in ^X
**	format. This does not support floating point numbers.
**
** Arguments
**	(1) A format string (like printf).
**	(2) Any list of arguments.
*/
void
print(va_alist)
va_dcl
{
	va_list	ap;
	char	*format;
	char	hold[2];
	char	fmt[3];
	char	*tmp;
	int	i;
	extern	void	show();

	va_start(ap);

	tmp = hold;
	hold[1] = '\0';
	fmt[0] = '%';
	fmt[2] = '\0';
	format = va_arg(ap,char *);
	while ( *format )
	{
		if ( *format != '%' )
		{
			*tmp = *format;
			show(tmp);
		}
		else
		{
			format++;
			switch (*format)
			{
			  case '%':
				printf("%%");
			  case 'd':
			  case 'o':
			  case 'u':
			  case 'x':
			  case 'X':
				fmt[1] = *format;
				printf(fmt,va_arg(ap,int));
				break;

			  case 's':
				show(va_arg(ap,char *));
				break;

			  case 'c':
				i = va_arg(ap,int);
				*tmp = (char) i;
				show(tmp);
				break;

			  default:
				*tmp = *format;
				show(tmp);
				break;
			}
		}
		format++;
	}

	va_end(ap);
}

/*
** show
**	Print a string, with control characters
**	printed in ^X format.
**
** Arguments
**	str	The string to print.
*/
void
show(str)
char	*str;
{
	char	ch;

	while ( *str )
	{
		ch = *str & 0177;
		if ( ch != '\n' )
			printf("%s",unctrl(ch));
		else
			printf("\n");
		str++;
	}
	
}

/*
** printarg
**	Given an ARGS list, print them all out in some nice format.
**
** Arguments
**	arg	The ARGS list.
*/
printarg(arg)
ARGS	*arg;
{
	while ( arg != (ARGS *) NULL )
	{
		switch (arg->arg_type)
		{
		  case ARG_INT:
			printf("%d",arg->arg_int);
			break;

		  case ARG_CHAR:
			print("%c",arg->arg_char);
			break;

		  case ARG_STR:
			print("%s",arg->arg_str);
			break;
		}
		arg = arg->arg_next;
		if ( arg != (ARGS *) NULL )
			print(", ");
	}
}

/*
** printterm
**	Print out a TERMINAL structure. This is just a front end.
**	If we are debugging, we go to a routine that prints out
**	characters with attributes on, else just a normal routine
**	is called.
**
** Arguments
**	term	The TERMINAL to print out.
*/
printterm(term)
TERMINAL	*term;
{
# ifdef	TRACE
	if ( Trace(51,0) )
		attrib_print(term);
	else
# endif	/* TRACE */
		term_print(term);
}

/*
** term_print
**	Print out a TERMINAL structure.
**
** Arguments
**	term	The TERMINAL to print out.
*/
static	int
term_print(term)
TERMINAL	*term;
{
	int	x,y;
	char	*str;
	int	height;

	height = term->term_height;
	if ( term_on(term,TERM_STATUS) )
		height++;

	for ( y = 0 ; y < (height - 1) ; y++ )
	{
		str = term->term_screen[y];

		for ( x = 0 ; x < term->term_width ; x++ )
			print("%c",*str++);
		if ( term->term_width != 80 )
			printf("\n");
	}
	str = term->term_screen[y];

	for ( x = 0 ; x < (term->term_width - 1) ; x++ )
		print("%c",*str++);
	if ( term->term_width != 80 )
		printf("\n");
	(void) fflush(stdout);
}

turn_off_char_attrs(term,y,x)
int y,x;
TERMINAL *term;
{
	if (t_bit_on(term,y,x,TERM_REV))
	{
		printf(tigetstr("sgr0"));
	}

	if (t_bit_on(term,y,x,TERM_HALF))
	{
		printf(tigetstr("sgr0"));
	}

	if (t_bit_on(term,y,x,TERM_BLINK))
	{
		printf(tigetstr("sgr0"));
	}

	if (t_bit_on(term,y,x,TERM_BOLD))
	{
		printf(tigetstr("sgr0"));
	}

	if (t_bit_on(term,y,x,TERM_SO))
	{
		printf(tigetstr("sgr0"));
	}

	if (t_bit_on(term,y,x,TERM_UL))
	{
		if (tigetstr("smul") != NULL)
		{
			printf(tigetstr("rmul"));
		}
		else
		{
			printf(tigetstr("sgr0"));
		}
	}
}

set_screen_attrs(term)
TERMINAL *term;
{
	char *ch;

	if (term_on(term,TERM_BELL))
	{
		if ((ch = tigetstr("bel")) != NULL)
		{
			printf(ch);
		} 
		else 
		{
			printf(tigetstr("smso"));
		}
	}
	
	if (term_on(term,TERM_INSERT))
	{
		if ((ch = tigetstr("smir")) != NULL)
		{
			printf(ch);
		}
		else
		{
			printf(tigetstr("smso"));
		}
	}
	
	if (term_on(term,TERM_CURINV))
	{
		if ((ch = tigetstr("civis")) != NULL)
		{
			printf(ch);
		}
		else
		{
			printf(tigetstr("smso"));
		}
	}
	
	if (term_on(term,TERM_CURSO))
	{
		printf(tigetstr("smso"));
	}
	
	if (term_on(term,TERM_STALTCH))
	{
		if ((ch = tigetstr("smacs")) != NULL)
		{
			printf(ch);
		}
		else
		{
			printf(tigetstr("smso"));
		}
	}
	
	if (term_on(term,TERM_VB))
	{
		if ((ch = tigetstr("flash")) != NULL)
		{
			printf(ch);
		}
		else
		{
			printf(tigetstr("smso"));
		}
	}
}

turnoff_screenattrs(term)
TERMINAL *term;
{
	if (term_on(term,TERM_BELL))
	{
		if (tigetstr("bel") != NULL)
		{
			printf("sgr0");
		}         
	}

	if (term_on(term,TERM_INSERT))
	{
		if (tigetstr("smir") != NULL)
		{
			printf(tigetstr("rmir"));
		}
		else
		{
			printf(tigetstr("sgr0"));
		}
	}
	
	if (term_on(term,TERM_CURINV))
	{
		printf(tigetstr("sgr0"));
	}

	if (term_on(term,TERM_CURSO))
	{
		printf(tigetstr("sgr0"));
	}

	if (term_on(term,TERM_ALTCH))
	{
		if (tigetstr("smacs") != NULL)
		{
			printf(tigetstr("rmacs"));
		}
		else
		{
			printf(tigetstr("sgr0"));
		}
	}

	if (term_on(term,TERM_VB))
	{
		printf(tigetstr("sgr0"));
	}

}


# ifdef	TRACE
/*
** term_print
**	Print out a TERMINAL structure, with attributes displayed
**	appropriately.
**
** Arguments
**	term	The TERMINAL to print out.
*/
static	int
attrib_print(term)
TERMINAL	*term;
{
	int	x,y;
	char	*str, *ch;
	int	printlast;

	printlast = 1;
	for ( y = 0 ; y < term->term_height ; y++ )
	{
		set_screen_attrs(term);
		if ( y == (term->term_height - 1) )
			printlast = 0;
	
		str = term->term_screen[y];

		for ( x = 0 ; x < term->term_width ; x++ )
		{
			if ( printlast == 0 && x == (term->term_width - 1) )
				break;

			if (t_bit_on(term,y,x,TERM_HALF))
			{
				if ((ch = tigetstr("dim")) != NULL)
				{
					printf(ch);
				}   
				else
				{
					printf(tigetstr("smso"));
				}
			}
			
			if (t_bit_on(term,y,x,TERM_REV))
			{
				if ((ch = tigetstr("rev")) != NULL)
				{
					printf(ch);
				}
				else
				{
					printf(tigetstr("smso"));
				}
			}
		
			if (t_bit_on(term,y,x,TERM_BLINK))
			{    
				if ((ch = tigetstr("blink")) != NULL)
				{

					printf(ch);
				}
				else
				{
					printf(tigetstr("smso"));
				}
			}
			
			if (t_bit_on(term,y,x,TERM_BOLD))
			{
				if ((ch = tigetstr("bold")) != NULL)
				{
					printf(ch);
				}
				else
				{
					printf(tigetstr("smso"));
				}
			}
			
			if (t_bit_on(term,y,x,TERM_SO))
			{
				printf(tigetstr("smso"));
			}
			
			if (t_bit_on(term,y,x,TERM_UL))
			{
				if ((ch = tigetstr("smul")) != NULL)
				{
					printf(ch);
				}
				else
				{
					printf(tigetstr("smso"));
				}
			}

			if (t_bit_on(term,y,x,TERM_ALTCH))
			{
				if ((ch = tigetstr("smacs")) != NULL)
				{
					printf(ch);
				}
				else
				{
					printf(tigetstr("smso"));
				}
			}
			printf("%c",*str++);

			turn_off_char_attrs(term,y,x);

		}
	}

	turnoff_screenattrs(term);
	fflush(stdout);
}
# endif	/* TRACE */

/*
** printlines
**	Given two lines, print them out with the differences marked.
**
** Arguments
**	str1	The first string
**	b1	The attributes associated with string one
**	len1	The length of the first string
**	str2	The second string
**	b2	The attributes associated with string two
**	len2	The length of the second string
**
*/
printlines(linenum,str1,b1,len1,str2,b2,len2)
int	linenum;
char	*str1;
long	*b1;
int	len1;
char	*str2;
long	*b2;
int	len2;
{
	int	l1,l2;
	long	*bits1;
	long	*bits2;
	char	*sp1, *sp2;
	char	*sp;
	int	rest;
	int	attsdiff = 0;
	int	i;
	char	err_buf[STRBUF];
	char	buf[LINELEN];
	char	blink_buf[LINELEN];
	char	reverse_buf[LINELEN];
	char	half_buf[LINELEN];
	char	bold_buf[LINELEN];
	char	under_buf[LINELEN];
	char	stand_buf[LINELEN];

	l1 = len1;
	l2 = len2;
	sp1 = str1;
	sp2 = str2;
	bits1 = b1;
	bits2 = b2;

# ifdef	TRACE
	if ( Trace(50,0) )
		printf("printline:(%d,%15s,%o,%d,%15s,%o,%d)\n",linenum,str1,*bits1,l1,str2,*bits2,l2);
# endif	/* TRACE */

	sprintf(err_buf,"Line %d (first is the expected line, the next 7 (if there)  indicate attributes, the last is the actual line):\n",linenum);
	(void) strcat(err_buf,		"            ");
	(void) strcpy(buf,		"Characters :");
	(void) strcpy(blink_buf,	"Blinking   :");
	(void) strcpy(reverse_buf,	"Reverse    :");
	(void) strcpy(half_buf,		"Half Bright:");
	(void) strcpy(bold_buf,		"Bold       :");
	(void) strcpy(under_buf,	"Underline  :");
	(void) strcpy(stand_buf,	"Standout   :");
	/*
	** Print the first line.
	*/
	while ( l1-- )
	{
		(void) strcat(err_buf,unctrl(*sp1));
		/*
		** Print an extra space if the corresponding
		** character in the second string is a control
		** character. We print the space, as control
		** characters are printed in the ^X format (i.e.
		** they take up two character positions).
		*/
		if ( isprint(*sp1) && !isprint(*sp2) )
			(void) strcat(err_buf," ");
		if ( (*bits1&(TERM_UL|TERM_SO|TERM_HALF|TERM_BOLD|TERM_BLINK|TERM_REV)) != (*bits2&(TERM_UL|TERM_SO|TERM_HALF|TERM_BOLD|TERM_BLINK|TERM_REV)) )
			attsdiff = 1;
		sp1++;
		sp2++;
		bits1++;
		bits2++;
		if ( l2-- <= 0 )
			break;
	}

	/*
	** In case the first string is longer than the second.
	*/
	while ( --l1 > 0 )
		(void) strcat(err_buf,unctrl(*sp1++));

	(void) strcat(err_buf,"\n");

	/*
	** Print the status lines.
	*/
	l1 = len1;
	l2 = len2;
	sp1 = str1;
	sp2 = str2;
	bits1 = b1;
	bits2 = b2;

	while ( l1-- )
	{
		if ( *sp1 == *sp2 && *bits1 == *bits2 )
		{
			(void) strcat(buf," ");
			(void) strcat(blink_buf," ");
			(void) strcat(reverse_buf," ");
			(void) strcat(half_buf," ");
			(void) strcat(bold_buf," ");
			(void) strcat(under_buf," ");
			(void) strcat(stand_buf," ");

		}
		else
		{
			if ( *sp1 != *sp2 )
				(void) strcat(buf,"^");
			else
				(void) strcat(buf," ");
			if ( !isprint(*sp1) || !isprint(*sp2) )
				(void) strcat(buf,"*");
			if ( *bits1 != *bits2 )
			{
				if ( (*bits1&TERM_REV) == (*bits2&TERM_REV) )
					(void) strcat(reverse_buf," ");
				else if ( (*bits1&TERM_REV) > (*bits2&TERM_REV) )
					(void) strcat(reverse_buf,"^");
				else
					(void) strcat(reverse_buf,"V");
				if ( (*bits1&TERM_BLINK) == (*bits2&TERM_BLINK) )
					(void) strcat(blink_buf," ");
				else if ( (*bits1&TERM_BLINK) > (*bits2&TERM_BLINK) )
					(void) strcat(blink_buf,"^");
				else
					(void) strcat(blink_buf,"V");
				if ( (*bits1&TERM_BOLD) == (*bits2&TERM_BOLD) )
					(void) strcat(bold_buf," ");
				else if ( (*bits1&TERM_BOLD) > (*bits2&TERM_BOLD) )
					(void) strcat(bold_buf,"^");
				else
					(void) strcat(bold_buf,"V");
				if ( (*bits1&TERM_SO) == (*bits2&TERM_SO) )
					(void) strcat(stand_buf," ");
				else if ( (*bits1&TERM_SO) > (*bits2&TERM_SO) )
					(void) strcat(stand_buf,"^");
				else
					(void) strcat(stand_buf,"V");
				if ( (*bits1&TERM_UL) == (*bits2&TERM_UL) )
					(void) strcat(under_buf," ");
				else if ( (*bits1&TERM_UL) > (*bits2&TERM_UL) )
					(void) strcat(under_buf,"^");
				else
					(void) strcat(under_buf,"V");
				if ( (*bits1&TERM_HALF) == (*bits2&TERM_HALF) )
					(void) strcat(half_buf," ");
				else if ( (*bits1&TERM_HALF) > (*bits2&TERM_HALF) )
					(void) strcat(half_buf,"^");
				else
					(void) strcat(half_buf,"V");
			}
		}
		sp1++;
		sp2++;
		bits1++;
		bits2++;
		if ( l2-- <= 0 )
			break;
	}
	(void) strcat(buf,"\n");
	(void) strcat(blink_buf,"\n");
	(void) strcat(reverse_buf,"\n");
	(void) strcat(half_buf,"\n");
	(void) strcat(bold_buf,"\n");
	(void) strcat(under_buf,"\n");
	(void) strcat(stand_buf,"\n");
	(void) strcat(err_buf,buf);
	if ( attsdiff )
	{
		(void) strcat(err_buf,blink_buf);
		(void) strcat(err_buf,reverse_buf);
		(void) strcat(err_buf,half_buf);
		(void) strcat(err_buf,bold_buf);
		(void) strcat(err_buf,under_buf);
		(void) strcat(err_buf,stand_buf);

	}


	(void) strcat(err_buf,"            ");
	l1 = len1;
	l2 = len2;
	sp1 = str1;
	sp2 = str2;
	bits1 = b1;
	bits2 = b2;
	/*
	** Print the second line.
	*/
	while ( l2-- )
	{
		(void) strcat(err_buf,unctrl(*sp2));
		if ( isprint(*sp2) && !isprint(*sp1) )
			(void) strcat(err_buf," ");
		sp1++;
		sp2++;
		bits1++;
		bits2++;
		if ( l1-- <= 0 )
			break;
	}

	/*
	** In case the second string is longer than the first.
	*/
	while ( --l2 > 0 )
		(void) strcat(err_buf,unctrl(*sp2++));
	(void) strcat(err_buf,"\n");
	msg_drop(err_buf);

}

/*
** printbits
**	Prints out all the bits in the term_flag field in a
**	human readable format.
**
** Arguments
**	term	The TERMINAL structure with term_flag.
**	fp	The FILE to print it out to.
*/
printflag(term,fp)
TERMINAL	*term;
FILE	*fp;
{
	if ( fp == NULL )
		fp = stdout;

	if ( term_on(term,TERM_BELL) )
		fprintf(fp,"TERM_BELL ");
	if ( term_on(term,TERM_REV) )
		fprintf(fp,"TERM_REV ");
	if ( term_on(term,TERM_HALF) )
		fprintf(fp,"TERM_HALF ");
	if ( term_on(term,TERM_TABS) )
		fprintf(fp,"TERM_TABS ");
	if ( term_on(term,TERM_BLINK) )
		fprintf(fp,"TERM_BLINK ");
	if ( term_on(term,TERM_INSERT) )
		fprintf(fp,"TERM_INSERT ");
	if ( term_on(term,TERM_CURINV) )
		fprintf(fp,"TERM_CURINV ");
	if ( term_on(term,TERM_CURSO) )
		fprintf(fp,"TERM_CURSO ");
	if ( term_on(term,TERM_ALTCH) )
		fprintf(fp,"TERM_ALTCH ");
	if ( term_on(term,TERM_STALTCH) )
		fprintf(fp,"TERM_STALTCH ");
	if ( term_on(term,TERM_BOLD) )
		fprintf(fp,"TERM_BOLD ");
	if ( term_on(term,TERM_CMINIT) )
		fprintf(fp,"TERM_CMINIT ");
	if ( term_on(term,TERM_SO) )
		fprintf(fp,"TERM_SO ");
	if ( term_on(term,TERM_UL) )
		fprintf(fp,"TERM_UL ");
	if ( term_on(term,TERM_VB) )
		fprintf(fp,"TERM_VB ");
	if ( term_on(term,TERM_INIT) )
		fprintf(fp,"TERM_INIT ");
	if ( term_on(term,TERM_INIT1) )
		fprintf(fp,"TERM_INIT1 ");
	if ( term_on(term,TERM_INIT2) )
		fprintf(fp,"TERM_INIT2 ");
	if ( term_on(term,TERM_INIT3) )
		fprintf(fp,"TERM_INIT3 ");
	if ( term_on(term,TERM_KBT) )
		fprintf(fp,"TERM_KBT ");
	if ( term_on(term,TERM_SUBWIN) )
		fprintf(fp,"TERM_SUBWIN ");

	if ( term_on(term,TERM_INVIS) )
		fprintf(fp,"TERM_INVIS ");
	if ( term_on(term,TERM_STATUS) )
		fprintf(fp,"TERM_STATUS ");
	if ( term_on(term,TERM_PROT) )
		fprintf(fp,"TERM_PROT ");

}

/*
** printbits
**	Prints out all the bits in the flag in a
**	human readable format.
**
** Arguments
**	flag	Flag structure to look at.
**	fp	The FILE to print it out to.
*/
printbits(flag,fp)
long	flag;
FILE	*fp;
{
	TERMINAL	t, *term;

	term = &t;
	term->term_flag = flag;
	if ( fp == NULL )
		fp = stdout;

	if ( term_on(term,TERM_BELL) )
		fprintf(fp,"TERM_BELL ");
	if ( term_on(term,TERM_REV) )
		fprintf(fp,"TERM_REV ");
	if ( term_on(term,TERM_HALF) )
		fprintf(fp,"TERM_HALF ");
	if ( term_on(term,TERM_TABS) )
		fprintf(fp,"TERM_TABS ");
	if ( term_on(term,TERM_BLINK) )
		fprintf(fp,"TERM_BLINK ");
	if ( term_on(term,TERM_INSERT) )
		fprintf(fp,"TERM_INSERT ");
	if ( term_on(term,TERM_CURINV) )
		fprintf(fp,"TERM_CURINV ");
	if ( term_on(term,TERM_CURSO) )
		fprintf(fp,"TERM_CURSO ");
	if ( term_on(term,TERM_ALTCH) )
		fprintf(fp,"TERM_ALTCH ");
	if ( term_on(term,TERM_STALTCH) )
		fprintf(fp,"TERM_STALTCH ");
	if ( term_on(term,TERM_BOLD) )
		fprintf(fp,"TERM_BOLD ");
	if ( term_on(term,TERM_CMINIT) )
		fprintf(fp,"TERM_CMINIT ");
	if ( term_on(term,TERM_SO) )
		fprintf(fp,"TERM_SO ");
	if ( term_on(term,TERM_UL) )
		fprintf(fp,"TERM_UL ");
	if ( term_on(term,TERM_VB) )
		fprintf(fp,"TERM_VB ");
	if ( term_on(term,TERM_INIT) )
		fprintf(fp,"TERM_INIT ");
	if ( term_on(term,TERM_KBT) )
		fprintf(fp,"TERM_KBT ");
	if ( term_on(term,TERM_SUBWIN) )
		fprintf(fp,"TERM_SUBWIN ");

	if ( term_on(term,TERM_INVIS) )
		fprintf(fp,"TERM_INVIS ");
	if ( term_on(term,TERM_STATUS) )
		fprintf(fp,"TERM_STATUS ");
	if ( term_on(term,TERM_PROT) )
		fprintf(fp,"TERM_PROT ");

}

/*
** dump_term
**	Print out a TERMINAL structure, and then the flags associated
**	with the TERMINAL.
**
** Arguments
**	term	The TERMINAL structure
**	fp	FILE	pointer
*/
dump_term(term,fp)
TERMINAL	*term;
FILE	*fp;
{
	int	x, y;
	long	*bits;
	char	*str;
	int	height;

	if ( fp == NULL )
		fp = stdout;

	fprintf(fp,"TERMINAL:\n");
	height = term->term_height;
	if ( term_on(term,TERM_STATUS) )
		height++;

	for ( y = 0 ; y <= height ; y++ )
	{
		str = term->term_screen[y];

		fprintf(fp,"%2d: ",y + 1);
		for ( x = 0 ; x < term->term_width ; x++ )
			fprintf(fp,"%c",*str++);
		if ( term->term_width < 80 )
			fprintf(fp,"\n");
	}
	for ( y = 0 ; y <= height ; y++ )
	{
		bits = term->term_bits[y];

		fprintf(fp,"%2d: ",y + 1);
		for ( x = 0 ; x < term->term_width ; x++ )
			printbits(bits[x],fp);
		if ( term->term_width < 80 )
			fprintf(fp,"\n");
	}
}
