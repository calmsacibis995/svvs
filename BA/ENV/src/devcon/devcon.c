#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)devcon.c	1.3"


/*
**	Test for the presence of /dev/console.
*/
main()
{
struct  stat	statbuf;

setup ("devcon");
funct ("console");


testing ("/dev/console should exist");
start
	expecting (SUCCESS);
	stat ("/dev/console", &statbuf);

	if ((statbuf.st_mode & S_IFMT) != S_IFCHR)
		error ("/dev/console is not a character special file");
finis


done();
/*NOTREACHED*/
}
