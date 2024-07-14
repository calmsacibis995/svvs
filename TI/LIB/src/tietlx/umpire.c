# include "main.h"
# include "flag.h"
# include "expect.h"
# include "msg.h"
# include "import.h"
# include <exec_head.h>
# include <catcher.h>

SCCSID("@(#)umpire.c	3.4 (SVID) 12/30/86");

extern char	*getenv();

# define	BITOF(x)	((x)&(TERM_REV|TERM_BLINK|TERM_BOLD|TERM_SO|TERM_UL|TERM_HALF))

void	chk_chars();
void	chk_flags();
BOOLEAN	diff_alt();
BOOLEAN	diff_chars();
BOOLEAN	loc_equal();
void	mk_alt_name();

void umpire(term_expect, term_got)
TERMINAL	*term_expect;
TERMINAL	*term_got;
{
	if (term_expect->term_flag != term_got->term_flag)
	{
		chk_flags(term_expect, term_got);
	}

	chk_chars(term_expect, term_got);
}

void chk_chars(term_expect, term_got)
TERMINAL	*term_expect;
TERMINAL	*term_got;
{
	if (!diff_chars(term_expect, term_got, TRUE))
	{
		return;
	}

	if ((!diff_alt(term_expect, term_got, 1)) ||
	    (!diff_alt(term_expect, term_got, 0)))
	{
		MsgWarning = TRUE;
	}
}

BOOLEAN diff_alt(term_expect, term_got, version)
TERMINAL	*term_expect;
TERMINAL	*term_got;
int		version;
{
	static TERMINAL	*term_alt = (TERMINAL *) NULL;
	char		restore_name[BASIC_STR_LEN];

	if (term_alt == (TERMINAL *) NULL)
	{
		term_alt = term_malloc(term_got->term_width, 
				       term_got->term_height);
		term_alt->term_width = term_got->term_width;
		term_alt->term_height = term_got->term_height;
	}

	mk_alt_name(restore_name, version);

	if (access(restore_name, 04) != 0)
	{
		return(TRUE);
	}

	restorescr(restore_name, term_alt, FALSE);

	if (!diff_chars(term_got, term_alt, FALSE))
	{
		return(FALSE);
	}

	return(TRUE);
}

BOOLEAN diff_chars(term_expect, term_got, log_diff)
TERMINAL	*term_expect;
TERMINAL	*term_got;
BOOLEAN		log_diff;
{
	int	x;
	int	y;
	int	offset;
	BOOLEAN	mis_match = FALSE;

	for (y = 0; y < term_got->term_height; y++)
	{
		offset = (y == term_got->term_height - 1) ? 1 : 0;

		for (x = 0; x < term_got->term_width - offset; x++)
		{
			if (loc_equal(term_expect, term_got, x, y))
			{
				continue;
			}

			mis_match = TRUE;

			if (log_diff)
			{
				printlines(y,
					   term_expect->term_screen[y],
					   term_expect->term_bits[y],
					   term_expect->term_width - offset,
					   term_got->term_screen[y],
					   term_got->term_bits[y],
					   term_got->term_width - offset);
			}

			break;
		}

		if (mis_match && !log_diff)
		{
			break;
		}
	}

	return(mis_match);
}

void mk_alt_name(restore_name, version)
char	*restore_name;
int	version;
{
	char	*tmp_str;
	char	term_id;

	tmp_str = getenv("TERM");
	term_id = tmp_str[strlen(tmp_str) - 1];
	sprintf(restore_name, "%s/E.%s/v%dt%ce%d", 
		wvs_d_path(), ExecHead.file_name, version, term_id, ExpectCount);
}

BOOLEAN loc_equal(term_expect, term_got, x, y)
TERMINAL	*term_expect;
TERMINAL	*term_got;
int		x;
int		y;
{
	if ((term_expect->term_screen[y][x] == term_got->term_screen[y][x]) &&
	    (BITOF(term_expect->term_bits[y][x]) == BITOF(term_got->term_bits[y][x])))
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

void chk_flags(term_expect, term_got)
TERMINAL	*term_expect;
TERMINAL	*term_got;
{
	if (term_on(term_expect,TERM_BELL) != term_on(term_got,TERM_BELL))
	{
		if term_on(term_expect,TERM_BELL)
		{
			msg_drop("An unexpected audible bell happened on the real screen.");
		}
		else
		{
			msg_drop("No audible bell occurred.");
		}
		term_clr(term_expect,TERM_BELL);
		term_clr(term_got,TERM_BELL);
	}

	if (term_on(term_expect,TERM_ALTCH) != term_on(term_got,TERM_ALTCH))
	{
		if term_on(term_expect,TERM_ALTCH)
		{
			msg_drop("The terminal is in the alternate character set.");
		}
		else
		{
			msg_drop("The terminal should be in the alternate character set, but is not.");
		}
	}

	if (term_on(term_expect,TERM_STALTCH) != term_on(term_got,TERM_STALTCH))
	{
		if term_on(term_expect,TERM_STALTCH)
		{
			msg_drop("The terminal is in the alternate character set.");
		}
		else
		{
			msg_drop("The terminal should be in the alternate character set, but is not.");
		}
	}
			
	if (term_on(term_expect,TERM_VB) != term_on(term_got,TERM_VB))
	{
		if term_on(term_expect,TERM_VB)
		{
			msg_drop("An unexpected visible bell happened on the real screen.");
		}
		else
		{
			msg_drop("No visible bell occured.");
		}
		term_clr(term_expect,TERM_VB);
		term_clr(term_got,TERM_VB);
	}
}
