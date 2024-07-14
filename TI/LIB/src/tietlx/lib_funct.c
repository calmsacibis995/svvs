# include "main.h"
# include "option.h"
# include "import.h"
extern char	*tgetstr();

SCCSID("@(#)lib_funct.c	3.1 (SVID) 12/1/86");

int t_settrace(trace_id, str_val)
int	trace_id;
char	*str_val;
{
	int	result;

# ifdef	TRACE
	switch(trace_id)
	{
	  case TRACE_BLESS:
		result = set_trace(Bless_trace, str_val, Bless_map);
		break;

	  case TRACE_TIETL:
		result = set_trace(TIETL_trace, str_val, TIETL_map);
		break;

	  default:
		screech("t_trace", 0, "Bad trace ID = %d\n", trace_id);
	}

	return(result);
# else	/* TRACE */
	return ( 0 );
# endif	/* TRACE */
}

void t_clrtrace(trace_id)
int	trace_id;
{
# ifdef	TRACE
	switch(trace_id)
	{
	  case TRACE_BLESS:
		clr_trace(Bless_trace);
		break;

	  case TRACE_TIETL:
		clr_trace(TIETL_trace);
		break;

	  default:
		screech("t_clrtrace", 0, "Bad trace ID = %d\n", trace_id);
	}
# endif	/* TRACE */
}


int tprintf(dest_arg, fmt, arg)
VAR_ENTRY	*dest_arg;
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;
	int		result;

	result = pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		if (dest_arg->var_type == DATA_TEXT)
		{
			free_text(dest_arg);
		}
		else
		{
			dest_arg->var_type = DATA_TEXT;
		}

		dest_arg->var_data.var_text_ptr = tmp_str;
	}

	return(result);
}

int fkeyoffset(offset)
{
	return(KEY_F0 + offset);
}

char *fkeyseq(fkeycode)
int	fkeycode;
{
	static char	*str = NULL;
	char		*id_str;

	switch(fkeycode)
	{
	  case KEY_BREAK:	id_str = NULL;	break;
	  case KEY_DOWN:	id_str = "kd";	break;
	  case KEY_UP:		id_str = "ku";	break;
	  case KEY_LEFT:	id_str = "kl";	break;
	  case KEY_RIGHT:	id_str = "kr";	break;
	  case KEY_HOME:	id_str = "kh";	break;
	  case KEY_BACKSPACE:	id_str = "kb";	break;
	  case KEY_F0:		id_str = "k0";	break;
	  case (KEY_F0 + 1):	id_str = "k1";	break;
	  case (KEY_F0 + 2):	id_str = "k2";	break;
	  case (KEY_F0 + 3):	id_str = "k3";	break;
	  case (KEY_F0 + 4):	id_str = "k4";	break;
	  case (KEY_F0 + 5):	id_str = "k5";	break;
	  case (KEY_F0 + 6):	id_str = "k6";	break;
	  case (KEY_F0 + 7):	id_str = "k7";	break;
	  case (KEY_F0 + 8):	id_str = "k8";	break;
	  case (KEY_F0 + 9):	id_str = "k9";	break;
	  case (KEY_F0 + 10):	id_str = "ka";	break;
	  case (KEY_F0 + 11):	id_str = "F1";	break;
	  case (KEY_F0 + 12):	id_str = "F2";	break;
	  case (KEY_F0 + 13):	id_str = "F3";	break;
	  case (KEY_F0 + 14):	id_str = "F4";	break;
	  case (KEY_F0 + 15):	id_str = "F5";	break;
	  case (KEY_F0 + 16):	id_str = "F6";	break;
	  case (KEY_F0 + 17):	id_str = "F7";	break;
	  case (KEY_F0 + 18):	id_str = "F8";	break;
	  case (KEY_F0 + 19):	id_str = "F9";	break;
	  case (KEY_F0 + 20):	id_str = "FA";	break;
	  case (KEY_F0 + 21):	id_str = "FB";	break;
	  case (KEY_F0 + 22):	id_str = "FC";	break;
	  case (KEY_F0 + 23):	id_str = "FD";	break;
	  case (KEY_F0 + 24):	id_str = "FE";	break;
	  case (KEY_F0 + 25):	id_str = "FF";	break;
	  case (KEY_F0 + 26):	id_str = "FG";	break;
	  case (KEY_F0 + 27):	id_str = "FH";	break;
	  case (KEY_F0 + 28):	id_str = "FI";	break;
	  case (KEY_F0 + 29):	id_str = "FJ";	break;
	  case (KEY_F0 + 30):	id_str = "FK";	break;
	  case (KEY_F0 + 31):	id_str = "FL";	break;
	  case (KEY_F0 + 32):	id_str = "FM";	break;
	  case (KEY_F0 + 33):	id_str = "FN";	break;
	  case (KEY_F0 + 34):	id_str = "FO";	break;
	  case (KEY_F0 + 35):	id_str = "FP";	break;
	  case (KEY_F0 + 36):	id_str = "FQ";	break;
	  case (KEY_F0 + 37):	id_str = "FR";	break;
	  case (KEY_F0 + 38):	id_str = "FS";	break;
	  case (KEY_F0 + 39):	id_str = "FT";	break;
	  case (KEY_F0 + 40):	id_str = "FU";	break;
	  case (KEY_F0 + 41):	id_str = "FV";	break;
	  case (KEY_F0 + 42):	id_str = "FW";	break;
	  case (KEY_F0 + 43):	id_str = "FX";	break;
	  case (KEY_F0 + 44):	id_str = "FY";	break;
	  case (KEY_F0 + 45):	id_str = "FZ";	break;
	  case (KEY_F0 + 46):	id_str = "Fa";	break;
	  case (KEY_F0 + 47):	id_str = "Fb";	break;
	  case (KEY_F0 + 48):	id_str = "Fc";	break;
	  case (KEY_F0 + 49):	id_str = "Fd";	break;
	  case (KEY_F0 + 50):	id_str = "Fe";	break;
	  case (KEY_F0 + 51):	id_str = "Ff";	break;
	  case (KEY_F0 + 52):	id_str = "Fg";	break;
	  case (KEY_F0 + 53):	id_str = "Fh";	break;
	  case (KEY_F0 + 54):	id_str = "Fi";	break;
	  case (KEY_F0 + 55):	id_str = "Fj";	break;
	  case (KEY_F0 + 56):	id_str = "Fk";	break;
	  case (KEY_F0 + 57):	id_str = "Fl";	break;
	  case (KEY_F0 + 58):	id_str = "Fm";	break;
	  case (KEY_F0 + 59):	id_str = "Fn";	break;
	  case (KEY_F0 + 60):	id_str = "Fo";	break;
	  case (KEY_F0 + 61):	id_str = "Fp";	break;
	  case (KEY_F0 + 62):	id_str = "Fq";	break;
	  case (KEY_F0 + 63):	id_str = "Fr";	break;
	  case KEY_DL:		id_str = "kL";	break;
	  case KEY_IL:		id_str = "kA";	break;
	  case KEY_DC:		id_str = "kD";	break;
	  case KEY_IC:		id_str = "kI";	break;
	  case KEY_EIC:		id_str = NULL;	break;
	  case KEY_CLEAR:	id_str = "kC";	break;
	  case KEY_EOS :	id_str = "kS";	break;
	  case KEY_EOL :	id_str = "kE";	break;
	  case KEY_SF :		id_str = "kF";	break;
	  case KEY_SR :		id_str = "kR";	break;
	  case KEY_NPAGE:	id_str = "kN";	break;
	  case KEY_PPAGE:	id_str = "kP";	break;
	  case KEY_STAB:	id_str = "kT";	break;
	  case KEY_CTAB:	id_str = "kt";	break;
	  case KEY_CATAB:	id_str = "k;";	break;
	  case KEY_ENTER:	id_str = "@8";	break;
	  case KEY_SRESET:	id_str = NULL;	break;
	  case KEY_RESET:	id_str = NULL;	break;
	  case KEY_PRINT:	id_str = "%9";	break;
	  case KEY_LL:		id_str = "kH";	break;
	  case KEY_A1:		id_str = "K1";	break;
	  case KEY_A3:		id_str = "K3";	break;
	  case KEY_B2:		id_str = "K2";	break;
	  case KEY_C1:		id_str = "K4";	break;
	  case KEY_C3:		id_str = "K5";	break;
	  case KEY_BTAB:	id_str = "kB";	break;
	  case KEY_BEG:		id_str = "@1";	break;
	  case KEY_CANCEL:	id_str = "@2";	break;
	  case KEY_CLOSE:	id_str = "@3";	break;
	  case KEY_COMMAND:	id_str = "@4";	break;
	  case KEY_COPY:	id_str = "@5";	break;
	  case KEY_CREATE:	id_str = "@6";	break;
	  case KEY_END:		id_str = "@7";	break;
	  case KEY_EXIT:	id_str = "@9";	break;
	  case KEY_FIND:	id_str = "@0";	break;
	  case KEY_HELP:	id_str = "%1";	break;
	  case KEY_REDO:	id_str = "%2";	break;
	  case KEY_MARK:	id_str = "%3";	break;
	  case KEY_MESSAGE:	id_str = "%4";	break;
	  case KEY_MOVE:	id_str = "%5";	break;
	  case KEY_NEXT:	id_str = "%6";	break;
	  case KEY_OPEN:	id_str = "%7";	break;
	  case KEY_OPTIONS:	id_str = "%8";	break;
	  case KEY_PREVIOUS:	id_str = "%0";	break;
	  case KEY_REFERENCE:	id_str = "&1";	break;
	  case KEY_REFRESH:	id_str = "&2";	break;
	  case KEY_REPLACE:	id_str = "&3";	break;
	  case KEY_RESTART:	id_str = "&4";	break;
	  case KEY_RESUME:	id_str = "&5";	break;
	  case KEY_SAVE:	id_str = "&6";	break;
	  case KEY_SBEG:	id_str = "&9";	break;
	  case KEY_SCANCEL:	id_str = "&0";	break;
	  case KEY_SCOMMAND:	id_str = "*1";	break;
	  case KEY_SCOPY:	id_str = "*2";	break;
	  case KEY_SCREATE:	id_str = "*3";	break;
	  case KEY_SDC:		id_str = "*4";	break;
	  case KEY_SDL:		id_str = "*5";	break;
	  case KEY_SELECT:	id_str = "*6";	break;
	  case KEY_SEND:	id_str = "*7";	break;
	  case KEY_SEOL:	id_str = "*8";	break;
	  case KEY_SEXIT:	id_str = "*9";	break;
	  case KEY_SFIND:	id_str = "*0";	break;
	  case KEY_SHELP:	id_str = "#1";	break;
	  case KEY_SHOME:	id_str = "#2";	break;
	  case KEY_SIC:		id_str = "#3";	break;
	  case KEY_SLEFT:	id_str = "#4";	break;
	  case KEY_SMESSAGE:	id_str = "%a";	break;
	  case KEY_SMOVE:	id_str = "%b";	break;
	  case KEY_SNEXT:	id_str = "%c";	break;
	  case KEY_SOPTIONS:	id_str = "%d";	break;
	  case KEY_SPREVIOUS:	id_str = "%e";	break;
	  case KEY_SPRINT:	id_str = "%f";	break;
	  case KEY_SREDO:	id_str = "%g";	break;
	  case KEY_SREPLACE:	id_str = "%h";	break;
	  case KEY_SRIGHT:	id_str = "%i";	break;
	  case KEY_SRSUME:	id_str = "%j";	break;
	  case KEY_SSAVE:	id_str = "!1";	break;
	  case KEY_SSUSPEND:	id_str = "!2";	break;
	  case KEY_SUNDO:	id_str = "!3";	break;
	  case KEY_SUSPEND:	id_str = "&7";	break;
	  case KEY_UNDO:	id_str = "&8";	break;
	  default:
		screech("fkeyseq", 0, "Unknown key %d\n", fkeycode);
	}

	if (id_str == NULL)
	{
		screech("fkeyseq", 0, 
			"Don't know termcap id for key %d\n", fkeycode);
	}
	else if ((str = tgetstr(id_str, NULL)) == NULL)
	{
		str = "";
	}

	return(str);
}

void c_error(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		screech("Catcher", 0, tmp_str);
	}
}

void c_warning(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		screech("Catcher", 0, tmp_str);
	}
}
