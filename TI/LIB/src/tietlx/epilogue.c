# include "main.h"
# include "device.h"
# include "option.h"
# include "pipe.h"
# include "finish.h"
# include "lib_pitch.h"
# include <varargs.h>
# include <signal.h>

SCCSID("@(#)epilogue.c	3.3 (SVID) 12/30/86");

void epilogue();
void kill_other();
void wait_pitcher();

/*
** "screech()" takes the following arguments:
**
**	char *		Usually used for routine name
**	BOOLEAN		Should "perror()" be called?
**	char *		Format string for "fprintf()"
**	arg1..		Arguments for "fprintf()"
*/

/*VARARGS*/

void screech(va_alist)
va_dcl
{
	va_list	args;
	char	*fmt;
	char	*name;
	BOOLEAN	do_perror;
	static BOOLEAN	already_called = FALSE;

	va_start(args);

	name = 		va_arg(args, char *);
	do_perror = 	va_arg(args, BOOLEAN);
	fmt = 		va_arg(args, char *);

	if ((name) || (fmt))
	{
		(void) fprintf(stderr, "tietlx");

		if (SideType == SIDE_PITCHER)
		{
			(void) fprintf(stderr, "(P)");
		}
		else
		{
			(void) fprintf(stderr, "(C)");
		}

		if (name)
		{
			(void) fprintf(stderr, ", %s", name);
		}

		if (fmt)
		{
			(void) fprintf(stderr, ": ");
			(void) vfprintf(stderr, fmt, args);
			if (fmt[strlen(fmt) - 1] != '\n')
			{
				(void) fprintf(stderr, "\n");
			}
		}
		else
		{
			(void) fprintf(stderr, "\n");
		}
	}

	if (do_perror)
	{
		perror("tietlx");
	}

	va_end(args);

	if (!already_called)
	{
		already_called = TRUE;
		epilogue();

		if (Finish == FINISH_BY_OTHER)
		{
			kill_other();
		}

		vs_done();
	}
}

void epilogue()
{
	static BOOLEAN	already_called = FALSE;

	if (!already_called)
	{
		already_called = TRUE;

		/*
		** Call "reset_keyboard()" regardless of the value of 
		** "Option.respond". The option may have been turned off 
		** during runtime and the terminal may still need to be reset.
		*/

		reset_keyboard();

		if (!Option.use_pipe)
		{
			reset_port(OPENUP_INPUT);
			reset_port(OPENUP_OUTPUT);
		}
		else
		{
			if (SideType == SIDE_CATCHER)
			{
				rm_named_pipe();
			}
		}
	}
}

void kill_other()
{
	if (SideType == SIDE_PITCHER)
	{
		if (kill(PID_Other, SIGUSR1))
		{
			screech("kill_other", 0, "Can't kill catcher\n");
		}
	}
	else if (SideType == SIDE_CATCHER)
	{
		if (kill(PID_Other, SIGUSR1))
		{
			screech("kill_other", 0, "Can't kill pitcher\n");
		}
		else
		{
			wait_pitcher();
		}
	}
	else
	{
		/*
		** "SideType" has not been assigned yet.  Therefore
		** "fork()" has not taken place and there is nothing
		** to kill.
		*/
	}
}

void handler(sig_caught)
int	sig_caught;
{
	(void) signal(SIGALRM, SIG_IGN);
	(void) signal(sig_caught, SIG_IGN);

	if (SideType == SIDE_PITCHER)
	{
		if (InitTIE)
		{
			p_endwin();
		}
	}

	epilogue();

	vs_done();
}

void wait_pitcher()
{
	if (SideType == SIDE_CATCHER)
	{
		if (wait((int *) 0) == -1)
		{
			screech("wait_pitcher", 0, "Can't wait for catcher\n");
		}
	}
}
