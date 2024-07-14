/*
** 12/1/86 (SVVS) @(#)tie.h	3.1
**	External definitions for all TIE programs.
*/

# define	TRACENUM	100	/* The number of possible trace values */

typedef	struct
{
	char	*trace_name;
	short	trace_value;
} TRACE_MAP;

/*
** External functions.
*/
extern	int	wbarber();		/* used to put barber poles on the screen */
extern	int	cprintf();	/* Like "fprintf()" except chars are printed 
				** in ^X format.  Limitation is that total
				** chars printed maynot exceed 1024.
				*/
