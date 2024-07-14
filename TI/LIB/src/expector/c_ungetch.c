# include "main.h"
# include <malloc.h>
# include <curses.h>

SCCSID("@(#)c_ungetch.c	3.1 (SVID) 12/1/86");
c_ungetch(cht) 
chtype cht;
{
	char *tmpstr;

/*
** This routine adds "ch" to the FRONT of the input stream.
*/

	if (TypeAhead)
	{
# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"ungetch: TypeAhead != NULL\n");
		}
# endif	/* TRACE */

		tmpstr = malloc ((unsigned) (strlen(TypeAhead) + 2));
		die(tmpstr,"c_ungetch:couldn't malloc tmpstr\n"); 

		TypeAhead = realloc(TypeAhead, (unsigned) (strlen(TypeAhead) + 2));
		die(TypeAhead,"c_ungetch: realloc of TypeAhead failed\n");
	}
	else
	{
# ifdef TRACE
		if (Dbg)
		{
			(void) fprintf(stderr,"ungetch: TypeAhead = NULL\n");
		}
# endif	/* TRACE */ 

		tmpstr = malloc ((unsigned) 2);
		die(tmpstr,"c_ungetch:couldn't malloc tmpstr\n"); 

		TypeAhead = malloc((unsigned) 2);
		die(TypeAhead,"c_ungetch: malloc of TypeAhead failed\n");

		*TypeAhead = '\0';

	}

	*tmpstr = (cht & A_CHARTEXT);

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"'%c' being added to head of input\n", *tmpstr);
	}
# endif	/* TRACE */

	(void) strcpy(tmpstr+1,TypeAhead);
	(void) strcpy(TypeAhead,tmpstr);
	free(tmpstr);
	
	return(OK);
}

