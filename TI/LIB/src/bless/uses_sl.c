# include	<bless.h>
# include	<common.h>

SCCSID("@(#)uses_sl.c	3.2 (SVVVS) 12/30/86");

/*
** uses_sl
**	Will CURSES use a status line?
**
** Returns
**	OK	If the status line is used.
**	ERR	Otherwise.
*/
uses_sl()
{
	extern	char	*tigetstr();

	if ( tigetstr("pln") == (char *) -1 || 
		(tigetnum("lh") * tigetnum("lw")) < 8 ||
		tigetnum("nlab") < 8 )
	{
# ifdef	TRACE
		if ( Trace(9,0) )
			printf("uses_sl returns ERR\n");
# endif	/* TRACE */
			return ( ERR );
	}

# ifdef	TRACE
	if ( Trace(9,0) )
		printf("uses_sl returns OK\n");
# endif	/* TRACE */
	return ( OK );
}
