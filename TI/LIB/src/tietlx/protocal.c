# include "main.h"
# include "flag.h"
# include "collect.h"
# include <exec_head.h>

SCCSID("@(#)protocal.c	3.1 (SVVS) 12/1/86");

# define	EOT_MARKER	"ATT - The Right Choice"
# define	EOT_LEN		22

static char	*EOT_Marker = EOT_MARKER;

BOOLEAN got_EOT()
{
	BOOLEAN	result = FALSE;
	char	*start;

	if (ExecHead.do_protocal)
	{

# ifdef TRACE
		if Trace(25, 0)
		{
			if (CollectSize >= EOT_LEN - 1)
			{
				start = &(CollectBuff[CollectSize - EOT_LEN]);
				CollectBuff[CollectSize] = '\0';
				cfprintf(stderr, "EOT = \"%s\"", start);
				fprintf(stderr, "\n");
			}
		}
# endif /* TRACE */

		if ((CollectSize >= EOT_LEN - 1) &&
		    (CollectBuff[CollectSize - 1] == EOT_Marker[EOT_LEN - 1]))
		{
			start = &(CollectBuff[CollectSize - EOT_LEN]);
			if (strncmp(start, EOT_Marker, EOT_LEN) == 0)
			{
				result = TRUE;
				CollectSize -= EOT_LEN;
			}
		}
	}
	else
	{
		result = TRUE;
	}

	return(result);
}

void send_EOT()
{
	if (ExecHead.do_protocal)
	{
		(void) printf(EOT_Marker);
	}
}
