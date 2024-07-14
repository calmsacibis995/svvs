#include	<sv_base.h> /*replacement macros */
#include	<fcntl.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>


#ident	"@(#)ttyname.c	1.1"



extern	char	*ttyname();
extern	int	isatty();

/*
 *  I must open /dev/tty before the call to setup because
 *  /dev/tty will not be available in the environment that setup creates.
 */
static	int	tty_fd;	

/*
** ttyname
**	Test the ttyname, and isatty routines.
*/
main()
{
if ( (tty_fd = open("/dev/tty",O_WRONLY)) == -1 )
	{
	error ("Can not open /dev/tty");
	exit (-1);
	/*NOTREACHED*/
	}

no_close (tty_fd);  /*  inform setup not to close this fd  */
no_setpgrp ();	/*  suppress setpgrp since that dissolves the /dev/tty link  */

setup("ttyname");
	t_ttyname();
	t_isatty();

	if ( close(tty_fd) )
		error ("close of /dev/tty failed");
done();
/*NOTREACHED*/
}

/*
** t_ttyname
**	Test ttyname.
*/
t_ttyname()
{
	char	*sp;
	int	fd;

funct ("ttyname");
testing("ttyname(a file descriptor attached to a terminal)");
testing("should return a string with a terminal name in it.");
start
	expecting (VOID);
	if ( (sp = ttyname(tty_fd)) == NULL )
	{
		error ("ttyname returned NULL");
	}
	else
		if ( strncmp("/dev/",sp,5) )
		{
			error ("ttyname returned '%s',\n%s", sp,
 				"a string starting with '/dev/' was expected");
		}
finis


testing("ttyname(a file descriptor attached to a file)");
testing("should return a NULL string.");
start
	expecting (SUCCESS);
	fd = open("/etc/passwd",O_RDONLY);

	expecting (NULL);
	ttyname(fd);

	expecting (SUCCESS);
	close(fd);
finis


testing("ttyname(a file descriptor attached to /dev/null)");
testing("should return NULL.");
start
	expecting (SUCCESS);
	fd = open("/dev/null",O_RDONLY);

	expecting (NULL);
	ttyname(fd);

	expecting (SUCCESS);
	close(fd);
finis


testing("ttyname(a closed file descriptor)");
testing("should return NULL.");
start
	expecting (NULL);
	ttyname(fd);
finis

}

/*
** t_isatty
**	Test isatty.
*/
t_isatty()
{
	int	ret_val;
	int	fd;

funct ("isatty");
testing("isatty(a file descriptor attached to a terminal)");
testing("should return 1.");
start
	expecting (1);
	isatty(tty_fd);
finis


testing("isatty(a file descriptor attached to a file)");
testing("should return 0.");
start
	expecting (SUCCESS);
	fd = open("/etc/passwd",O_RDONLY);

	expecting (0);
	isatty(fd);

	expecting (SUCCESS);
	close(fd);
finis


testing("isatty(a file descriptor attached to /dev/null)");
testing("should return 0.");
start
	expecting (SUCCESS);
	open("/dev/null",O_RDONLY);

	expecting (0);
	isatty(fd);

	expecting (SUCCESS);
	close(fd);
finis


testing("isatty(called with a closed file descriptor) should return 0.");
start
	expecting (0);
	isatty(fd);
finis

}
