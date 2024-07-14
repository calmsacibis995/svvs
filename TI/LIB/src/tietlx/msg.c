# include "main.h"
# include "vs.h"
# include "option.h"
# include "msg.h"
# include "flag.h"
# include <varargs.h>

SCCSID("@(#)msg.c	3.2 (SVID) 12/23/86");

BOOLEAN	MsgTouched = FALSE;
BOOLEAN	UmpireError = FALSE;
BOOLEAN	MsgWarning = FALSE;

static FILE	*FP_Msg;
static char	*Name_Msg;
static char	*FullPath;

# define STD_MSG	"Expected and actual terminal screens did not match."

void msg_open()
{
	char	*mode;

	switch (SideType)
	{
	  case SIDE_PITCHER:
		mode = "r";
		break;

	  case SIDE_CATCHER:
		mode = "w";
		break;

	  default:
		screech("msg_open", 0, "Bad SideType = %d\n", SideType);
	}

# ifdef TRACE
	if (Trace(30, 0))
	{
		(void) fprintf(stderr, "Calling temp_open(\"%s\", \"%s\")\n", 
			       FullPath, mode);
	}
# endif

	FP_Msg = vs_t_fopen(Name_Msg, mode);
}

void msg_close()
{
	if (SideType == SIDE_CATCHER)
	{
		(void) fflush(FP_Msg);
	}
	(void) fclose(FP_Msg);
}

/* VARARGS */

void msg_drop(va_alist)
va_dcl
{
	va_list	args;
	char	*fmt;

	if (!MsgTouched)
	{
		msg_open();
		MsgTouched = TRUE;
	}

	va_start(args);
	fmt = va_arg(args, char *);
	(void) vfprintf(FP_Msg, fmt, args);
	va_end(args);
}

void msg_pickup(flag_type)
FLAG_TYPE	flag_type;
{
	int	in_char;
	char	buff[512];
	char	*buff_end;

	switch (flag_type)
	{
	  case FLAG_WARN:
		msg_begin(wvs_msg_type(flag_type), 
			  "%s\n%s\n", STD_MSG, wvs_advisory());
		break;

	  default:
		msg_begin(wvs_msg_type(flag_type), 
			  "%s\n", STD_MSG);
		break;
	}

	msg_body("\t%s\n", MSG_BOS);

	msg_open();
	buff_end = &buff[0];
	while ((in_char = getc(FP_Msg)) != EOF)
	{
		if ((*buff_end++ = in_char) == '\n')
		{
			*buff_end = '\0';
			if (strlen(buff) > 512)
			{
				screech("msg_pickup", 0, "Buffer too big\n");
			}
			msg_body("\t%s",buff);
			buff_end = &buff[0];
		}
	}

	if (buff_end != &buff[0])
	{
		*buff_end++ = '\n';
		*buff_end = '\0';

		if (strlen(buff) > 512)
		{
			screech("msg_pickup", 0, "Buffer too big\n");
		}
		msg_body("\t%s",buff);
	}
	msg_close();

	msg_body("\t%s\n", MSG_EOS);

	UmpireError = TRUE;

	msg_end();
}

void msg_name()
{
	char	tmp_str[BASIC_STR_LEN];

	if (Option.obj_file == NULL)
	{
		(void) sprintf(tmp_str, "tietl.%d", getpid());
	}
	else
	{
		(void) sprintf(tmp_str, "%s.%d", Option.obj_file, getpid());
	}

	if ((Name_Msg = malloc((unsigned) (strlen(tmp_str) + 1))) == NULL)
	{
		screech("msg_name", 0, "malloc failed\n");
	}

	(void) strcpy(Name_Msg, tmp_str);

	(void) sprintf(tmp_str, "%s%s", wvs_t_path(), Name_Msg);
	if ((FullPath = malloc((unsigned) (strlen(tmp_str) + 1))) == NULL)
	{
		screech("msg_name", 0, "malloc failed\n");
	}

	(void) strcpy(FullPath, tmp_str);
}
