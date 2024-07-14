# include "main.h"
# include <malloc.h>

SCCSID("@(#)c_delwin.c	3.1 (SVID) 12/1/86");

/*
** This auxiliary routine removes the given subwindow from its parent's
** list of children.
*/
rm_from_list(win)
TERMINAL *win;
{
	SUBWINDOWS *savedhead, *subhead;

	subhead = (win->container)->subws;

	if (subhead->subwin == win)
	{
		subhead = subhead->next;
	}
	else
	{
		while (subhead->subwin != win)
		{ 
			savedhead = subhead;
			subhead = subhead->next;
		}

		savedhead->next = subhead->next;
	}
}	 	
			
c_delwin(win) 
TERMINAL *win;
{
	SUBWINDOWS *head;
	int i;
	extern void free();

	head = win->subws;

	/*
	** First, we delete all the window's subwindows.
	*/

	while ((head != NULL) && (head->subwin != NULL)) 
	{
# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"deleting a subwindow\n");
		}
# endif	/* TRACE */
		(void) c_delwin(head->subwin);
		head->subwin = NULL;
		head = head->next;
	}

	if (win->subws != NULL)
	{
		win->subws->next = NULL;
	}

	/*
	** If the window is itself a subwindow, we just delete it from its 
	** parent's list and we're done, since a subwindow is a pointer into
	** its parent window and hence has no "screen" of its own.
	*/

	if (win->container != NULL)
	{
# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"removing subwindow from list\n");
		}
# endif	/* TRACE */
		rm_from_list(win);
		free((char *) win);
		return(OK);
	}

	/*
	** Otherwise, we free up the space used by the characters in the windows
	** and their attributes.
	*/

	for (i = 0; i < win->term_height; i++)
	{
		free((char *) win->term_screen[i]);
		free((char *) win->term_bits[i]);
		free((char *) win->term_lastrefresh[i]);
		free((char *) win->term_lastbits[i]);
	}

	free((char *) win->term_screen);
	free((char *) win->term_bits);
	free((char *) win->term_lastrefresh);
	free((char *) win->term_lastbits);
	free((char *) win);

	return(OK);
}
 
