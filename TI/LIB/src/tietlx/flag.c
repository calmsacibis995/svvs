# include "main.h"
# include "flag.h"
# include <fcntl.h>

SCCSID("@(#)flag.c	3.3 (SVID) 12/30/86");

static int	PC_Pipe[2];
static int	CP_Pipe[2];

static int	PipeIn;
static int	PipeOut;

void	no_pipe_delay();

void make_pipe()
{
	if (pipe(PC_Pipe) != NULL)
	{
		screech("split_up", 1, "pipe(PC_Pipe)\n");
	}

	if (pipe(CP_Pipe) != NULL)
	{
		screech("split_up", 1, "pipe(CP_Pipe)\n");
	}
}

void set_pipe()
{
	switch (SideType)
	{
	  case SIDE_PITCHER:
		PipeIn = PC_Pipe[0];
		PipeOut = CP_Pipe[1];
		break;

	  case SIDE_CATCHER:
		PipeIn = CP_Pipe[0];
		PipeOut = PC_Pipe[1];
		break;

	  default:
		screech("set_pipe", 0, "SideType not set\n");
	}

	no_pipe_delay(PipeIn);
}

FLAG_TYPE ch_to_bit(ch_flag)
char	ch_flag;
{
	FLAG_TYPE	result;

	switch (ch_flag)
	{
	  case CFLAG_NULL:
		result = FLAG_NULL;
		break;

	  case CFLAG_PROCEED:
		result = FLAG_PROCEED;
		break;

	  case CFLAG_END:
	  	result = FLAG_END;
		break;

	  case CFLAG_SHOW:
		result = FLAG_SHOW;
		break;

	  case CFLAG_ERROR:
		result = FLAG_ERROR;
		break;

	  case CFLAG_WARN:
		result = FLAG_WARN;
		break;

	  default:		
		screech("ch_to_bit", 0, "Bad ch_flag = '%c'\n", ch_flag);
	}

	return(result);
}

char bit_to_ch(bit_flag)
FLAG_TYPE	bit_flag;
{
	char	result;

	switch (bit_flag)
	{
	  case FLAG_NULL:
		result = CFLAG_NULL;
		break;

	  case FLAG_PROCEED:
		result = CFLAG_PROCEED;
		break;

	  case FLAG_END:
	  	result = CFLAG_END;
		break;

	  case FLAG_SHOW:
		result = CFLAG_SHOW;
		break;

	  case FLAG_ERROR:
		result = CFLAG_ERROR;
		break;

	  case FLAG_WARN:
		result = CFLAG_WARN;
		break;

	  default:		
		screech("bit_to_ch", 0, "Bad bit_flag = %d\n", bit_flag);
	}

	return(result);
}

void put_flag(flag)
FLAG_TYPE	flag;
{
	char	buff;

	buff = bit_to_ch(flag);
	if (write(PipeOut, &buff, 1) == NULL)
	{
		screech("put_flag", 0, "write failed\n");
	}
}

FLAG_TYPE take_flag()
{
	char	buff;

	if (read(PipeIn, &buff, 1) != 1)
	{
		buff = CFLAG_NULL;
	}
	return(ch_to_bit(buff));
}

FLAG_TYPE wait_flag(flag_set)
FLAG_TYPE	flag_set;
{
	char		char_flag = CFLAG_NULL;
	FLAG_TYPE	bit_flag = FLAG_NULL;
	int		rounds = 0;

	do
	{
		if (!read(PipeIn, &char_flag, 1))
		{
			char_flag = CFLAG_NULL;
			chk_partner(&rounds);
		}
		else
		{
			bit_flag = ch_to_bit(char_flag);

# ifdef TRACE
			if (Trace(15, 0))
			{
				(void) fprintf(stderr, 
					       "Want %d; got %c\n", 
					       bit_to_ch(flag_set), char_flag);
			}
# endif /* TRACE */

		}
	} while (bit_flag == FLAG_NULL);

	if (!(bit_flag & flag_set))
	{
		screech("wait_flag", 0, 
			"received unexpected flag = %d\n", bit_flag);
	}

	return(bit_flag);
}

void no_pipe_delay(pipedes)
int	pipedes;
{
	int	flags;

	if ((flags = fcntl(pipedes, F_GETFL, NULL)) == -1)
	{
		screech("no_pipe_delay", 1, "fcntl(,F_GETFL,)");
	}

	flags |= O_NDELAY;

	if (fcntl(pipedes, F_SETFL, flags) == -1)
	{
		screech("no_pipe_delay", 1, "fcntl(,F_SETFL,)");
	}
}
