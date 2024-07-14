/*
** SCCSID("@(#)parse.h	3.2 (SVID) 12/23/86");
*/

typedef	int	BOOLEAN;
typedef int	TBL_INDEX;

typedef int	OPERATOR_TYPE;

# define	OP_ASSIGN	101
# define	OP_ADDASSIGN	102
# define	OP_SUBASSIGN	103
# define	OP_MULTASSIGN	104
# define	OP_DIVASSIGN	105
# define	OP_MODASSIGN	106
# define	OP_ADD		107
# define	OP_SUB		108
# define	OP_MULT		109
# define	OP_DIV		110
# define	OP_MOD		111
# define	OP_CAT		112
# define	OP_AND		113
# define	OP_OR		114
# define	OP_BIT_AND	115
# define	OP_BIT_OR	116
# define	OP_EQ		117
# define	OP_NE		118
# define	OP_LT		119
# define	OP_LE		120
# define	OP_GT		121
# define	OP_GE		122
# define	OP_UMINUS	123
# define	OP_NOT		124
# define	OP_PREDECR	125
# define	OP_POSTDECR	126
# define	OP_PREINCR	127
# define	OP_POSTINCR	128
# define	OP_ARRAY	129

typedef	int	CLASS_TYPE;

# define	CLASS_NULL	11
# define	CLASS_CONST	12
# define	CLASS_VAR	13
# define	CLASS_TEMP	14

typedef	char	DATA_TYPE;

# define	DATA_NULL	'x'
# define	DATA_BOOL	'b'
# define	DATA_NUM	'n'
# define	DATA_FLOAT	'f'
# define	DATA_CHAR	'c'
# define	DATA_TEXT	't'
# define	DATA_WINDOW	'w'
# define	DATA_SCREEN	's'
# define	DATA_ARRAY	'a'
# define	DATA_TERM	'm'

typedef	int	EXPR_TYPE;

# define	EXPR_NULL	30
# define	EXPR_OP		31
# define	EXPR_VAR	32
# define	EXPR_CALL	33
# define	EXPR_END	34

typedef	int	STMT_TYPE;

# define	STMT_NULL	40
# define	STMT_COMPARE	41
# define	STMT_ASSIGN	42
# define	STMT_GOTO	43
# define	STMT_GOTO_ELSE	44
# define	STMT_LOOP	45
# define	STMT_CONTINUE	46
# define	STMT_CONT_LOOP	47
# define	STMT_BREAK	48
# define	STMT_EXIT	49
# define	STMT_START	50
# define	STMT_FINIS	51
# define	STMT_END	52

typedef int	SIDE_TYPE;

# define	SIDE_NULL	60
# define	SIDE_PITCHER	61
# define	SIDE_CATCHER	62
# define	SIDE_COMMENT	63

typedef int	TRACE_TYPE;

# define	TRACE_BLESS	70
# define	TRACE_TIETL	71

/*
** Header elements
*/

typedef struct
{
	TBL_INDEX	head_name;
	TBL_INDEX	head_expr;
	int		head_expr_count;
	TBL_INDEX	head_expr_next;
} HEAD_ENTRY;

/*
** Expression elements
*/

typedef struct
{
	EXPR_TYPE	expr_type;
	TBL_INDEX	expr_short_circuit;
	union
	{
		OPERATOR_TYPE	expr_op;
		TBL_INDEX	expr_var;
		TBL_INDEX	expr_call;
	} expr_data;
} EXPR_ENTRY;

/*
** Statement element definition
*/

typedef TBL_INDEX	ASSIGN_STMT_TYPE;

typedef struct
{
	TBL_INDEX	cmp_branch;
	TBL_INDEX	cmp_expr;
} COMPARE_STMT_TYPE;

typedef struct
{
	TBL_INDEX	loop_branch;
	TBL_INDEX	loop_expr;	/* Index to loop expression */
	TBL_INDEX	loop_for3;	/* Index to 3rd for-loop expression */
} LOOP_STMT_TYPE;

typedef TBL_INDEX	GOTO_STMT_TYPE;

typedef TBL_INDEX	HEAD_STMT_TYPE;

typedef TBL_INDEX	BLOCK_STMT_TYPE;

typedef struct
{
	STMT_TYPE	stmt_type;
	SIDE_TYPE	stmt_side_type;
	union
	{
		ASSIGN_STMT_TYPE	stmt_assign;
		COMPARE_STMT_TYPE	stmt_cmp;
		LOOP_STMT_TYPE		stmt_loop;
		GOTO_STMT_TYPE		stmt_goto;
		HEAD_STMT_TYPE		stmt_head;
		BLOCK_STMT_TYPE		stmt_block;
	} stmt_data;
} STMT_ENTRY;

/*
** Variable element definition
*/

typedef	int	NUMBER_TYPE;
# define	NUMBER_FMT	"%d"

typedef struct ARRAY_TYPE
{
	CLASS_TYPE	var_class;
	DATA_TYPE	var_type;
	union
	{
		TBL_INDEX	name_index;
		char		*name_ptr;
	} var_name;
	union 
	{
		NUMBER_TYPE	var_num;
		double		var_float;
		BOOLEAN		var_bool;
		TBL_INDEX	var_text_index;
		char*		var_text_ptr;
		chtype		var_chtype;	/* as defined in "curses.h" */
		char		var_char;	/* for "*scanw()" ONLY */
		WINDOW		*var_window;
		SCREEN		*var_screen;
		TERMINAL	*var_term;
		struct
		{
			struct ARRAY_TYPE	*array_var;
			int			array_size;
		} var_array;
	} var_data;
} VAR_ENTRY;

/*
** Call element definition
*/

typedef struct
{
	TBL_INDEX	call_funct_index;
	TBL_INDEX	call_expr;
	TBL_INDEX	call_expr_next;
	int		call_expr_count;
} CALL_ENTRY;

/*
** Name (symbol table) element definition
*/

typedef char	NAME_ENTRY;

/*
** Text (string) element definition
*/

typedef char	TEXT_ENTRY;

/*
** Table Statistics
*/

typedef	char	TBL_ID_TYPE;

# define	TBL_ID_EXPR	'e'
# define	TBL_ID_HEAD	'h'
# define	TBL_ID_VAR	'v'
# define	TBL_ID_CALL	'c'
# define	TBL_ID_NAME	'n'
# define	TBL_ID_STMT	's'
# define	TBL_ID_TEXT	't'

typedef struct
{
	int	elem_size;
	int	next_free;
	int	cur_size;
	int	grow_size;
	int	restart;
} TBL_STAT_TYPE;

typedef struct
{
	HEAD_ENTRY	*HeadTbl;
	EXPR_ENTRY	*ExprTbl;
	VAR_ENTRY	*VarTbl;
	CALL_ENTRY	*CallTbl;
	NAME_ENTRY	*NameTbl;
	STMT_ENTRY	*StmtTbl;
	TEXT_ENTRY	*TextTbl;

	TBL_STAT_TYPE	HeadStat;
	TBL_STAT_TYPE	ExprStat;
	TBL_STAT_TYPE	VarStat;
	TBL_STAT_TYPE	CallStat;
	TBL_STAT_TYPE	NameStat;
	TBL_STAT_TYPE	StmtStat;
	TBL_STAT_TYPE	TextStat;
} PRGM_TYPE;

/*
** GENERIC is used as a cast type for "tbl_alloc()".
** "tbl_alloc()" takes as one of its arguments a pointer to
** any one of the types in the union.  This way, if we are
** on a system which has different pointer sizes for different
** pointer types, we are assured compatibility.
*/

typedef union
{
	EXPR_ENTRY	*expr_ptr;
	HEAD_ENTRY	*head_ptr;
	VAR_ENTRY	*var_ptr;
	CALL_ENTRY	*call_ptr;
	NAME_ENTRY	*name_ptr;
	STMT_ENTRY	*stmt_ptr;
	TEXT_ENTRY	*text_ptr;
	char		*char_ptr;
} GENERIC;

/*
** Public variables
*/

extern PRGM_TYPE	Prgm;
extern char		*KB_Buffer;
extern BOOLEAN		FirstCall;
extern BOOLEAN		LastCall;

/*
** Macros to ease typing
*/

# define	text_addr(var_ptr)	(var_ptr)->var_data.var_text_ptr

/*
** Constants
*/

# define	NEXT_FREE_STMT	-1
# define	NIL		-1

