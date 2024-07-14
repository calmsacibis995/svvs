#include	<sv_base.h> /*replacement macros */
#include	<fcntl.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>



long   lseek();

#ident	"@(#)devnul.c	1.3"



/*
** devices
**      Check that the /dev/null file is in fact a null file.
*/
main()
{
    int     fdes;
    char    read_buff[10];

setup ("devnul");
funct ("null");


testing ("open /dev/null for read/write");
start
	expecting (SUCCESS);
	break_on (ERROR | SEVERR);
	fdes = open ("/dev/null", O_RDWR);

	testing ("Zero bytes should be read from /dev/null after 10 bytes were written");
	expecting (10);
	break_on (ERROR | SEVERR);
	write (fdes, "1234567890", 10);
	(void)  lseek(fdes, 0L, 0);

	expecting (SUCCESS);
        read (fdes, read_buff,10);
	close(fdes);
finis

done();
/*NOTREACHED*/
}
