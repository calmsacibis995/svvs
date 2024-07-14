# include	<bless.h>

SCCSID("@(#)error.c	3.1 (SVID) 12/1/86");

/*
** errormsg
**	A catch all error function. This will call a more
**	appropriate error function for SVVS.
**
** Arguments
**	reason	A string with a reason for the error.
**	term	The current terminal description.
*/
void
errormsg(reason,term)
char		*reason;
TERMINAL	*term;
{
	printf("%s\nScreen looks like:\n",reason);
	printterm(term);
}
