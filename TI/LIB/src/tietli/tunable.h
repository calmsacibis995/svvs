/*
** SCCSID("@(#)tunable.h	3.1 (SVID) 12/1/86");
*/

# define	BASIC_STR_LEN		200

# define	HASH_TABLE_LEN		2000	/* Size of hash table 
						** for hsearch 
						*/

/*
** The following values are used to determine how many array
** elements are to be allocated as the need arises. Growth is 
** linear.
*/

/*
** These values are kept small for testing purposes.  They will be
** tuned later.
*/

# define	HEAD_INIT_SIZE	5
# define	EXPR_INIT_SIZE	500
# define	VAR_INIT_SIZE	200
# define	CALL_INIT_SIZE	50
# define	NAME_INIT_SIZE	2500
# define	STMT_INIT_SIZE	100
# define	TEXT_INIT_SIZE	300

# define	HEAD_GROW_SIZE	5
# define	EXPR_GROW_SIZE	5
# define	VAR_GROW_SIZE	5
# define	CALL_GROW_SIZE	5
# define	NAME_GROW_SIZE	5
# define	STMT_GROW_SIZE	5
# define	TEXT_GROW_SIZE	5
