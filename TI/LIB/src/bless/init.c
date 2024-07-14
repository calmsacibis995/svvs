# include	<bless.h>
# include	<common.h>
# include	<memory.h>
# include	<string.h>
# include	<search.h>

SCCSID("@(#)init.c	3.3 (SVID) 12/30/86");

TERMINAL *term_malloc();

/*
** init_terminfo
**	Read in, and initialize the terminfo data base for
**	this psuedo-terminal. This routine will initialize
**	everything we need to know (hopefully).
**
** Arguments
**	term_type	The terminal type.
**
** Return value
**	TERMINAL *	A pointer to an initilized TERMINAL.
**	NULL		on any error.
**
** Side Effects
**	Sets up the "Commands" tree,
**	and the booleans.
*/
TERMINAL	*
init_terminfo(term_type)
char	*term_type;
{
	char			*ret_val;
	int			errval;
	COMM			*commptr;
	struct	_termdata	*ptr;
	struct	_bools		*bools;
	char			id[3];
	TERMINAL		*term;
	int			i;
	int			lines;
	int			columns;
	int			tabstops;
	extern	int	compare();
	extern	char	*tgetstr();
	extern	char	*calloc();

# ifdef	TRACE
	if ( Trace(1,0) )
		print("init_terminfo(%s)\n",term_type);
# endif	/* TRACE */
	if ( setupterm(term_type,1,&errval) == ERR )
	{
		fprintf(stderr,"setupterm returned ERR, errval == %d\n",errval);
		return ((TERMINAL *) NULL);
	}

	ptr = Termdata;

	while ( ptr->data_name != NULL )
	{
# ifdef	TRACE
		if ( Trace(1,5) )
			print("init_terminfo: tgetstr(%s) == ",ptr->data_name);
# endif	/* TRACE */
		if ( (ret_val = tgetstr(ptr->data_name, NULL)) != NULL )
		{
# ifdef	TRACE
			if ( Trace(1,5) )
				print("%s\n",ret_val);
# endif	/* TRACE */
			if ( Numcommands >= Sizecommands )
			{
				fprintf(stderr,"There are more commands then can be stored\n");
				return ((TERMINAL *) NULL);
			}
			commptr = &Commands[Numcommands++];
			if ( setcomm(commptr,ret_val,ptr->data_func) == ERR )
				return ((TERMINAL *) NULL);
			commptr->comm_name = ptr->data_name;
		}
# ifdef	TRACE
		else
			if ( Trace(1,5) )
				print("NULL\n");
# endif	/* TRACE */
		ptr++;
	}
	if ( Numcommands )
		qsort((char *)Commands,Numcommands, sizeof (COMM), compare);

	bools = Booleans;

	id[2] = '\0';
	while ( bools->bool_name != NULL )
	{
		id[0] = *bools->bool_name;
		id[1] = *(bools->bool_name + 1);
		*bools->bool_val = tgetflag(id);
		bools++;
	}

	columns = tgetnum("co") + 1;
	lines = tgetnum("li") + 2;	/* one extra for the status line */

	if ((term = term_malloc(columns, lines)) == (TERMINAL *) NULL)
	{
		return((TERMINAL *) NULL);
	}

	term_init(term,columns - 1,lines - 2,0,0);
	tabstops = tigetnum("it");
	if ( tabstops <= 0 )
		tabstops = MAXX + 1;
	for ( i = 1 ; i < MAXX ; i++ )
		if ( (i % tabstops) == 0 )
			term->term_tabs[i] = 1;
		else
			term->term_tabs[i] = 0;
# ifdef	TRACE
	if ( Trace(1,4) )
	{
		printf("init_terminfo: tabs set at");
		for ( i = 0 ; i < MAXX ; i++ )
			if ( term->term_tabs[i] )
				printf(" %d",i);
		printf("\n");
	}
# endif	/* TRACE */
	return (term);
}

TERMINAL *term_malloc(columns, lines)
int	columns;
int	lines;
{
	TERMINAL	*term;
	int		i;

	if ( (term = (TERMINAL *) calloc(1,sizeof (TERMINAL))) == (TERMINAL *) NULL )
	{
		fprintf(stderr,"Calloc of TERMINAL returned error\n");
		return ( (TERMINAL *) NULL );
	}
	term->term_flag = 0;
	if ( uses_sl() == OK )
		term_set(term,TERM_STATUS);
# ifdef	TRACE
	if ( Trace(1,0) )
		print("init_terminfo: columns (%d) lines (%d) STATUS = %s\n",
			columns,lines,(term_on(term,TERM_STATUS)?"TRUE":"FALSE"));
# endif	/* TRACE */
	term->term_bits = (long **) calloc(lines,sizeof (char *));
	if ( term->term_bits == (long **) NULL )
	{
		fprintf(stderr,"could not calloc term_bits field\n");
		return ( (TERMINAL *) NULL);
	}
	term->term_screen = (char **) calloc(lines,sizeof (char *));
	if ( term->term_screen == (char **) NULL )
	{
		fprintf(stderr,"could not calloc term_screen field\n");
		return ( (TERMINAL *) NULL);
	}
	for ( i = 0 ; i < lines ; i++ )
	{
		if ( (term->term_bits[i] = (long *) calloc(columns,sizeof (long))) == (long *) NULL )
		{
			fprintf(stderr,"Could not calloc term_bits[%d]\n",i);
			return ( (TERMINAL *) NULL);
		}
		if ( (term->term_screen[i] = calloc(columns,sizeof (char))) == (char *) NULL )
		{
			fprintf(stderr,"Could not calloc term_screen[%d]\n",i);
			return ( (TERMINAL *) NULL);
		}
		(void) memset(term->term_screen[i],' ',columns);
	}

	return(term);
}

/*
** setcomm
**	Given a string from terminfo, find the initial part
**	that is not parameterized, and set comm->string to that,
**	set comm->data to the rest.
**
** Return Value
**	OK	if everything went OK
**	ERR	on any error
**
** Arguments
**	comm	the COMMAND ptr to do magic with.
**	str	the string to decode.
**	func	the function for the command.
*/
setcomm(comm,str,func)
COMM	*comm;
char	*str;
int	(*func)();
{
	char	*string;
	char	*start;
	int	len;
	extern	char	*calloc();

	start = str;
	len = 0;

# ifdef	TRACE
	if ( Trace(1,10) )
		print("setcomm(%s)\n",str);
# endif	/* TRACE */
	while ( *str )
	{
		if ( ++len > Commlength )
			Commlength = len;

		if ( *str == '%' )
		{
			if ( str[1] == '%' )
				str++;
			else
				break;
		}
		str++;
	}
	len = abs(str - start);
	if ( len <= 0 )
		comm->comm_string = (char *) NULL;
	else
	{
		if ( (string = calloc(1,(len + 1) * sizeof (char))) == (char *) NULL )
			return (ERR);
		(void) strncpy(string,start,len);
		comm->comm_string = string;
	}
	if ( *str  == NULL )
		comm->comm_data = NULL;
	else
	{
		if ( (comm->comm_data = (char *) calloc(1,strlen(str) + 1)) == NULL )
		{
			fprintf(stderr,"setcomm: calloc of comm_data failed\n");
			return (ERR);
		}
		(void) strcpy(comm->comm_data,str);
	}
	comm->comm_func = func;
# ifdef	TRACE
	if ( Trace(1,10) )
		print("setcomm: comm->string (%s), comm->data (%s)\n",
			comm->comm_string,comm->comm_data);
# endif	/* TRACE */
	return (OK);
}

/*
** compare
**	Compare the comm_strings of two COMM structures.
**
** Arguments
**	comma	The first COMM structure.
**	commb	The second COMM structure.
**
** Returns
**	< 0 	if comma->comm_string < commb->comm_string
**	= 0 	if comma->comm_string = commb->comm_string
**	> 0 	if comma->comm_string > commb->comm_string
*/
int
compare(comma,commb)
COMM	*comma, *commb;
{
# ifdef	TRACE
	int	ret_val;
	char	buf[6];

	if ( Trace(2,0) )
	{
		print("compare(");
		strncpy(buf,comma->comm_string,5);
		print("%s,",buf);
		strncpy(buf,commb->comm_string,5);
		print("%s) ",buf);
	}
# endif	/* TRACE */
	if ( comma->comm_string == NULL && commb->comm_string == NULL )
	{
# ifdef	TRACE
		if ( Trace(2,0) )
			print("0\n");
# endif	/* TRACE */
		return (0);
	}

	if ( comma->comm_string == NULL )
	{
# ifdef	TRACE
		if ( Trace(2,0) )
			print("-1\n");
# endif	/* TRACE */
		return (-1);
	}

	if ( commb->comm_string == NULL )
	{
# ifdef	TRACE
		if ( Trace(2,0) )
			print("1\n");
# endif	/* TRACE */
		return (1);
	}

# ifdef	TRACE
	if ( Trace(2,0) )
	{
		ret_val = strcmp(comma->comm_string,commb->comm_string);
		print("%d\n",ret_val);
		return (ret_val);
	}
	else
# endif	/* TRACE */
		return (strcmp(comma->comm_string,commb->comm_string));
}

/*
** term_clear
**	Set a TERMINAL structure to all spaces.
**
** Arguments
**	term	A pointer to a TERMINAL structure.
*/
term_clear(term)
TERMINAL	*term;
{
	int	x,y;
	char	*str;
	long	*bits;

	if ( term == (TERMINAL *) NULL )
		return;

	y = term->term_height;

	while ( y-- )
	{
		x = term->term_width;
		str = term->term_screen[y];
		bits = term->term_bits[y];

		while ( x-- )
		{
			*str++ = ' ';
			*bits++ = 0;
		}
	}
	term->term_xloc = term->term_yloc = 0;
}

/*
** term_init
**	Initilize a TERMINAL structure.
**
** Arguments
**	term	A pointer to a TERMINAL structure.
**	width	The width of the window/terminal.
**	height	The height of the window/terminal.
**	x,y	Position on the screen that this window is at.
*/
term_init(term,width,height,x,y)
TERMINAL	*term;
int	width;
int	height;
int	x;
int	y;
{
	extern	char	*calloc();

# ifdef	TRACE
	if ( Trace(3,0) )
		printf("term_init(%x,%d,%d,%d,%d)\n",term,width,height,x,y);
# endif	/* TRACE */
	if ( term == (TERMINAL *) NULL )
		return;

	term->term_winx = x;
	term->term_winy = y;
	term->term_xloc = 0;
	term->term_yloc = 0;
	term->term_width = width;
	term->term_height = height;
	term->term_lmargin = 0;
	term->term_rmargin = width;
	term->term_onstatus = 0;
	term->touched = 0;
	term->term_flag = 0;
	term_clear(term);
}
