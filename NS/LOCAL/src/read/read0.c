#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

#include	<sys/types.h>
#include	<errno.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<fcntl.h>
/*	#include	<limits.h>	*/
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>

#ident	"@(#)read0.c	1.6"

# define FILENAME	"read.data"

typedef char *ptr;

int modes[] = {
	O_RDONLY, 
	O_RDWR, 
};
#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** read
**	Test the read function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		close(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
**		creat()
**		time()
*/
main()
{
	int		mode, nbytes;
	char		path[PATH_MAX];
	int		fd;
	int		ret_val;
	ttime		t;
	struct stat	st_buf1;
	struct stat	st_buf2;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ*2]; /* need extra room for eof test */

	ttime		time();
	void		init_buf();
	void		put_file();
	void		clr_buf();
	int		cmp_file();
	void		cmp_stat();
	long		lseek();
	char		*data_path();

	setup("read");

	funct("read");

	start
		prep("Init a buffer and file names.");
		start
			init_buf( buf1 );

			strcpy( path, data_path() );
			strcat( path, FILENAME );

			put_file( path, 0666, buf1 );
		finis

		for( mode = 0; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:

				abstract("Read of various combinations of nbytes from file opened for");
				abstract("O_RDONLY.");
				abstract("Return value of nbytes, file status unchanged except");
				abstract("for updated access time.");
				break;
			case 1:

				abstract("Read of various combinations of nbytes from file opened for");
				abstract("O_RDWR.");
				abstract("Return value of nbytes, file status unchanged except");
				abstract("for updated access time.");
				break;
			}

			testing("Stat the created file and wait a few seconds.");
			start
				expecting ( SUCCESS );
				stat( path, &st_buf1 );

				expecting ( SUCCESS );
				t = time((ttime *)0) + 2;

				expecting ( SUCCESS );
				while ( time((ttime *)0) < t );
			finis

			testing("Issue the call to open.");
			start
				expecting ( SUCCESS );
				fd = open( path, modes[mode] );
			finis

			testing("Verify the open by examining contents of file.");
			start
				for( nbytes = 0; nbytes <= FILESIZ; nbytes=newcnt(nbytes) )
				{
					clr_buf( &buf2[0], (nbytes+1) );

					expecting ( nbytes );
					read(fd, &buf2[0], (unsigned)nbytes);

					expecting ( (FILESIZ-nbytes) );
					read(fd, &buf2[nbytes], (unsigned)(FILESIZ-nbytes));

					if (cmp_file(buf1, buf2 ) != FILESIZ)
					{
						error("File contents differ.");
					}

					expecting ( VOID );
					ret_val= read(fd, &buf2[0], 1);
					if (ret_val != 0)
					{
						error("Read did not return 0 at EOF.\nRead returned %d.\n", ret_val);	/*  ERROR */
					}

					expecting ( 0L );
					lseek(fd, 0L, SEEK_SET);
				}
			finis

			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				stat( path, &st_buf2 );

				cmp_stat( &st_buf1, &st_buf2, ACCESS );

				expecting ( SUCCESS );
				close( fd );
			finis
		}

		cleanup("Unlink the file.");
		start
			expecting ( SUCCESS );
			unlink( path );
		finis

	finis

	testing("Read with bad file descriptor.");
	testing("Expect a return value (-1), and errno of EBADF.");
	start

		expecting ( EBADF );
		read(fd, &buf2[0], 1);

	finis

	done();
	/*NOTREACHED*/
}

newcnt(i)
int i;
{
	int k;
	int j = 0;

	while(i/10) {
		i/=10;
		j++;
	}
	if(!i) i = 1;
	else if(i < 2) i = 2;
	else if (i < 5) i = 5;
	else if (i <= 9) i = 1;
	if(i==1) j++;
	for(k=0;k<j;k++) i *= 10;
	return(i);
}
