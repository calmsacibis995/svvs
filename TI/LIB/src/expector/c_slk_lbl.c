# include "main.h"

SCCSID("@(#)c_slk_lbl.c	3.1 (SVID) 12/1/86");

char *c_slklabel(labnum)
int labnum;
{
	char *initial_str, *returned_str, *saved_str;

	initial_str = labeltbl[labnum].text;

# ifdef TRACE
if (Dbg)
{
	(void) fprintf(stderr,"initial_str = '%s'\n",initial_str);
}
# endif	/* TRACE */

/*
** Strip leading blanks from label.
*/

	while (*initial_str == ' ')
	{
		initial_str++;
	}

	saved_str = initial_str;

/*
** Strip trailing blanks from label.
*/

	while ((*saved_str != ' ') && (*saved_str != '\0'))
	{
		saved_str++;
	}

	*saved_str = '\0';
	
/*
** Truncate label (if necessary) to width of label field
*/

	returned_str = malloc ((unsigned) Labelwidth + 2);
	die(returned_str,"c_slk_label: malloc of returned_str failed\n");

	(void) strncpy(returned_str,initial_str,Labelwidth);
	returned_str[Labelwidth] = '\0';

# ifdef TRACE
	if (Dbg)
	{
		(void) fprintf(stderr,"returned_str = '%s'\n",returned_str);
	}
# endif	/* TRACE */

	return(returned_str);
}


