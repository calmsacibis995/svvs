# include	<stdio.h>
# include	<sccs.h>
# include	<tie.h>
# include	<ctype.h>

SCCSID("@(#)trace.c	3.2 (SVVS) 12/30/86");

/*
** set_trace
**	Set the trace levels for a particular trace vector.
**	The syntax of the string is:
**		x		(set flag x to 1)
**		x=num		(set flag x to num)
**		x,y=num		(set flags x and y to num)
**		x-y=num		(set flags x through y to num)
**		x:y=num		(set flag x to 1, and flag y to num)
**
** Arguments
**	trace	The trace array.
**	value	A string of values to set it to.
**
** Return Value
**	0	If the trace levels could be set.
**	-1	On any error.
*/
set_trace(trace,value,map)
short		trace[];
char		*value;
TRACE_MAP	map[];
{
	char	*sp;
	int	begin, end;
	int	level;

	while ( *value )
	{
		/*
		** First determine the level.
		*/
		level = 1;
		sp = value;

		while (*sp && *sp != ':' && *sp != '=' )
			sp++;
		if ( *sp == '=' )
		{
			sp++;
			if ( !isdigit(*sp) )
				return (-1);
			level = atoi(sp);
		}

		while ( *value && *value != ':' && *value != '=' )
		{
			/*
			** Skip the separator character.
			*/
			if ( *value == ',' )
				value++;
			/*
			** Check for symbolic names.
			*/
			if ( !isdigit(*value) )
			{
				if ( (begin = trace_lookup(&value,map)) == -1 )
					return (-1);
			}
			else
			{
				begin = 0;
				while ( isdigit(*value) )
				{
					begin = (begin * 10) + (*value - '0');
					value++;
				}
			}
			/*
			** Check for ranges.
			*/
			if ( *value != '-' )
				end = begin;
			else
			{
				value++;
				/*
				** Check for symbolic names.
				*/
				if ( !isdigit(*value) )
				{
					if ( (end = trace_lookup(&value,map)) == -1 )
						return (-1);
					
				}
				else
				{
					end = 0;
					while ( isdigit(*value) )
					{
						end = (end * 10) + (*value - '0');
						value++;
					}
				}
			}
			if ( begin == 0 )
				end = TRACENUM - 1;
			if ( begin > end || begin >= TRACENUM || end >= TRACENUM )
				return (-1);
			for ( ; begin <= end ; begin++ )
				trace[begin] = level;
		}
		if ( *value == '=' )
		{
			value++;
			while (*value && isdigit(*value) )
				value++;
			if ( *value == ':' || *value == ',' )
				value++;
		}
		else
			if ( *value == ':' )
				value++;
	}
	return (0);
}

/*
** clr_trace
**	Clear all trace flags.
**
** Arguments
**	trace	The trace array.
*/
clr_trace(trace)
short	trace[];
{
	int	index;

	for ( index = 0 ; index < TRACENUM ; index++ )
		trace[index] = 0;
}

/*
** trace_list
**	List all the non-zero trace flags.
**
** Arguments
**	trace	Array of trace flags.
*/
trace_list(trace)
short	trace[];
{
	int	i;

	for ( i = 0 ; i < TRACENUM ; i++ )
		if ( trace[i] )
			printf("Trace[%d] = %d\n",i,trace[i]);
}

/*
** trace_lookup
**	Given a string, look it up in a trace_map for the associated
**	trace constant. Partial matches are made.
**
** Arguments
**	string	The string with the symbolic name (a double pointer so we
**		can skip it in the calling routine).
**	trace_map	The trace map.
**
** Return Value
**	the trace value if the name is found,
**	-1 on any error.
*/
trace_lookup(string,trace_map)
char	**string;
TRACE_MAP	trace_map[];
{
	TRACE_MAP	*map;
	int		len;
	char		*sp;
	char		*bp;
	char		buffer[50];

	sp = *string;
	bp = buffer;
	map = trace_map;

	while ( *sp && *sp != ',' && *sp !=':' && *sp != '=' )
		*bp++ = *sp++;

	if ( bp == buffer )
		return (-1);

	*bp = '\0';
	bp = buffer;
	len = strlen(bp);

	while ( map->trace_name != NULL )
	{
		if ( *bp == map->trace_name[0] )
		{
			if ( strncmp(bp,map->trace_name,len) == 0 )
			{
				*string = sp;
				return ( map->trace_value );
			}
		}
		map++;
	}
	return ( -1 );
}
