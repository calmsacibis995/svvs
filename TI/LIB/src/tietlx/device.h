/*
** SCCSID("@(#)device.h	3.1 (SVID) 12/1/86");
*/

extern void	set_keyboard();
extern void	reset_keyboard();
extern void	get_keyboard();

extern void	openup();
extern void	set_port();
extern void	reset_port();
extern void	no_device_delay();

extern int	read_buff();
extern void	write_kb_buff();
extern void	keyboard();

extern void	kickout();

/*
** Types and modes for "openup()".  These allow us to use the same routine
** to reopen stdin (0) and stdout (1) as well as open new files descriptors
** for input and output
*/

typedef	int	OPENUP_FLOW;

# define	OPENUP_INPUT	0
# define	OPENUP_OUTPUT	1

typedef char	OPENUP_MODE;

# define	OPENUP_OLD	'O'
# define	OPENUP_NEW	'N'
