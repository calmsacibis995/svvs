#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)devtty.c	1.3"


/*
**	Test for the presence of /dev/tty.
*/
main()
{
    struct  stat	statbuf;

setup ("devtty");
funct ("tty");


testing ("/dev/tty should exist");
start
	expecting (SUCCESS);
	stat ("/dev/tty", &statbuf);

	if ((statbuf.st_mode & S_IFMT) != S_IFCHR)
		error ("/dev/tty is not a character special file");
finis


done();
/*NOTREACHED*/
}
