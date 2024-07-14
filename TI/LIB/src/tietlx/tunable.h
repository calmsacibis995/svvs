/*
** SCCSID("@(#)tunable.h	3.2 (SVID) 12/30/86");
*/

# define	BASIC_STR_LEN		200
# define	STACK_LEN		150

# define	HASH_TABLE_LEN		2000	/* Size of hash table 
						** for hsearch 
						*/

# define	EXAM_ROUNDS		5	/* Wait for timeout to expire
						** this many times before
						** checking for dead pitcher
						*/

# define	CHK_TEST_LEN		1	/* Wait this many seconds for
						** the "wait()" to complete
						** before pronouncing pitcher
						** process has not been killed
						*/

# define	TERM_ROOT		"SVVS"	/* Root name of all terminals
						** used with test
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

# define	HEAD_INIT_SIZE		5
# define	EXPR_INIT_SIZE		500
# define	VAR_INIT_SIZE		200
# define	CALL_INIT_SIZE		50
# define	NAME_INIT_SIZE		2500
# define	STMT_INIT_SIZE		100
# define	TEXT_INIT_SIZE		300

# define	HEAD_GROW_SIZE		5
# define	EXPR_GROW_SIZE		5
# define	VAR_GROW_SIZE		5
# define	CALL_GROW_SIZE		5
# define	NAME_GROW_SIZE		5
# define	STMT_GROW_SIZE		5
# define	TEXT_GROW_SIZE		5

# define	COLLECT_INIT_SIZE	1024
# define	COLLECT_GROW_SIZE	1024
