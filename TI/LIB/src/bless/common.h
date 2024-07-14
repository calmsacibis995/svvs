/*
** @(#)common.h	3.1 (SVID) 12/1/86
**
** Structures, and external definitions for the reverse
** curses program "bless". These are specific to Bless itself.
*/

/*
** Trace macro. This is specific to bless, as
** the trace array is mentioned.
*/
# define	Trace(flg,level)	( (Bless_trace[flg] > level) )


/*
** Command structure. This is how terminal commands
** are stored for quick lookup. This is stored as
** a tree.
*/
struct	_comm
{
	int	(*comm_func)();		/* Function to call */
	char	*comm_data;		/* The parameritized data  */
	char	*comm_string;		/* The command string */
	char	*comm_name;		/* The name of the terminfo capability (for debugging purposes) */
};

typedef	struct _comm	COMM;


/*
** This is a list of all known terminal
** strings, and their "type".
*/
struct	_termdata
{
	char	*data_name;	/* name */
	int	(*data_func)();	/* status flag */
};

/*
** Boolean list.
** This is for the list of all boolean functions.
*/
struct	_bools
{
	char	*bool_name;	/* name of the boolean */
	char	*bool_val;	/* A pointer to the boolean variable */
};

/*
** Argument list.
** This is for the return value from apply. From any function,
** any number of "objects" can be returned from the parameritized
** strings this allows anything to come back.
*/
struct	_args
{
	short		arg_type;		/* argument type */
	char		arg_char;
	char		*arg_str;
	int		arg_int;
	struct	_args	*arg_next;
};

typedef	struct	_args	ARGS;

# define	ARG_NADA	00		/* This arg undefined */
# define	ARG_CHAR	01
# define	ARG_STR		02
# define	ARG_INT		03

# define	MAX(a,b)	((a) > (b) ? (a) : (b))
# define	MIN(a,b)	((a) < (b) ? (a) : (b))

# define	NUMATTS		6

# include	<extern.h>
