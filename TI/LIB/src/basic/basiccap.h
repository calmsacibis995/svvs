/*
**	@(#)basiccap.h	3.1 (SVVS) 12/1/86
**
**	basiccap.h
**
**	contains definitions and things needed for the basic
**	capabilities testing of curses.
*/

# define TERMINFO	0	/* used in main to decide whether to do */
# define TERMCAP	1	/* tigetstr, etc. or tgetstr etc.	*/

# define MAXTNAMES	310

# define CAPSIZ		2048
# define NAMESIZE 	10

# define COMMENT	'#'
# define CAPCOMMENT	'.'
# define SEPARATE	','	/* Separates the capabilities. */
# define NAMESEPARATOR	'|'	/* Separates the names in the list. */
# define NUMBER		'#'
# define STRNG		'='
# define CANCEL		'@'

struct	cap 
{
	char	cap_name[NAMESIZE];
	char	cap_string[CAPSIZ];
	struct cap	*cap_next;
};
