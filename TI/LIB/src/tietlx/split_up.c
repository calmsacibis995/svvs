# include "main.h"
# include "option.h"
# include "device.h"
# include "flag.h"
# include "pipe.h"
# include "expect.h"
# include "protocal.h"
# include "lib_pitch.h"
# include <signal.h>
# include <fcntl.h>

SCCSID("@(#)split_up.c	3.1 (SVID) 12/1/86");

void	set_new_name();
void	run_pitcher();
void	run_catcher();
void	set_pitcher();
void	set_catcher();

void split_up()
{
	make_pipe();

	if (Option.use_pipe)
	{
		make_named_pipe();
		Option.pitch_dev_name = Option.pipe_name;
		Option.catch_dev_name = Option.pipe_name;
	}

	PID_Other = fork();

	if (PID_Other > 0)
	{
		run_catcher();
	}
	else if (PID_Other == 0)
	{
		run_pitcher();
	}
	else
	{
		screech("split_up", 1, "fork()");
	}
}

void run_pitcher()
{
	SideType = SIDE_PITCHER;
	PID_Other = getppid();
	set_pipe();

	(void) signal(SIGUSR1, handler);

	(void) wait_flag(FLAG_PROCEED);

# ifdef TRACE
	if (Trace(5, 0))
	{
		fprintf(stderr, "Pitcher device = %s\n", Option.pitch_dev_name);
	}
# endif

	if (signal(SIGINT, SIG_IGN) == SIG_IGN)
	{
		set_pitcher();
	}
	else
	{
		(void) signal(SIGINT, SIG_IGN);
		set_pitcher();
		(void) signal(SIGINT, handler);
	}

	put_flag(FLAG_PROCEED);
	(void) wait_flag(FLAG_PROCEED | FLAG_END);

	if (Option.block_dump)
	{
		block_dump();
	}
	else
	{

# ifdef TRACE
		if (Trace(5, 0))
		{
			fprintf(stderr, "Execute Pitcher\n");
		}
# endif /* TRACE */

		(void) exec_block(0, 0);
	}

	send_EOT();
	kickout();

	if (DeferScrDump != NULL)
	{
		p_scrdump(DeferScrDump);
	}

	put_flag(FLAG_END);
	(void) wait_flag(FLAG_END);

	if (Option.show_var)
	{
		(void) fprintf(stderr, "PITCHER SYMBOL TABLE:\n");
		shw_v_table();
		put_flag(FLAG_SHOW);
		(void) wait_flag(FLAG_SHOW);
	}
}

void run_catcher()
{
	FLAG_TYPE	flag;

	SideType = SIDE_CATCHER;
	set_pipe();

	(void) signal(SIGUSR1, handler);

	if (signal(SIGINT, SIG_IGN) == SIG_IGN)
	{
		set_catcher();
	}
	else
	{
		(void) signal(SIGINT, SIG_IGN);
		set_catcher();
		(void) signal(SIGINT, handler);
	}

	put_flag(FLAG_PROCEED);
	(void) wait_flag(FLAG_PROCEED);

	if (Option.run_mimic)
	{

# ifdef TRACE
		if (Trace(5, 0))
		{
			fprintf(stderr, "Execute Catcher\n");
		}
# endif /* TRACE */

		(void) exec_block(0, 0);

		(void) xpk_final();
		put_flag(FLAG_END);
	}
	else
	{

# ifdef TRACE
		if (Trace(5, 0))
		{
			fprintf(stderr, "Execute Dummy Catcher\n");
		}
# endif /* TRACE */

		while ((flag = xpk_final()) != FLAG_END)
		{
			/* spin */
		}
		put_flag(flag);
	}

	if (!Option.respond)
	{
		(void) printf("\n");
	}

	if (Option.show_var)
	{
		(void) wait_flag(FLAG_SHOW);
		(void) fprintf(stderr, "CATCHER SYMBOL TABLE:\n");
		shw_v_table();
		put_flag(FLAG_SHOW);
	}

	wait((int *) 0);
}

void set_pitcher()
{
	(void) set_stdout();
	if (freopen(Option.pitch_dev_name, "w", stdout) == NULL)
	{
		screech("set_pitcher", 1,
			"freopen of '%s' failed\n", Option.pitch_dev_name);
	}
	openup(OPENUP_INPUT, Option.pitch_dev_name, OPENUP_OLD);
	openup(OPENUP_OUTPUT, Option.pitch_dev_name, OPENUP_NEW);
}

void set_catcher()
{

# ifdef TRACE
	if (Trace(5, 0))
	{
		fprintf(stderr, "Catcher device = %s\n", Option.catch_dev_name);
	}
# endif

	openup(OPENUP_INPUT, Option.catch_dev_name, OPENUP_NEW);
	openup(OPENUP_OUTPUT, Option.catch_dev_name, OPENUP_NEW);

	if (!Option.use_pipe)
	{
		no_device_delay(OPENUP_INPUT);
	}

	if (Option.respond)
	{

# ifdef TRACE
		if (Trace(5, 0))
		{
			fprintf(stderr, "BEGIN: set_keyboard\n");
		}
# endif

		set_keyboard();

# ifdef TRACE
		if (Trace(5, 0))
		{
			fprintf(stderr, "END: set_keyboard\n");
		}
# endif

	}
}
