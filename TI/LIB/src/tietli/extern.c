# include "main.h"
# include "option.h"
# include <exec_head.h>

SCCSID("@(#)extern.c	3.1 (SVID) 12/1/86");

PRGM_TYPE	Prgm;
OPTION_TYPE	Option;
EXEC_HEAD_TYPE	ExecHead;

# include "funct.h"

SIDE_TYPE	SideType	= SIDE_NULL;
int		LineNumber	= 1;
