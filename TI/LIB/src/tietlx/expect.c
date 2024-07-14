# include "main.h"
# include "option.h"
# include "device.h"
# include "flag.h"
# include "collect.h"
# include "protocal.h"
# include "import.h"
# include "msg.h"
# include "execute.h"
# include <exec_head.h>
# include <catcher.h>
# include <fcntl.h>

SCCSID("@(#)expect.c	3.4 (SVID) 1/2/87");

int	ExpectCount = 0;

extern char	*getenv();
extern int	access();

FLAG_TYPE	step_in_time();
void		echo_collect();
void		clr_collect();
void		any_msg();
void		dump_error();

void xpk()
{
	FLAG_TYPE	flag_type;

	if (step_in_time(FLAG_PROCEED) != FLAG_ERROR)
	{
		if (SideType == SIDE_CATCHER)
		{
			echo_collect();

			ExpectCount++;

			if (Option.noisy)
			{
				(void) fprintf(stderr, "%4d", ExpectCount);
			}

			judge();

			any_msg();
		}
	}
	if (SideType == SIDE_CATCHER)
	{
		clr_collect();
	}
}

void xpk_flush()
{
	(void) step_in_time(FLAG_PROCEED);
	if (SideType == SIDE_CATCHER)
	{
		clr_collect();
	}
}

char xpk_final()
{
	char	result;

	if (SideType == SIDE_PITCHER)
	{
		screech("expect_final", 0, 
			"Calling from Pitcher is not allowed\n");
	}

	result = step_in_time((FLAG_PROCEED | FLAG_END));
	echo_collect();

	clr_collect();

	return(result);
}

void xpk_vidattr(attr)
int	attr;
{
	if (step_in_time(FLAG_PROCEED) != FLAG_ERROR)
	{
		if (SideType == SIDE_CATCHER)
		{
			echo_collect();
			test_vidattr(CollectBuff, CollectSize, attr);

			any_msg();
		}
	}
	if (SideType == SIDE_CATCHER)
	{
		clr_collect();
	}
}

void xpk_putp(str)
char	*str;
{
	if (step_in_time(FLAG_PROCEED) != FLAG_ERROR)
	{
		if (SideType == SIDE_CATCHER)
		{
			echo_collect();
			test_putp(CollectBuff, CollectSize, str);

			any_msg();
		}
	}
	if (SideType == SIDE_CATCHER)
	{
		clr_collect();
	}
}

void xpk_mvcur(oldy, oldx, newy, newx)
int	oldy;
int	oldx;
int	newy;
int	newx;
{
	if (step_in_time(FLAG_PROCEED) != FLAG_ERROR)
	{
		if (SideType == SIDE_CATCHER)
		{
			echo_collect();
			test_mvcur(CollectBuff, CollectSize, 
				   oldy, oldx, newy, newx);

			any_msg();
		}
	}
	if (SideType == SIDE_CATCHER)
	{
		clr_collect();
	}
}

FLAG_TYPE step_in_time(flag_set)
FLAG_TYPE	flag_set;
{
	FLAG_TYPE	result;
	FLAG_TYPE	bit_flag;

	switch (SideType)
	{
	  case SIDE_PITCHER:
		send_EOT();
		kickout();
		put_flag(FLAG_PROCEED);
		bit_flag = wait_flag(FLAG_PROCEED | FLAG_WARN | FLAG_ERROR | FLAG_END);
		switch (bit_flag)
		{
		  case FLAG_ERROR:
		  case FLAG_WARN:
			msg_pickup(bit_flag);
			screech("step_in_time", 0, 
				"Returned from msg_pickup()\n");
			break;

		  case FLAG_PROCEED:
		  case FLAG_END:
			break;

		  default:
			screech("step_in_time", 0, "Bad flag = %d\n", bit_flag);
		}
		break;

	  case SIDE_CATCHER:
		put_flag(FLAG_PROCEED);
		result = collect(flag_set | FLAG_ERROR);
		if (result == FLAG_ERROR)
		{
			JumpFinis = TRUE;
		}
		break;

	  default:
		screech("step_in_time", 0, "Bad SideType = %d\n", SideType);
		break;
	}

	return(result);
}

void echo_collect()
{
	if (Option.direct)
	{
		(void) write(1, CollectBuff, CollectSize);
		kickout();

		if (Option.respond)
		{
			get_keyboard();
		}
	}
}

void clr_collect()
{
	CollectSize = 0;
}

void any_msg()
{
	if (MsgTouched)
	{
		msg_close();

		if (Option.dump_error)
		{
			dump_error();
		}

		if (MsgWarning)
		{
			put_flag(FLAG_WARN);
		}
		else
		{
			put_flag(FLAG_ERROR);
		}

		(void) wait_flag(FLAG_ERROR);

		MsgTouched = FALSE;
		MsgWarning = FALSE;
		JumpFinis = TRUE;
	}
}

void dump_error()
{
	char	dump_name[BASIC_STR_LEN];
	int	term_id;
	char	*tmp_str;

	sprintf(dump_name, "%s/%s", wvs_t_path(), ExecHead.file_name);
	if (access(dump_name, 06) == 0)
	{
		tmp_str = getenv("TERM");
		term_id = tmp_str[strlen(tmp_str) - 1];

		sprintf(dump_name, "%s/E.%s/v1t%ce%d", 
			wvs_t_path(), ExecHead.file_name, term_id, ExpectCount);

		dumpscr(dump_name, BlessTerm);
	}
}
