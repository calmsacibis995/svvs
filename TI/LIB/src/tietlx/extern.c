/*
** SVVS Reporting
*/

# include <sv_base.h>

/*
** Include specifically for "extern.c"
*/

# include "main.h"

# include "option.h"
# include <exec_head.h>

SCCSID("@(#)extern.c	3.1 (SVID) 12/1/86");

/*
** Headers so "funct_tbl.h" knows what to reference
*/

# include <catcher.h>
# include "lib_pitch.h"
# include "lib_funct.h"
# include "config.h"
# include "device.h"
# include "expect.h"
# include "vs.h"
# include "comb.h"
# include "cursehead.h"

/*
** SVVS Reporting (cont.)
*/

# include <sv_env.h>
# include <svvs.h>
# include <sv_macro.h>

/*
** Global variables
*/

PRGM_TYPE	Prgm;
OPTION_TYPE	Option;
EXEC_HEAD_TYPE	ExecHead;
# include <funct_tbl.h>

SIDE_TYPE	SideType	= SIDE_NULL;
int		LineNumber	= 1;
int		PID_Other	= 0;

short		TIETL_trace[TRACENUM];
TRACE_MAP	TIETL_map[] = 
{
	"init",		5,
	"call",		10,
	"flag",		15,
	"vararg",	20,
	"protocal",	25,
	NULL,	0
};
