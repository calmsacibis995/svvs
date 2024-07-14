# include "main.h"
# include "option.h"
# include "device.h"
# include "eval_arg.h"
# include "import.h"
# include <fcntl.h>
# include <signal.h>
# include <exec_head.h>

SCCSID("@(#)device.c	3.1 (SVID) 12/1/86");

/*
** Private variables used to hold various information about device states.
** 
** "SET_..." tells us if the device has been set
** "TIO_..." holds termio state of device before we changed it.  We will
**	     use this information when we want to reset the device to its
**	     normal state
** "FD_..." holds the file descriptor associated with each device
*/

static BOOLEAN		SET_Device[2] = {FALSE, FALSE};
static BOOLEAN		SET_Keyboard = FALSE;

static struct termio	TIO_Device[2];
static struct termio	TIO_Keyboard;

static int		FD_Device[2];
static int		FD_Keyboard;

/*
** Holds the current data to place on the input device of the pitcher.
** Also used for catcher mimic routines to emulate input.
*/

char	*KB_Buffer = NULL;

/*
** Forwardly declared functions which are defined at the end of this file
*/

void	set_port();
void	no_filedes_delay();
void	fill_kb_buffer();
void	kickout();

void set_keyboard()
{
/*
	if (close(0) != NULL)
	{
		screech("set_keyboard", 1, "close");
	}
*/

	if ((FD_Keyboard = open("/dev/tty", O_RDONLY)) == -1)
	{
		screech("set_keyboard", 1, "Can't open keyboard");
	}

/*
	if (FD_Keyboard != 0)
	{
		screech("set_keyboard", 0, "failed to open keyboard as stdin");
	}
*/

	if (ioctl(FD_Keyboard, TCGETA, &TIO_Keyboard))
	{
		screech("set_keyboard", 0, "ioctl(,TCGETA,) failed\n");
	}

	no_filedes_delay(FD_Keyboard, 0, 255);

	SET_Keyboard = TRUE;
}

void reset_keyboard()
{
	if (SET_Keyboard)
	{
		if (ioctl(FD_Keyboard, TCSETAW, &TIO_Keyboard))
		{
			screech("reset_keyboard", 
				0, "ioctl(,TCSETAW,) failed\n");
		}
		else
		{
			SET_Keyboard = FALSE;
		}
	}
}

void get_keyboard()
{
	char		buff[1];
	BOOLEAN		done = FALSE;

	kickout();
	buff[0] = ' ';
	(void) fprintf(stdout, "");
	(void) fflush(stdout);

	if (ioctl(FD_Keyboard, TCFLSH, 0))
	{
		screech("get_keyboard", 0, "ioctl(,TCFLSH,) failed\n");
	}

	do
	{
		if (read(FD_Keyboard, buff, 1))
		{
			switch (buff[0])
			{
			  case 'C':
				Option.respond = FALSE;
				break;

			  default:
				done = TRUE;
			}
		}
	} while (!done);
}

void openup(flow, filename, mode)
OPENUP_FLOW	flow;
char		*filename;
OPENUP_MODE	mode;
{
	int	old_filedes;
	int	new_filedes;
	int	flag;

	switch (flow)
	{
	  case OPENUP_INPUT:
		flag = O_RDONLY;
		if (Option.use_pipe)
		{
			flag |= O_NDELAY;
		}

		if (mode == OPENUP_OLD)
		{
			old_filedes = 0;
		}
		break;

	  case OPENUP_OUTPUT:
		flag = O_WRONLY;
		if (mode == OPENUP_OLD)
		{
			old_filedes = 1;
		}
		break;

	  default:
		screech("set_port", 0, 
			"flow must be OPENUP_INPUT or OPENUP_OUTPUT\n");
	}

	if ((mode == OPENUP_OLD) && (close(old_filedes) != NULL))
	{
		screech("openup", 1, "Can't close %d\n", old_filedes);
	}

	if ((new_filedes = open(filename, flag)) == -1)
	{
		screech("openup", 1, "Can't open TIETL port %s\n", filename);
	}

	if ((mode == OPENUP_OLD) && (old_filedes != new_filedes))
	{
		screech("openup", 0, 
			"Opened %d rather than %d\n", new_filedes, old_filedes);
	}

	FD_Device[flow] = new_filedes;

	if (!Option.use_pipe)
	{
		set_port(flow);
	}
}

void set_port(flow)
OPENUP_FLOW	flow;
{
	struct termio	termio_data;
	int		flush_arg;

	switch (flow)
	{
	  case OPENUP_INPUT:
		flush_arg = 0;
		break;

	  case OPENUP_OUTPUT:
		flush_arg = 1;
		break;

	  default:
		screech("set_port", 0, "flow must be OPENUP_INPUT or OPENUP_OUTPUT\n");
	}

	if (ioctl(FD_Device[flow], TCFLSH, flush_arg))
	{
		screech("set_port", 0, "ioctl(,TCFLSH,) failed\n");
	}

	if (ioctl(FD_Device[flow], TCGETA, &termio_data))
	{
		screech("set_port", 0, "ioctl(,TCGETA,) failed\n");
	}

	TIO_Device[flow] = termio_data;

	if (ExecHead.do_xon_xoff)
	{
		termio_data.c_iflag |= IXON | IXOFF;
	}
	else
	{
		termio_data.c_iflag &= ~IXON;
		termio_data.c_iflag &= ~IXOFF;
	}
	termio_data.c_cflag &= ~CBAUD & ~CSIZE;
	termio_data.c_cflag |= (Option.baud_rate | CS7);
	termio_data.c_cflag |= PARENB;

	/*
	termio_data.c_iflag |= IXON | IXOFF;
	termio_data.c_cflag &= ~CBAUD;
	termio_data.c_cflag |= Option.baud_rate;
	*/

	if (ioctl(FD_Device[flow], TCSETAW, &termio_data))
	{
		screech("set_port", 0, "ioctl(,TCSETAW,) failed\n");
	}

	SET_Device[flow] = TRUE;
}

void reset_port(flow)
OPENUP_FLOW	flow;
{
	struct termio	termio_data;
	BOOLEAN		proceed = FALSE;

	switch(flow)
	{
	  case OPENUP_INPUT:
	  case OPENUP_OUTPUT:
		if (SET_Device[flow])
		{
			termio_data = TIO_Device[flow];
			proceed = TRUE;
		}
		break;

	  default:
		screech("reset_port", 0, 
			"filedes must be OPENUP_INPUT or OPENUP_OUTPUT\n");
	}

	if ((proceed) && (ioctl(FD_Device[flow], TCSETA, &termio_data) == NULL))
	{
		SET_Device[flow] = FALSE;
	}
	else if (proceed)
	{
		screech("reset_port", 0, "ioctl(,TCSETA,) failed\n");
	}
}

void no_device_delay(flow)
OPENUP_FLOW	flow;
{
	switch(flow)
	{
	  case OPENUP_INPUT:
	  case OPENUP_OUTPUT:
		break;

	  default:
		screech("no_device_delay", 
			0, 
			"flow must be OPENUP_INPUT or OPENUP_OUTPUT\n");
	}

	no_filedes_delay(FD_Device[flow], Option.MIN, Option.TIME);
}

void no_filedes_delay(filedes, char_count, time_out)
int		filedes;
unsigned	char_count;
unsigned	time_out;
{
	struct	termio	termio_data;

	if (ioctl(filedes, TCGETA, &termio_data))
	{
		screech("no_filedes_delay", 0, "ioctl(,TCGETA,) failed\n");
	}

# ifdef TRACE
	if Trace(5, 0)
	{
		(void) fprintf(stderr, "TIMEOUT = %d\n", time_out);
	}
# endif /* TRACE */

	termio_data.c_lflag &= ~ICANON;
	termio_data.c_lflag &= ~ECHO;

	termio_data.c_cc[4] = char_count;
	termio_data.c_cc[5] = time_out;

	if (ioctl(filedes, TCSETAW, &termio_data))
	{
		screech("no_filedes_delay", 0, "ioctl(,TCSETAW,) failed\n");
	}
}

int read_buff(buf, nbyte)
char		*buf;
unsigned	nbyte;
{
	return(read(FD_Device[OPENUP_INPUT], buf, nbyte));
}

void write_kb_buff()
{
	unsigned	nbyte;

	if (KB_Buffer)
	{
		nbyte = strlen(KB_Buffer);
		(void) write(FD_Device[OPENUP_OUTPUT], KB_Buffer, nbyte);
	}
}

void keyboard(fmt, arg)
VAR_ENTRY	*fmt;
VAR_ENTRY	*arg;
{
	static char	*tmp_str;
	unsigned	new_size;

	(void) pprintf(&tmp_str, fmt, arg);

	if (LastCall)
	{
		if (KB_Buffer)
		{
			new_size = strlen(KB_Buffer) + strlen(tmp_str) + 1;
			if ((KB_Buffer = realloc(KB_Buffer, new_size)) == NULL)
			{
				screech("keyboard", 0, "realloc failed.\n");
			}
			(void) strcat(KB_Buffer, tmp_str);
			free(tmp_str);
		}
		else
		{
			KB_Buffer = tmp_str;
		}
	}
}

void kickout()
{
	char	flush_flag;
	int	flag_count;
	int	flag_index;

	flag_count = strlen(Option.flush_flag);

	for (flag_index = 0; flag_index < flag_count; flag_index++)
	{
		flush_flag = Option.flush_flag[flag_index];

		switch (flush_flag)
		{
		  case 'f':
			if (fflush(stdout))
			{
				screech("kickout", 0, "fflush failed\n");
			}
			break;

		  case 'i':
			if (!Option.use_pipe)
			{
				if (ioctl(FD_Device[OPENUP_OUTPUT], TCFLSH, 1))
				{
					screech("kickout", 0, 
						"ioctl(,TCFLSH,) failed\n");
				}
			}
			break;

		  case 'd':
			if (!Option.use_pipe)
			{
				if (ioctl(FD_Device[OPENUP_OUTPUT], TCSBRK, 1))
				{
					screech("kickout", 0, 
						"ioctl(,TCSBRK,) failed\n");
				}
			}
		  default:
			screech("kickout", 0, 
				"Bad flush flag = %d\n", flush_flag);
		}
	}
}
