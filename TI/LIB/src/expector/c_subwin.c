# include "main.h"

SCCSID("@(#)c_subwin.c	3.1 (SVID) 12/1/86");

TERMINAL *c_subwin(orig,nlines,ncols,begin_y,begin_x)
TERMINAL *orig;
int nlines, ncols, begin_y, begin_x;
{
	TERMINAL *term;
	extern TERMINAL *c_newwin();
	SUBWINDOWS *list, *savedlist;

	/*
	** Since a subwindow is just another window, we must allocate it.
	** However, since it contains no screen characters (it's just a 
	** pointer into the parent window), we just make its screen portion 1 x 1.
	*/

	term = c_newwin(1,1,begin_y,begin_x);

	/*
	** If this is not the first subwindow of the parent window, we must
	** add to an existing subwindow chain.
	*/

	if (orig->subws->subwin == NULL)
	{
# ifdef TRACE
		if (Dbg)
		{
			printf("adding the first subwindow of the current window\n");
		}
# endif	/* TRACE */

		orig->subws->subwin = term;
	}
	else
	{
# ifdef TRACE
		if (Dbg)
		{
			printf("finding room for a co-subwindow\n");
		}
# endif	/* TRACE */
		list = orig->subws;
		while (list != NULL) 
		{
			savedlist = list;	
			list = list->next;
		}
		
		savedlist->next = (SUBWINDOWS *) malloc(sizeof(SUBWINDOWS));
		die(savedlist->next,"c_subwin: malloc of savedlist->next failed\n");
		
# ifdef TRACE
		if (Dbg)
		{
		printf("adding the co-subwindow to the chain\n");	
		}
# endif /* TRACE */

		savedlist = savedlist->next;
		savedlist->subwin = term;
		savedlist->next = NULL;
	}
	term->term_width = ncols;
	term->term_height = nlines;
	term->container = orig;
	return(term);
}

