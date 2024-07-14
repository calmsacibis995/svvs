# include "main.h"
# include "default.h"
# include "option.h"
# include "import.h"
# include "msg.h"
# include "wvs.h"
# include <fcntl.h>
# include <signal.h>

SCCSID("@(#)prologue.c	3.2 (SVID) 12/23/86");

extern char	*getenv();
extern int	putenv();
extern void	init_BlessTerm();

void	set_option();
void	reset_option();
void	process_args();
void	to_stderr();
void	usage();
BOOLEAN	set_speed();
void	set_env();
BOOLEAN	set_flush();
BOOLEAN	set_output();
BOOLEAN	set_run();
BOOLEAN	set_hard();
int	set_TIME();

void prologue(argc, argv)
int	argc;
char	*argv[];
{
	clr_trace(TIETL_trace);
	set_option();

	process_args(argc, argv);
	reset_option();
	
	if ((Option.run_bless) || (Option.run_mimic))
	{
		init_BlessTerm();
	}

	if (!hcreate(HASH_TABLE_LEN))
	{
		screech("prologue", 0, "hcreate() failed\n");
	}

	vs_setup(Option.obj_file);

	if (Option.core_dump)
	{
		grab_sig();
	}

	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		screech("prologue", 0, "Can't ignore signal SIGHUP\n");
	}

	set_svvs_option();

	msg_name();
	vs_no_expecting();
}

void set_option()
{
	Option.obj_file = 	DEFAULT_OBJ_FILE;
	Option.use_stdin = 	DEFAULT_USE_STDIN;

	Option.pitch_dev_name = NULL;
	Option.catch_dev_name = NULL;

	Option.show_struct = 	DEFAULT_SHOW_STRUCT;
	Option.show_var = 	DEFAULT_SHOW_VAR;

	Option.term_type = 	NULL;

	Option.hard = 		set_hard();
	Option.use_pipe = 	DEFAULT_USE_PIPE;
	Option.pipe_name = 	DEFAULT_PIPE_NAME;

	Option.TIME = 		set_TIME();
	Option.MIN = 		DEFAULT_MIN;

	Option.baud_rate = 	DEFAULT_BAUD_RATE;
	Option.block_dump = 	DEFAULT_BLOCK_DUMP;

	Option.run_mimic = 	DEFAULT_RUN_MIMIC;
	Option.run_bless = 	DEFAULT_RUN_BLESS;
	Option.run_umpire = 	DEFAULT_RUN_UMPIRE;

	Option.direct = 	DEFAULT_DIRECT;

	Option.output_bless = 	DEFAULT_OUTPUT_BLESS;
	Option.output_mimic = 	DEFAULT_OUTPUT_MIMIC;
	Option.output_umpire = 	DEFAULT_OUTPUT_UMPIRE;

	Option.respond = 	DEFAULT_RESPOND;
	Option.flush_flag = 	DEFAULT_FLUSH_FLAG;

	Option.bell = 		DEFAULT_BELL;
	Option.noisy = 		DEFAULT_NOISY;
	Option.core_dump = 	DEFAULT_CORE_DUMP;
	Option.size_file =	NULL;
	Option.dump_error =	DEFAULT_DUMP_ERROR;
}

void reset_option()
{
	if (Option.respond)
	{
		if ((!Option.direct) && 
		    (!Option.output_mimic) && 
		    (!Option.output_bless) && 
		    (!Option.output_umpire))
		{
			Option.direct = TRUE;
		}
	}

	if (Option.direct)
	{
		Option.output_bless = FALSE;
		Option.output_mimic = FALSE;
		Option.output_umpire = FALSE;
		if (!Option.term_type)
		{
			if ((Option.term_type = getenv("TERM")) == NULL)
			{
				screech("reset_option", 0, 
					"Set TERM or use -t\n");
			}
		}
	}

	if (!Option.term_type)
	{
		if (((Option.term_type = getenv("TERM")) == NULL) ||
		    (strncmp(Option.term_type, TERM_ROOT, strlen(TERM_ROOT))))
		{
			Option.term_type = DEFAULT_TERM_TYPE;
		}
	}

	set_env(Option.term_type);

	if (!Option.run_mimic)
	{
		Option.output_mimic = FALSE;
		Option.run_umpire = FALSE;
		Option.output_umpire = FALSE;
	}

	if (!Option.run_bless)
	{
		Option.output_bless = FALSE;
		Option.run_umpire = FALSE;
		Option.output_umpire = FALSE;
	}

	if (Option.use_stdin)
	{
		Option.obj_file = NULL;
	}

	if ((!Option.direct) && 
	    (!Option.run_bless) && 
	    (!Option.run_mimic) &&
	    (!Option.run_umpire))
	{
		(void) fprintf(stderr, "Set at least one: m, b, u, d\n");
		usage();
		screech(NULL, 0, NULL);
	}
}

void process_args(argc, argv)
int		argc;
char		*argv[];
{
	char	*s;

	while (--argc > 0 && (*++argv)[0] == '-')
	{
		s = argv[0]+1;
		if (*s == '\0')
		{
			usage();
			screech(NULL, 0, NULL);
		}

		for (; *s != '\0'; s++)
		{
			switch (*s)
			{
			  case '?':
				usage();
				screech(NULL, 0, NULL);
				break;

			  case 'i':
				Option.use_stdin = !Option.use_stdin;
				break;

			  case 'o':
				/* tietli only */
				break;

			  case 'x':
				if (--argc > 0)
				{
					if (!set_run((*++argv)))
					{
						argc = -1;
					}
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'd':
				Option.direct = !Option.direct;
				break;

			  case 'O':
				if (--argc > 0)
				{
					if (!set_output((*++argv)))
					{
						argc = -1;
					}
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'h':
				Option.hard = !Option.hard;
				break;

			  case 's':
				if (--argc > 0)
				{
					if (!set_speed((*++argv)))
					{
						argc = -1;
					}
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'M':
				if (--argc > 0)
				{
					(void) sscanf((*++argv), 
						      "%d", 
						      &Option.MIN);
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'T':
				if (--argc > 0)
				{
					(void) sscanf((*++argv), 
						      "%d", 
						      &Option.TIME);
				}
				else
				{
					argc = -1;
				}
				break;

			  case 't':
				if (--argc > 0)
				{
					Option.term_type = (*++argv);
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'p':
				if (--argc > 0)
				{
					Option.pitch_dev_name = (*++argv);
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'c':
				if (--argc > 0)
				{
					Option.catch_dev_name = (*++argv);
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'P':
				Option.use_pipe = !Option.use_pipe;
				break;

			  case 'r':
				Option.respond = !Option.respond;
				break;

			  case 'g':
				Option.bell = !Option.bell;
				break;

			  case 'n':
				Option.noisy = !Option.noisy;
				break;

			  case 'C':
				Option.core_dump = !Option.core_dump;
				break;

			  case 'l':
				Option.show_struct = !Option.show_struct;
				break;

			  case 'S':
				Option.show_var = !Option.show_var;
				break;

			  case 'f':
				/* tietli only */
				break;

			  case 'F':
				if (--argc > 0)
				{
					Option.flush_flag = *++argv;
					if (!set_flush(Option.flush_flag))
					{
						argc = -1;
					}
				}
				else
				{
					argc = -1;
				}
				break;

# ifdef	TRACE
			  case 'B':
				if (--argc > 0)
				{
					if (set_trace(Bless_trace, *++argv, Bless_map) == -1)
					{
						(void) fprintf(stderr,
							       "Bad Bless debug flags\n");
						argc = -1;
					}
				}
				else
				{
					argc = -1;
				}
				break;

			  case 'I':
				if (--argc > 0)
				{
					if (set_trace(TIETL_trace, *++argv, TIETL_map) == -1)
					{
						(void) fprintf(stderr,
							       "Bad TIETL debug flags\n");
						argc = -1;
					}
				}
				else
				{
					argc = -1;
				}
				break;
# endif	/* TRACE */

			  case 'L':
				Option.size_file = DEFAULT_SIZE_FILE;
				break;

			  case 'D':
				Option.dump_error = !Option.dump_error;
				break;

			  default:
				(void) fprintf(stderr, 
					       "tietl: illegal option %c\n", 
					       *s);
				argc = -1;
				break;
			}
		}
	}

	switch (argc)
	{
	  case 0:
		Option.obj_file = NULL;	/* Use stdin */
		break;

	  case 1:
		Option.obj_file = argv[0];
		break;

	  default:
		usage();
		screech(NULL, 0, NULL);
	}
}

void to_stderr(str)
char	*str;
{
	(void) fprintf(stderr, str);
}

void usage()
{
	to_stderr("Usage: tietl [options] [file]\n");
	to_stderr("Option Arg      Default    Description\n");
	to_stderr("\n");
	to_stderr("    -i          FALSE      read \"stdin\" no matter what\n");
	to_stderr("    -x list     FALSE      don't run b, m, u\n");
	to_stderr("    -d          FALSE      run direct TIE routines\n");
	to_stderr("                           (suppresses b, m, u output)\n");
	to_stderr("    -O list     u          Switch output b, m, u\n");
	to_stderr("    -h          FALSE      hard testing\n");
	to_stderr("    -s speed    9600       speed (baud rate)\n");
	to_stderr("    -M charcnt  0          MIN char count \n");
	to_stderr("    -T timeout  5          TIME timeout in 1/10 seconds\n");
	to_stderr("    -t termtype SVVS3      terminal type\n");
	to_stderr("    -p device   /dev/tty31 pitcher device\n");
	to_stderr("    -c device   /dev/tty32 catcher device\n");
	to_stderr("    -P          FALSE      named pipe\n");
	to_stderr("    -r          FALSE      respond between expects\n");
	to_stderr("    -g          FALSE      sound bell when timeout\n");
	to_stderr("    -n          FALSE      noisy output between expects\n");
	to_stderr("    -C          FALSE      catch signals and dump core\n");
	to_stderr("    -l          FALSE      show lines in program\n");
	to_stderr("    -S          FALSE      show symbol table\n");
	to_stderr("    -F list     f          set flush type f, i, d\n");
	to_stderr("    -B          FALSE      turn on Bless debug flag\n");
	to_stderr("    -I          FALSE      turn on TIETL debug flag\n");
	to_stderr("    -L          TRUE       report program length\n");
	to_stderr("    -D          TRUE       dump screen on error\n");
}

BOOLEAN set_speed(str)
char	*str;
{
	BOOLEAN	result;

	result = TRUE;

	if (!strcmp(str, "50")) 	Option.baud_rate = B50; 	else
	if (!strcmp(str, "75")) 	Option.baud_rate = B75; 	else
	if (!strcmp(str, "110")) 	Option.baud_rate = B110; 	else
	if (!strcmp(str, "134")) 	Option.baud_rate = B134; 	else
	if (!strcmp(str, "150")) 	Option.baud_rate = B150; 	else
	if (!strcmp(str, "200")) 	Option.baud_rate = B200; 	else
	if (!strcmp(str, "300")) 	Option.baud_rate = B300; 	else
	if (!strcmp(str, "600")) 	Option.baud_rate = B600; 	else
	if (!strcmp(str, "1200")) 	Option.baud_rate = B1200; 	else
	if (!strcmp(str, "1800")) 	Option.baud_rate = B1800; 	else
	if (!strcmp(str, "2400")) 	Option.baud_rate = B2400; 	else
	if (!strcmp(str, "4800")) 	Option.baud_rate = B4800; 	else
	if (!strcmp(str, "9600")) 	Option.baud_rate = B9600; 	else
	if (!strcmp(str, "19200"))	Option.baud_rate = B19200; 	else
	if (!strcmp(str, "38400")) 	Option.baud_rate = B38400; 	else
	result = FALSE;

	if (! result)
	{
		(void) fprintf(stderr, "tietl: bad baud rate %s\n", str);
	}

	return(result);
}

BOOLEAN set_output(subject_list)
char	*subject_list;
{
	BOOLEAN	result = TRUE;

	while (*subject_list != '\0')
	{
		switch (*subject_list)
		{
		  case 'm':
			Option.output_mimic = TRUE;
			break;

		  case 'b':
			Option.output_bless = TRUE;
			break;

		  case 'u':
			Option.output_umpire = FALSE;
			break;

		  default:
			(void) fprintf(stderr, "tietl: bad suppress code %c\n", *subject_list);
			result = FALSE;
			break;
		}
		subject_list++;
	}

	return(result);
}

BOOLEAN set_run(subject_list)
char	*subject_list;
{
	BOOLEAN	result = TRUE;

	while (*subject_list != '\0')
	{
		switch (*subject_list)
		{
		  case 'm':
			Option.run_mimic = FALSE;
			break;

		  case 'b':
			Option.run_bless = FALSE;
			break;

		  case 'u':
			Option.run_umpire = FALSE;
			break;

		  default:
			(void) fprintf(stderr, "tietl: bad suppress code %c\n", *subject_list);
			result = FALSE;
			break;
		}
		subject_list++;
	}

	return(result);
}

void set_env(term_type)
char	*term_type;
{
	char		*env_var;
	char		*env_prefix = "TERM=";
	unsigned	length;

	length = strlen(env_prefix) + strlen(term_type) + 1;
	env_var = malloc(length);

	(void) strcpy(env_var, env_prefix);
	(void) strcat(env_var, term_type);

	if (putenv(env_var))
	{
		screech("set_env", 0, "putenv() failed\n");
	}
}

BOOLEAN set_flush(flag_list)
char	*flag_list;
{
	BOOLEAN	result = TRUE;

	while (*flag_list != '\0')
	{
		switch (*flag_list)
		{
		  case 'f':
		  case 'i':
		  case 'd':
			break;

		  default:
			(void) fprintf(stderr, "tietl: bad flush flag %c\n", *flag_list);
			result = FALSE;
			break;
		}
		flag_list++;
	}

	return(result);
}

/*
**	"sscanf()" is used instead of "atoi()" for better error detection
*/

int set_TIME()
{
	char	*TIME_val;
	int	result;

	if ((TIME_val = getenv("TIMEOUT")) == NULL)
	{
		result = DEFAULT_TIME;
	}
	else if (sscanf(TIME_val, "%d", &result) != 1)
	{
		result = DEFAULT_TIME;
	}

	return(result);
}

int set_hard()
{
	int	result;

/*
**	Until we are ready to test the return value of any TIE function,
**	we should always return FALSE. The default value should be FALSE
**	so always return DEFAULT_HARD.
**
**	if (getenv("SVVS_HARD") == NULL)
**	{
**		result = DEFAULT_HARD;
**	}
**	else
**	{
**		result = TRUE;
**	}
**
**	return(result);
*/

	return(DEFAULT_HARD);
}
