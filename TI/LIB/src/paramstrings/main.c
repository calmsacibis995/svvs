# include <sv_base.h>
# include <svvs.h>
# include <sv_env.h>
# include <curses.h>
# include <fcntl.h>

/*
**	main.c
**
**	main.c contains main(), some miscellaneous routines and
**	tgo() which tests the tgoto function.
**
**	putting tests the tputs(), vidputs(), and tgoto() curses
**	routines.  each of these routines is tested individually
**	on various types of data.  the output of these functions
**	is put into an internal buffer and then compared to the
**	expected result.
*/

# include <sccs.h>

SCCSID("@(#)main.c	3.1 (SVVS) 12/1/86");

# define STRSIZE	256

char		outbuf[STRSIZE];
char		*outptr;
int		RealTTY;
struct termio	TermIOData;

void	set_port();
void	reset_port();

main()
{
	setup("strings");

	abstract("tputs(), vidputs(), and tgoto().  the output of these");
	abstract("functions is put into an internal buffer and then");
	abstract("compared to the expected result.");
	start
		exit_on(WARNING | ERROR | SEVERR);

		prep("Open file descriptor to a real tty device.");
		prep("Use this descriptor in all calls to \"setupterm()\".");
		start
			expecting(SUCCESS);
			RealTTY = open(DEFAULT_PITCH_DEV, O_RDWR, 0);
		finis

# ifdef DO_PAD_TEST
		prep("Turn off X-ON/X-OFF processing and set baud rate");
		prep("to 4800 baud. This will insure padding is done.");
		start
			set_port(RealTTY);
		finis
# endif /* DO_PAD_TEST */

		testing("tputs()");
		start
			tput();
		finis

		testing("vidputs()");
		start
			vputs();
		finis

		testing("tgoto");
		start
			tgo();
		finis
	finis

	cleanup("reset_shell_mode to restore original state");
	cleanup("Also reset terminal device to previous state.");
	start
		exit(WARNING | ERROR | SEVERR);

		Zreset_shell_mode();

# ifdef DO_PAD_TEST
		reset_port(RealTTY);
# endif /* DO_PAD_TEST */

	finis

	done();
}

/*
**	outc() takes the character passed and and puts it
**	into outbuf.  outbuf is an array of characters that
**	is used instead of putting the char to the screen.
*/
int outc(c)
int c;
{
	*outptr = c;
	outptr++;
}

/*
**	nullbuf() sets the global outptr to the start of outbuf
**	and puts all nulls in outbuf
*/
nullbuf()
{
	int i;

	outptr = outbuf;
	for (i = 0; i < STRSIZE; i++)
		outbuf[i] = '\0';
}

void set_port(fd)
int	fd;
{
	expecting(0);
	ioctl(fd, TCFLSH, 1);

	expecting(0);
	ioctl(fd, TCGETA, &TermIOData);

	TermIOData.c_iflag &= ~IXON;
	TermIOData.c_iflag &= ~IXOFF;

	TermIOData.c_cflag &= ~CBAUD & ~CSIZE;
	TermIOData.c_cflag |= (B4800 | CS7);
	TermIOData.c_cflag |= PARENB;

	expecting(0);
	ioctl(fd, TCSETAW, &TermIOData);
}

void reset_port(fd)
int	fd;
{
	expecting(0);
	ioctl(fd, TCSETA, &TermIOData);
}
