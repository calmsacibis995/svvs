/*
** SCCSID("@(#)flag.h	3.2 (SVID) 12/23/86");
*/

typedef	int	FLAG_TYPE;

# define	FLAG_NULL	000
# define	FLAG_PROCEED	001
# define	FLAG_END	002
# define	FLAG_SHOW	004
# define	FLAG_ERROR	010
# define	FLAG_WARN	020

# define	CFLAG_NULL	' '
# define	CFLAG_PROCEED	'P'
# define	CFLAG_END	'E'
# define	CFLAG_SHOW	'S'
# define	CFLAG_ERROR	'X'
# define	CFLAG_WARN	'W'

extern void	make_pipe();
extern void	set_pipe();

extern void		put_flag();
extern FLAG_TYPE	take_flag();
extern FLAG_TYPE	wait_flag();
