# include "main.h"
# include <exec_head.h>
# include <ctype.h>

SCCSID("@(#)switch.c	3.2 (SVID) 12/30/86");

void get_timeout();
char *skip_blank();

typedef	int	SWITCH_TYPE;

# define	SWITCH_NULL		0
# define	SWITCH_NOINIT		1
# define	SWITCH_NOINITSCR	2
# define	SWITCH_NOENDWIN		3
# define	SWITCH_NOXON		4
# define	SWITCH_TIMEOUT		5
# define	SWITCH_NOCHECK		6
# define	SWITCH_NOPROTOCAL	7

struct switch_tbl {
	char		*switch_name;
	SWITCH_TYPE	switch_code;
} switch_tbl[] = {
	"NOINIT",		SWITCH_NOINIT,
	"NOINITSCR",		SWITCH_NOINITSCR,
	"NOENDWIN",		SWITCH_NOENDWIN,
	"NOXON",		SWITCH_NOXON,
	"TIMEOUT",		SWITCH_TIMEOUT,
	"NOCHECK",		SWITCH_NOCHECK,
	"NOPROTOCAL",		SWITCH_NOPROTOCAL,
	NULL,			NULL
};

void compile_switch(comment)
char	*comment;
{
	struct switch_tbl	*seeker;
	SWITCH_TYPE		switch_code;
	char			*upper;
	int			offset = 'a' - 'A';

	comment = skip_blank(++comment);

	for (upper = comment; *upper != '\0'; upper++)
	{
		if (islower(*upper))
		{
			*upper = *upper - offset;
		}
	}

	switch_code = SWITCH_NULL;
	for (seeker = &switch_tbl[0]; seeker->switch_name != NULL; seeker++)
	{
		if (strncmp(comment, seeker->switch_name, strlen(seeker->switch_name)) == 0)
		{
			break;
		}
	}

	switch (seeker->switch_code)
	{
	  case SWITCH_NOINITSCR:
		ExecHead.do_initscr = FALSE;
		break;

	  case SWITCH_NOINIT:
		ExecHead.do_initscr = FALSE;
		ExecHead.do_endwin = FALSE;
		break;

	  case SWITCH_NOENDWIN:
		ExecHead.do_endwin = FALSE;
		break;

	  case SWITCH_NOXON:
		ExecHead.do_xon_xoff = FALSE;
		break;

	  case SWITCH_TIMEOUT:
		get_timeout(comment + strlen(seeker->switch_name));
		break;

	  case SWITCH_NOCHECK:
		ExecHead.do_check = FALSE;
		break;

	  case SWITCH_NOPROTOCAL:
		ExecHead.do_protocal = FALSE;
		break;

	  case SWITCH_NULL:
		break;

	  default:
		screech("compile_switch", 0, 
			"Bad switch_code = %d\n", switch_code);
	}
}

/*
**	"sscanf()" is used instead of "atoi()" for better error detection
*/

void get_timeout(arg)
char	*arg;
{
	char	sign_char;

	if (sscanf(arg, "%d", &(ExecHead.timeout_mod)) != 1)
	{
		screech("switch", 0, "Bad TIMEOUT value = \"%s\"\n", arg);
	}

	(void) sscanf(arg, "%1s", &sign_char);

	switch (sign_char) 
	{
	  case '+': 
	  case '-': 
		ExecHead.timeout_type = TIMEOUT_MOD;
		break;

	  default:
		ExecHead.timeout_type = TIMEOUT_SET;
	}
}

char *skip_blank(comment)
char	*comment;
{
	while ((*comment != '\0') && isspace(*comment))
	{
		comment++;
	}

	return(comment);
}
