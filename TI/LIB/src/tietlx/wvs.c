/*
** SVVS Reporting
*/

# include <sv_base.h>

/*
** Includes specifically for "wvs.c"
*/

# include "main.h"
# include "option.h"
# include "execute.h"
# include <exec_head.h>
# include "flag.h"
# include "msg.h"

SCCSID("@(#)wvs.c	3.2 (SVID) 12/23/86");

/*
** SVVS Reporting
*/

# include <sv_env.h>
# include <svvs.h>
# include <sv_macro.h>

/*
** Define the DEFAULT_PITCH_DEV and DEFAULT_CATCH_DEV macros if the
** macros haven't been defined in "sv_conf.h".  This only happens when
** linking TIETL with an old  "sv_conf.h".
*/

# ifndef DEFAULT_PITCH_DEV
# define DEFAULT_PITCH_DEV	"/dev/tty31"
# endif /* DEFAULT_PITCH_DEV */

# ifndef DEFAULT_CATCH_DEV
# define DEFAULT_CATCH_DEV	"/dev/tty32"
# endif /* DEFAULT_CATCH_DEV */

extern char	*getenv();

void set_svvs_option()
{
	if (Option.pitch_dev_name == NULL)
	{
		Option.pitch_dev_name = DEFAULT_PITCH_DEV;
	}

	if (Option.catch_dev_name == NULL)
	{
		Option.catch_dev_name = DEFAULT_CATCH_DEV;
	}
}

wvs_setup(name)
char	*name;
{
	setup(name);
	break_on(ERROR);
	break_on(WARNING);
}

wvs_done()
{
	done();
}

wvs_expecting()
{
	expecting(SUCCESS);
}

wvs_no_expecting()
{
	expecting(VOID);
}

wvs_abstract(fmt)
char	*fmt;
{
	abstract(fmt);
}

wvs_testing(fmt)
char	*fmt;
{
	testing(fmt);
}

wvs_prep(fmt)
char	*fmt;
{
	prep(fmt);
}

wvs_error(fmt)
char	*fmt;
{
	error(fmt);
}

wvs_warning(fmt)
char	*fmt;
{
	warning(fmt);
}

wvs_message(fmt)
char	*fmt;
{
	message(fmt);
}

wvs_cleanup(fmt)
char	*fmt;
{
	cleanup(fmt);
}

STMT_TYPE exec_block(level, stmt_index)
int	level;
int	stmt_index;
{
	STMT_TYPE	result;
	BOOLEAN		graceful;

	switch (SideType)
	{
	  case SIDE_CATCHER:
		result = execute(level, stmt_index);
		switch (result)
		{
		  case STMT_FINIS:
			JumpFinis = FALSE;
			result = STMT_NULL;
			break;

		  case STMT_EXIT:
		  case STMT_NULL:
			break;

		  default:
			screech("exec_block", 0, 
				"Bad execute return value = %d\n", result);
		}
		break;

	  case SIDE_PITCHER:
		if (level == 0)
		{
			abstract("Executing test %s with terminal type %s",
				 ExecHead.file_name, getenv("TERM"));
		}
		start
			break_on(ERROR);
			break_on(WARNING);
			graceful = FALSE;
			result = execute(level, stmt_index);
			graceful = TRUE;

			switch (result)
			{
			  case STMT_FINIS:
				result = STMT_NULL;
				break;

			  case STMT_EXIT:
			  case STMT_NULL:
				break;

			  default:
				screech("exec_block", 0, 
					"Bad execute return value = %d\n", 
					result);
			}
		finis

		if ( graceful == FALSE )
		{
			expecting(VOID);

			if (UmpireError)
			{
				UmpireError = FALSE;
			}
			else
			{
				send_EOT();
				kickout();
			}

			put_flag(FLAG_ERROR);
			(void) wait_flag(FLAG_PROCEED | FLAG_END);
			result = STMT_NULL;
		}

		break;

	  default:
		screech("exec_block", 0, "Bad SideType = %d\n", SideType);
	}

	return(result);
}

FILE *wvs_t_fopen(sub_path, type)
char	*sub_path;
char	*type;
{
	FILE	*result;
	extern	FILE	*temp_fopen();

	expecting(SUCCESS);
	result = temp_fopen(sub_path, type);
	expecting(VOID);

	return(result);
}

char *wvs_t_path()
{
	return(temp_path());
}

FILE *wvs_d_fopen(sub_path, type)
char	*sub_path;
char	*type;
{
	FILE		*result;
	extern	FILE	*data_fopen();

	expecting(SUCCESS);
	result = data_fopen(sub_path, type);
	expecting(VOID);

	return(result);
}

char *wvs_d_path()
{
	return(data_path());
}

char *wvs_advisory()
{
	return(ADVISORY);
}

int wvs_msg_type(flag_type)
FLAG_TYPE	flag_type;
{
	int	result;

	switch (flag_type)
	{
	  case FLAG_WARN:
		result = WARNING;
		break;

	  default:
		result = ERROR;
		break;
	}
	return(result);
}
