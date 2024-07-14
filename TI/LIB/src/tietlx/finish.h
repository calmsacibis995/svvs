/*
** SCCSID("@(#)finish.h	3.3 (SVVS) 12/30/86")
*/

typedef	int	FINISH_TYPE;

# define	FINISH_NULL	0
# define	FINISH_BY_SELF	1
# define	FINISH_BY_OTHER	2

extern	FINISH_TYPE	Finish;
