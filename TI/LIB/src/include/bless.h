/*
** @(#)bless.h	3.1 (SVID) 12/1/86
**
** Structures, and external definitions for the reverse
** curses program "bless".
*/
 
# include	<curses.h>
# include	<unctrl.h>
# include	<sccs.h>

# define	MAXX	200
# define	MAXY	50
/*
** Terminal screen structure. This is what the physical
** screen should look like.
*/
typedef struct	_term
{
	long	term_flag;			/* Status flags (see below) */
	int	term_width;			/* width of the screen - 1 */
	int	term_height;			/* height of the screen - 1 */
	int	term_xloc;			/* current X location */
	int	term_yloc;			/* current Y location */
	char	**term_screen;			/* the characters on the screen */
	long	**term_bits;			/* the information about each character */
	char	**term_lastrefresh;		/* window as of last wrefresh  */
	long	**term_lastbits;			/* the information about each character as of last refresh */
	int	term_savx;			/* saved X position */
	int	term_savy;			/* saved Y position */
	int	term_winx;			/* location on the screen of this window/terminal */
	int	term_winy;			/* location on the screen of this window/terminal */
	char	term_tabs[MAXX];		/* list of all tabs */
	int	term_onstatus;			/* on status line? */
	int	term_lmargin;			/* left margin */
	int 	term_rmargin;			/* right margin */
	int 	scrl_top;			/* top of scrolling region */
	int	scrl_bot;			/* bottom of scrolling region */
	int	deleted;			/* has window been deleted? */ 
	int	touched;			/* TRUE = no optimization */
	int	keypad_on;			/* are keypads enabled? */
	int	is_a_pad;			/* Is the window a pad? */
	int	last_pminrow;			/* arguments to last prefresh, */
	int 	last_pmincol;			/* saved for use in pechochar */
	int	last_sminrow;
	int	last_smincol;
	int	last_smaxrow;
	int	last_smaxcol;
	int	pminrow;			/* for use in prefresh */
	int	pmincol;
	int	sminrow;
	int	smincol;
	int	smaxrow;
	int	smaxcol;

	struct _term *container;		/* if we're in a subwindow */
	struct subwlist
	{
		struct _term *subwin;			/* a sub-window of the current window */	
		struct subwlist *next;			/* the next sub-window of the current window */
	};
	struct subwlist  *subws;			/* list of sub-windows */
} TERMINAL;

typedef struct subwlist SUBWINDOWS;

/*
** Structure for soft label keys.
*/
struct label_entry 
{
	int pos,fmt,disp;
	char *text;
} labeltbl[9];

extern	TERMINAL *DISPLAY, *std;
extern	TERMINAL *status_line;
extern	int	c_LINES, c_COLS;
extern	int 	Status_line_initialized;
extern	int	Echo_on;
extern	int	Soft_label_keys_on;
extern  int	Keypad_on;
extern  long 	Default_attr;
extern  int	Labelwidth;

# define	TERM_BELL	01
# define	TERM_REV	02
# define	TERM_HALF	04
# define	TERM_TABS	010
# define	TERM_BLINK	020
# define	TERM_INSERT	040
# define	TERM_CURINV	0100
# define	TERM_CURSO	0200
# define	TERM_ALTCH	0400
# define	TERM_STALTCH	01000
# define	TERM_BOLD	02000
# define	TERM_CMINIT	04000
# define	TERM_SO		010000
# define	TERM_UL		020000
# define	TERM_VB		040000
# define	TERM_INIT	0100000
# define	TERM_KBT	0200000
# define	TERM_SUBWIN	0400000

# define	WIN_CLROK	01000000
# define	WIN_LVOK	02000000
# define	WIN_SCRLOK	04000000

# define	TERM_INVIS	010000000
# define	TERM_STATUS	020000000
# define	TERM_PROT	040000000

# define	TERM_INIT1	0100000000
# define	TERM_INIT2	0200000000
# define	TERM_INIT3	0400000000

# define	TERM_NULL	01000000000

# define	term_set(term,bit)	(term)->term_flag |= (bit)
# define	term_clr(term,bit)	(term)->term_flag &= ~(bit)
# define	term_on(term,bit)	((term)->term_flag & (bit))
# define	term_off(term,bit)	(!((term)->term_flag & (bit)))

# define	t_bit_set(term,y,x,bit)	(term)->term_bits[y][x] |= (bit)
# define	t_bit_clr(term,y,x,bit)	(term)->term_bits[y][x] &= ~(bit)
# define	t_bit_on(term,y,x,bit)	((term)->term_bits[y][x] & (bit))
# define	t_bit_off(term,y,x,bit)	(!((term)->term_bits[y][x] & (bit)))

# define	TABLEN		8	/* default tab stop is 8 spaces */
