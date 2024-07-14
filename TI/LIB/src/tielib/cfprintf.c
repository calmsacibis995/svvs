# include <sccs.h>
# include <curses.h>
# include <varargs.h>

SCCSID("@(#)cfprintf.c	3.1 (SVID) 12/1/86");

# define	MAXBUFF	1000

/*
** Like "fprintf()" except that all characters are sent through
** "unctrl()" to convert all control characters to "^X" format.
**
** Biggest drawback is that the total length of the formatted output 
** of "cfprintf()" cannot exceed MAXBUFF.
*/

int cfprintf(va_alist)
va_dcl
{
	va_list	args;
	FILE	*fp;
	char	*fmt;
	char	old_buff[MAXBUFF];
	char	new_buff[MAXBUFF * 2];	/* "old_buff" could be all ctrl chars */
	int	buff_index;
	int	buff_len;
	int	result;

/*
** Pop off variable arguments. Send the rest to "vsprintf()"
*/

	va_start(args);

	fp = va_arg(args, FILE *);
	fmt = va_arg(args, char *);
	result = vsprintf(old_buff, fmt, args);

	va_end(args);

/*
** Go through each character and send it through "unctrl()". Append
** result to "new_buff()".  Print "new_buff()" when done.
*/

	new_buff[0] = '\0';
	buff_len = strlen(old_buff);
	for (buff_index = 0; buff_index < buff_len; buff_index++)
	{
		strcat(new_buff, unctrl(old_buff[buff_index]));
	}

	fprintf(fp, "%s", new_buff);

	return(result);
}
