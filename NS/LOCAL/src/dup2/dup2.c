#ident	"@(#)dup2.c	1.3"

#include	<sv_base.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ifndef	SYSVR_2  /*  dup2 was added in SVR3  */


main()
{
char	bigbuf [PATH_MAX + 9];
int	i;
struct	stat	stat_buf1, stat_buf2;

setup ("dup2");
funct ("dup2");

testing ("dup2 with invalid arguments should return -1 with errno set to EBADF");
start
	expecting (EBADF);
	dup2 (-3, 2);		/*  first arg is invalid  */

	expecting (EBADF);
	dup2 (5, 2);		/*  fd 5 is not open  */

	expecting (EBADF);
	dup2 (OPEN_MAX, 8);	/*  first arg is invalid  */

	expecting (EBADF);
	dup2 (1, -1);		/*  second arg is invalid  */

	expecting (EBADF);
	dup2 (1, OPEN_MAX);	/*  second arg is invalid  */
finis


testing ("dup2'ing files to MAX_OPEN should succeed");
start
	expecting (SUCCESS);
	for (i=3 ; i<OPEN_MAX ; i++)
		{
		dup2 (i-1, i);
		fstat (i-1, &stat_buf1);
		fstat (i, &stat_buf2);
		if ( stat_buf1.st_ino != stat_buf2.st_ino)
			error ("fstat reports different inodes");

		if ( stat_buf1.st_mode != stat_buf2.st_mode)
			error ("fstat reports different modes");
		}

finis


testing ("duping a file with a full file table should succeed.");
start
	expecting (SUCCESS);
	dup2 (5, 6);
finis



done ();
/*NOTREACHED*/
}


#else

main()
{
unsupported("dup2 is not a supported feature in SVR2");
}

#endif
