# include "main.h"
# include "option.h"
# include "device.h"
# include "flag.h"
# include "import.h"
# include "protocal.h"
# include <exec_head.h>
# include <fcntl.h>

SCCSID("@(#)collect.c	3.3 (SVID) 12/30/86");

TERMINAL	*BlessTerm;
char		*CollectBuff = NULL;
unsigned	CollectSize  = 0;
unsigned	CollectMax   = 0;

void init_BlessTerm()
{

# ifdef TRACE
	if Trace(5, 0)
	{
		(void) fprintf(stderr, "init_terminfo(%s)\n", Option.term_type);
	}
# endif /* TRACE */

	BlessTerm = init_terminfo(Option.term_type);
	if ( BlessTerm == (TERMINAL *) NULL )
	{
		screech("init_terminfo", 0, "init_terminfo failed\n");
	}
}

FLAG_TYPE collect(flag_set)
FLAG_TYPE	flag_set;
{
	FLAG_TYPE	flag = FLAG_NULL;
	BOOLEAN		got_flag = FALSE;

	unsigned	read_max;
	int		read_count;
	char		*tmp_buff;
	int		rounds = 0;

	int		idle = 0;
	int		backup = 0;

	if (!CollectBuff)
	{
		CollectBuff = malloc(COLLECT_INIT_SIZE);
		CollectMax = COLLECT_INIT_SIZE;
		CollectSize = 0;
	}

	do
	{
/*
**		if (idle++ > 200)
**		{
**			backup = CollectSize - 50;
**			if (backup < 0)
**			{
**				backup = 0;
**			}
**
**			CollectBuff[CollectSize] = '\0';
**			cfprintf(stderr, "CollectBuff[%d] = %s", 
**				 backup, &(CollectBuff[backup]));
**			fprintf(stderr, "\n");
**		}
*/

		read_max = CollectMax - CollectSize;
		read_count = read_buff(&((CollectBuff)[CollectSize]), read_max);

		if (read_count > 0)
		{
			rounds = 0;

			CollectSize += read_count;

			if ( CollectSize >= (CollectMax * 3 / 4) )
			{
				CollectMax += COLLECT_GROW_SIZE;
				tmp_buff = realloc(CollectBuff, CollectMax);
				CollectBuff = tmp_buff;
			}
		}

		if ((read_count <= 0) || ((Option.MIN) && (Option.TIME)))
		{
			if (Option.bell)
			{
				(void) fprintf(stderr, "");
			}

			if (!got_flag)
			{
				got_flag = ((flag = take_flag()) != FLAG_NULL);
				if ((got_flag) && (!(flag_set & flag)))
				{
					screech("collect", 0,
						"Received unexpected flag = %d\n",
						flag);
				}
			}

			chk_partner(&rounds);

			if ((Option.use_pipe) && (Option.TIME))
			{
				(void) sleep(Option.TIME);
			}
		}
	} while ((!got_flag) || ((got_flag) && (!got_EOT())));

	return(flag);
}

void judge()
{
	if (Option.run_bless)
	{
		if (CollectSize)
		{
			bless(BlessTerm, CollectBuff, CollectSize);
		}

		if (Option.output_bless)
		{
			(void) printf("\nBLESS:\n");
			printterm(BlessTerm);

			if (Option.respond)
			{
				get_keyboard();
				(void) printf("\n");
			}
		}
	}


	if (Option.run_mimic)
	{
		if (Option.output_mimic)
		{
			(void) printf("\nMIMIC:\n");
			printterm(DISPLAY);

			if (Option.respond)
			{
				get_keyboard();
				(void) printf("\n");
			}
		}

	}

	if ((Option.run_mimic) && 
	    (Option.run_bless) && 
	    (Option.run_umpire) &&
	    (Option.output_umpire))
	{
		if ((Option.output_mimic || Option.output_bless) &&
		    Option.output_umpire)
		{
			(void) printf("\n");
		}

		umpire(DISPLAY, BlessTerm);

		if (Option.respond)
		{
			get_keyboard();
			(void) printf("\n");
		}
	}
}
