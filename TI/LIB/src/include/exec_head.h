/*
** SCCSID("@(#)exec_head.h	3.2 (SVID) 12/23/86");
*/

/*
** Constants
*/

# define	MAX_FILE_NAME_LEN	14

/*
** Default Values
*/

# define	TIMEOUT_MOD		1
# define	TIMEOUT_SET		2
# define	TIMEOUT_NULL		3

# define	DEFAULT_DO_INITSCR	TRUE
# define	DEFAULT_DO_ENDWIN	TRUE
# define	DEFAULT_DO_XON_XOFF	TRUE
# define	DEFAULT_DO_CHECK	TRUE
# define	DEFAULT_DO_PROTOCAL	TRUE

# define	DEFAULT_TIMEOUT_MOD	0
# define	DEFAULT_TIMEOUT_TYPE	TIMEOUT_NULL

/*
** Execution Header
*/

typedef struct
{
	BOOLEAN		do_initscr;
	BOOLEAN		do_endwin;
	BOOLEAN		do_xon_xoff;
	BOOLEAN		do_check;
	BOOLEAN		do_protocal;

	int		timeout_mod;
	int		timeout_type;
	
	TBL_INDEX	index_stdscr;
	TBL_INDEX	index_curscr;
	TBL_INDEX	index_LINES;
	TBL_INDEX	index_COLS;
	TBL_INDEX	index_SideType;

	char		file_name[MAX_FILE_NAME_LEN];
} EXEC_HEAD_TYPE;

extern EXEC_HEAD_TYPE	ExecHead;
