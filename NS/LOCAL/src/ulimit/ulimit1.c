#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#include	<sys/types.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)ulimit1.c	1.7"

# define	PROT	0777
# define	NOARG	00
# define	GET_LIMIT	01
# define	SET_LIMIT	02

# define	ULIMBLK		512	/* ulimit block size */
# define	NEAR_TO_END	23

/*
** ulimit
**	Test the ulimit function.
**
** This test must be run as setuid root.
**
*/

main()
{
	long	ret_val;
	long	near_to_end;
	int	end_of_loop;

	char	buffer[ULIMBLK];
	int	fildes;
	int	file_limit;
	int	i;
	long	ulimit();
	long	lseek();


	setup("ulimit");

	funct("ulimit");

	exit_on ( ERROR );

	testing("Get process file upper limit and remember for next subtest.");
	testing("Expect a return value of greater than zero.");
	start
		expecting( (long) PROC_LIMIT );
		file_limit = ulimit(GET_LIMIT, (long)NOARG);
	finis

	testing("Writing a file of what the maximum process limit should be.");
	testing("Expect a return value of zero.");
	start

		fildes = temp_creat("ulimit_2", PROT);

		end_of_loop = file_limit;

		if (file_limit > NEAR_TO_END)
		{
			near_to_end=(long)ULIMBLK*((long)file_limit-(long)NEAR_TO_END);
			expecting ( VOID );
			if((ret_val=lseek(fildes, near_to_end, SEEK_SET)) != near_to_end)
			{
				severr("Cannot seek near to end of file.\nThe expected location was %l.\nThe actual location was %l.\n", near_to_end, ret_val);	/* HALT */
			}
			end_of_loop = NEAR_TO_END;
		}

		for(i=0; i < end_of_loop; i++)
		{
			expecting( ULIMBLK );
			write(fildes, buffer, ULIMBLK);
		}

	finis

	testing("Root should be able to raise his size limit.");
	testing("Expect a return value of current limit plus 1.");
	start

		expecting( (long) (file_limit + 1) );
		ulimit(SET_LIMIT, (long)(file_limit+1));

	finis

	testing("Writing a file of what the maximum process limit should be.");
	testing("Expect a return value of zero.");
	start

		fildes = temp_creat("ulimit_2", PROT);

		end_of_loop = file_limit;
		if (file_limit > NEAR_TO_END)
		{
			near_to_end=(long)ULIMBLK*((long)file_limit-(long)NEAR_TO_END);
			expecting ( VOID );
			if((ret_val=lseek(fildes, near_to_end, SEEK_SET)) != near_to_end)
			{
				severr("Cannot seek near to end of file.\nThe expected location was %l.\nThe actual location was %l.\n", near_to_end, ret_val);	/* HALT */
			}
			end_of_loop = NEAR_TO_END;
		}

		for(i=0; i < end_of_loop; i++)
		{
			expecting( ULIMBLK );
			write(fildes, buffer, ULIMBLK);

		}

	finis
	done();
	/*NOTREACHED*/
}
