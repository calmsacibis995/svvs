#include	<sv_base.h> /*replacement macros */
#include	<sys/types.h>
#include	<signal.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<errno.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

# define	COE_FLAG_ON	0x1
# define	WAIT		5

#ident	"@(#)fcntl0.c	1.8"


extern	int	handler();

/*
** fcntl()
**	 This is the test program for the OS routine fcntl.
*/
main()
{
	int		fildes1;
	int		fildes2;
	int		dupfildes;
	int		clsonx1;
	int		clsonx2;
	int		dummyarg = 0;
	int		i;
	int		fileflag;
	struct	stat	buf1;
	struct	stat	*buf1ptr = &buf1;
	struct	stat	buf2;
	struct	stat	*buf2ptr = &buf2;

	setup("fcntl");
	funct("fcntl");

	abstract("Using a cmd of F_DUPFD on an open file descriptor and an arg");
	abstract("equal to that of a closed descriptor");
	abstract("should return arg for the open file above and ");
	abstract("the same access mode, file status, and close on exec flag.");
	start

		prep("Open a temp file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_RDWR|O_TRUNC, 0777);
		finis

		testing("Issue a call to fcntl to GETFD.");
		start
			fstat(fildes1, buf1ptr);

			expecting( 0 );	/* expecting no close on exec */
			/* only lsb matters */
			clsonx1 = fcntl(fildes1, F_GETFD, dummyarg);
		finis

		prep("Open/create a second temp file and close it.");
		start
			expecting ( SUCCESS );
			fildes2 = temp_open("fcntlrandom", O_CREAT|O_RDWR|O_TRUNC, 0666);

			close(fildes2);
		finis

		testing("Issue a call to fcntl of F_DUPFD.");
		start
			expecting ( fildes2 );
			dupfildes = fcntl(fildes1, F_DUPFD, fildes2);
		finis

		testing("Call fstat and examine file modes.");
		start
			expecting ( SUCCESS );
			fstat(dupfildes, buf2ptr);
			expecting( 0 ); /* no close on exec */
			clsonx2 = fcntl(dupfildes, F_GETFD, dummyarg);


			if ( buf2ptr->st_mode != buf1ptr->st_mode )
				error("File status item st_mode is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_ino != buf1ptr->st_ino )
				error("File status item st_ino is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_dev != buf1ptr->st_dev )
				error("File status item st_dev is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_rdev != buf1ptr->st_rdev )
				error("File status item st_rdev is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_nlink != buf1ptr->st_nlink )
				error("File status item st_nlink is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_uid != buf1ptr->st_uid )
				error("File status item st_uid is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_gid != buf1ptr->st_gid )
				error("File status item st_gid is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_size != buf1ptr->st_size )
				error("File status item st_size is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_atime != buf1ptr->st_atime )
				error("File status item st_atime is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_mtime != buf1ptr->st_mtime )
				error("File status item st_mtime is not \
the same for the two fildes's pointing to the same file.");
			if ( buf2ptr->st_ctime != buf1ptr->st_ctime )
				error("File status item st_ctime is not \
the same for the two fildes's pointing to the same file.");

			if ( (clsonx1 & 0x1) != (clsonx2 & 0x1) )
				error("%s\nexpected value was %d actual \
value was %d:",
"Close_on_exec for the file is not same for its different file descriptors.",
(clsonx1 & 0x1), (clsonx2 &0x1));
		finis

		cleanup("Close open files.");
		start
			expecting (  SUCCESS );
			close(fildes1);
			close(dupfildes);
		finis

	finis

	abstract("Use a cmd of F_DUPFD on file descriptor with the");
	abstract("close on exec flag on.  Verify a return descriptor");
	abstract("with the close on exec flag off.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1",
				O_CREAT|O_RDWR|O_TRUNC, 0666);
		finis

		testing("Issue calls to fcntl to duplicate the file descriptor.");
		testing("After the calls we examine the results.");

		start
			clsonx1 = COE_FLAG_ON;

			fcntl(fildes1, F_SETFD, clsonx1);
			dupfildes = fcntl(fildes1, F_DUPFD, 2);
			clsonx2 = fcntl(dupfildes, F_GETFD, dummyarg);

			if ( (clsonx2 & 0x1) != 0x0 )
			{
				error("Close_on_exec for the DUP file \
descriptor should be off.\nexpected value was %d actual value was %d:\n",
0, (clsonx2 &0x1));	/* ERROR */
			}
		finis

		cleanup("Close all open files.");
		start
			close(dupfildes);
			close(fildes1);
		finis

	finis

	abstract("Using a cmd of F_DUPFD on an invalid descriptor");
	abstract("should return -1 with an errno of EBADF.");
	start
		expecting ( EBADF );
		dupfildes = fcntl(BAD_FILDES, F_DUPFD, 2);

	finis

	abstract("Use a cmd of F_DUPFD on a valid descriptor but after all available");
	abstract("file descriptors have been used up");
	abstract("should return -1 with an errno of EMFILE.");
	start
		prep("Consume all open file descriptors.");
		start
			expecting ( SUCCESS );
			for ( i = 3; i < OPEN_MAX; i++ )
			{
				temp_open("fcntl1", O_RDONLY);
			}
		finis

		testing("Issue the call to fcntl.");
		start
			expecting ( EMFILE );
			dupfildes = fcntl(fildes1, F_DUPFD, 2);
		finis

		cleanup("Close all open file descriptors.");
		start
			for ( i = 3; i < OPEN_MAX; i++ )
			{
				expecting ( SUCCESS );
				close(i);
			}
		finis
	finis

	abstract("Using a cmd of F_DUPFD on a valid descriptor but with");
	abstract("set arg to -1 should return -1 with an errno of EINVAL.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_RDONLY);
		finis

		testing("Issue the call to fcntl.");
		start
			expecting ( EINVAL );
			dupfildes = fcntl(fildes1, F_DUPFD, -1);
		finis

		cleanup("Close the open file.");
		start
			expecting ( SUCCESS );
			close(fildes1);
		finis

	finis

	abstract("Using a cmd of F_DUPFD on a valid descriptor but with");
	abstract("arg set to OPEN_MAX should return -1 with an errno of EINVAL.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_RDONLY);
		finis

		testing("Issue the call to fcntl.");
		start
			expecting ( EINVAL );
			dupfildes = fcntl(fildes1, F_DUPFD, OPEN_MAX);
		finis

		cleanup("Close the open file.");
		start
			expecting ( SUCCESS );
			close(fildes1);
		finis
	finis

	abstract("Using a cmd of F_GETFD on file descriptor with the");
	abstract("close on exec flag off should return a close on exec flag of zero.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_RDWR|O_TRUNC, 0666);
		finis

		testing("Issue the call to fcntl.");
		start
			clsonx1 = fcntl(fildes1, F_GETFD, dummyarg);
			if ( (clsonx1 & 0x1) != 0x0 )
			{
				error("Close_on_exec for the file descriptor \
	should be off.\nexpected value was %d actual value was %d:\n",
	 0, (clsonx1 &0x1));	/* ERROR */
			}
		finis

		cleanup("Close the open file.");
		start
			close(fildes1);
		finis

	finis

	abstract("Using a cmd of F_GETFD on file descriptor with the");
	abstract("close on exec flag on should return a close on exec flag of one.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_CREAT|O_RDWR|O_TRUNC, 0666);
		finis

		testing("Issue the call to fcntl.");
		start
			clsonx1 = COE_FLAG_ON;
			fcntl(fildes1, F_SETFD, clsonx1);
			clsonx2 = fcntl(fildes1, F_GETFD, dummyarg);

			if ( (clsonx2 & 0x1) != 0x1 )
			{
				error("Close_on_exec for the file descriptor \
		should be off.\nexpected value was %d actual value was %d:\n",
		1, (clsonx2 &0x1));	/* ERROR */
			}
		finis

		cleanup("Close the open file.");
		start
			close(fildes1);
		finis

	finis

	abstract("Using a cmd of F_SETFD on an invalid descriptor");
	abstract("should return -1 with an errno of EBADF.");
	start
		expecting ( EBADF );
		dupfildes = fcntl(BAD_FILDES, F_SETFD, 2);

	finis

	for (i=0 ; i<10 ; i++)
	{

	abstract("Using a cmd of F_GETFL on file descriptor opened for");
	switch (i)
	  {
	  case 0:
		abstract ("O_RDONLY should return O_RDONLY.");
		fileflag = O_RDONLY;
		break;

	  case 1:
		abstract ("O_WRONLY should return O_WRONLY.");
		fileflag = O_WRONLY;
		break;

	  case 2:
		abstract ("O_RDWR should return O_RDWR.");
		fileflag = O_RDWR;
		break;

	  case 3:
		abstract ("O_RDONLY | O_NDELAY should return O_RDONLY | O_NDELAY.");
		fileflag = O_RDONLY | O_NDELAY;
		break;

	  case 4:
		abstract ("O_WRONLY | O_NDELAY should return O_WRONLY | O_NDELAY.");
		fileflag = O_WRONLY | O_NDELAY;
		break;

	  case 5:
		abstract ("O_RDWR | O_NDELAY should return O_RDWR | O_NDELAY.");
		fileflag = O_RDWR | O_NDELAY;
		break;

	  case 6:
		abstract ("O_WRONLY | O_APPEND should return O_WRONLY | O_APPEND.");
		fileflag = O_WRONLY | O_APPEND;
		break;

	  case 7:
		abstract ("O_RDWR | O_APPEND should return O_RDWR | O_APPEND.");
		fileflag = O_RDWR | O_APPEND;
		break;

	  case 8:
		abstract ("O_WRONLY | O_APPEND | O_NDELAY should return \
O_WRONLY | O_APPEND | O_NDELAY.");
		fileflag = O_WRONLY | O_APPEND | O_NDELAY;
		break;

	  case 9:
		abstract ("O_RDWR | O_APPEND | O_NDELAY should return \
O_RDWR | O_APPEND | O_NDELAY.");
		fileflag = O_RDWR | O_APPEND | O_NDELAY;
		break;
	  }

	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", fileflag);
		finis

		testing("Issue the call to fcntl.");
		start
			expecting ( fileflag);
			fcntl(fildes1, F_GETFL, dummyarg);
		finis

		cleanup("Close the open file.");
		start
			close(fildes1);
		finis

	finis
	}  /*  end the while loop  */



	abstract("Using a cmd of F_GETFL on an invalid descriptor");
	abstract("should return -1 with an errno of EBADF.");
	start
		expecting ( EBADF );
		dupfildes = fcntl(BAD_FILDES, F_GETFL, 2);
	finis



	abstract("Using a cmd of F_SETFL of O_WRONLY on file opened for O_RDONLY");
	abstract("should return O_RDONLY.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_RDONLY);
		finis

		testing("Issue the call to fcntl for SETFL.");
		start
			expecting ( SUCCESS );
			fcntl(fildes1, F_SETFL, O_WRONLY);
		finis

		testing("Issue the call to fcntl for GETFL.");
		start
			expecting ( O_RDONLY );
			fcntl(fildes1, F_GETFL, dummyarg);
		finis

		cleanup("Close the open file.");
		start
			close(fildes1);
		finis

	finis

	abstract("Using a cmd of F_SETFL of O_RDONLY on file opened for O_WRONLY");
	abstract("should return O_WRONLY.");
	start
		prep("Open a temporary file.");
		start
			expecting ( SUCCESS );
			fildes1 = temp_open("fcntl1", O_WRONLY);
		finis

		testing("Issue the call to fcntl for SETFL.");
		start
			expecting ( SUCCESS );
			fcntl(fildes1, F_SETFL, O_RDONLY);
		finis

		testing("Issue the call to fcntl for GETFL.");
		start
			expecting ( O_WRONLY );
			fcntl(fildes1, F_GETFL, dummyarg);
		finis

		cleanup("Close the open file.");
		start
			close(fildes1);
		finis

	finis

	abstract("Using a cmd of F_SETFL on an invalid descriptor");
	abstract("should return -1 with an errno of EBADF.");
	start
		expecting ( EBADF );
		dupfildes = fcntl(BAD_FILDES, F_SETFL, 2);

	finis
	done();
}
