#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<termio.h>
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)ioctl.c	1.3"

# define	TTY	"/dev/console"


/*
** ioctl
**	Test the ioctl function.
*/

main()
{
	int	fildes;
	int	ioctl();
	struct	termio	buf;


	setup("ioctl");
	funct("ioctl");

	exit_on ( ERROR );

	testing("A legal invocation of ioctl on /dev/console.");
	testing("Expect a return value of 0.");
	start

		expecting ( SUCCESS );
		fildes = open(TTY, O_WRONLY, O_NDELAY);

		expecting ( 0 );
		ioctl(fildes, TCGETA, &buf);

		expecting ( SUCCESS );
		close ( fildes );
	finis


	testing("Ioctl(invalid file descriptor).");
	testing("Expect a return value of -1 and an errno of EBADF.");
	start

		expecting( EBADF );
		ioctl(-1, TCGETA, &buf);

	finis


	testing("Ioctl(invalid request).");
	testing("Expect a return value of -1 and an errno of EBADF.");
	start

		expecting( EBADF );
		ioctl(fildes, -1, & buf);

	finis


	testing("Ioctl(invalid arg).");
	testing("Expect a return value of -1 and an errno of EINVAL.");
	start

		expecting( EBADF );
		ioctl(fildes, TCFLSH, -1);

	finis


	testing("Ioctl(invalid file descriptor).");
	testing("Expect a return value of -1 and an errno of EBADF.");
	start

		expecting( EBADF );
		ioctl(-1, TCGETA, &buf);


	finis


	testing("Ioctl(device that is not character special).");
	testing("Expect a return value of -1 and an errno of ENOTTY.");

	start

		expecting ( SUCCESS );
		fildes = data_open("ioctl_notspec", O_RDWR, 0);

		expecting( ENOTTY );
		ioctl(fildes, TCGETA, &buf);

	finis

	done();
	/*NOTREACHED*/
}
