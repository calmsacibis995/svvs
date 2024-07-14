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

#ident	"@(#)ulimit0.c	1.7"

# define	PROT	0777
# define	NOARG	0L
# define	GET_LIMIT	01
# define	SET_LIMIT	02

# define	SMALL_PROC_LIMIT	20

# define	ULIMBLK		512	/* ulimit block size */

/*
** ulimit
**	Test the ulimit function.
**
*/

# define	NEAR_TO_END	23

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
	int	fstat();
	int	unlink();
	int	close();


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

		fildes = temp_creat("ulimit_1", PROT);

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

	testing("A simple user should not be able to raise his size limit.");
	testing("Expect a return value of -1 and an errno of EPERM.");
	start

		expecting( EPERM );
		ulimit(SET_LIMIT, (long)(file_limit+1));

	finis

	testing("Reset process file limit to a smaller value and remember for next subtest.");
	testing("Expect a return value of the new limit.");
	start
		expecting( SUCCESS );
		file_limit = ulimit(SET_LIMIT, (long)(file_limit - SMALL_PROC_LIMIT));

	finis

	testing("Writing a file of what the maximum process limit should be.");
	testing("Expect a return value of zero.");
	start

		fildes = temp_creat("ulimit_1", PROT);

		end_of_loop = file_limit;
		if (file_limit > NEAR_TO_END)
		{
			near_to_end=(long)ULIMBLK*((long)file_limit-(long)NEAR_TO_END);
			expecting ( VOID );
			if((ret_val=lseek(fildes, near_to_end, 0)) != near_to_end)
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

	testing("A simple user should not be able to raise his new size limit.");
	testing("Expect a return value of -1 and an errno of EPERM.");
	start

		expecting( EPERM );
		ulimit(SET_LIMIT, (long)(file_limit+1));


	finis
	done();
	/*NOTREACHED*/
}
