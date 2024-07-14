# include "main.h"
# include <signal.h>

SCCSID("@(#)grab_sig.c	3.1 (SVID) 12/1/86");

struct sig_tbl {
	char *sig_name;
	int sig_id;
} sig_tbl[] = {
	"SIGQUIT",	SIGQUIT,
	"SIGILL",	SIGILL,
	"SIGTRAP",	SIGTRAP,
	"SIGFPE",	SIGFPE,
	"SIGSYS",	SIGSYS,

# ifdef SIGIOT
	"SIGIOT",	SIGIOT,
# endif /* SIGIOT */
# ifdef SIGEMT
	"SIGEMT",	SIGEMT,
# endif /* SIGEMT */
# ifdef SIGBUS
	"SIGBUS",	SIGBUS,
# endif /* SIGBUS */
# ifdef SIGSEGV
	"SIGSEGV",	SIGSEGV,
# endif /* SIGSEGV */
	NULL,		NULL
};

void grab_sig()
{
	int	sig_index;

	for (sig_index = 0; sig_tbl[sig_index].sig_name != NULL; sig_index++)
	{
		if (signal(sig_tbl[sig_index].sig_id, SIG_DFL) == SIG_ERR)
		{
			screech("re_catch", 1, 
				"Can't re-catch signal %s\n",
				sig_tbl[sig_index].sig_name);
		}
	}
}
