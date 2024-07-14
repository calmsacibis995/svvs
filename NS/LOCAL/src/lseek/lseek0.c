#include <sv_base.h> /*replacement macros */

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

#ifdef	LOCKF
#include	<unistd.h>
#endif

#ident	"@(#)lseek0.c	1.9"

# define FILENAME	"lseek.data"
# define min(a,b)	(((a)<(b))?(a):(b))
# define max(a,b)	(((a)>(b))?(a):(b))

int modes[] = {
	O_RDONLY, 
	O_RDWR, 
	O_RDWR | O_APPEND, 
};
#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** lseek0
**	Test the lseek function for read files
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		open()
**		read()
**		close()
*/
main()
{
	int		i;
	int		mode;
	long		loc;
	char		path[PATH_MAX];
	int		fd;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ+1];

	void		init_buf();
	void		put_file();
	void		get_file();
	void		clr_buf();
	int		cmp_file();
	long		lseek();
	char		*data_path();

	setup("lseek");
	funct("lseek");

	exit_on ( ERROR );

	start

		prep("Init a buffer and file names.");
		start
			init_buf( buf1 );

			strcpy( path, data_path() );
			strcat( path, FILENAME );
		finis

		prep("Write data to a file for reading.");
		start

			put_file( path, 0666, buf1 );
		finis

		for( mode = 0; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:

				abstract("Lseek of existing file opened for O_RDONLY at many points in file.");
				abstract("Determine movement through file done by reading.");
				abstract("Expect a return value of current loc.");
				break;
			case 1:


				abstract("Lseek to current loc and beginning of existing file opened for O_RDWR from many points in file.");
				abstract("Determine movement through file done by reading.");
				abstract("Expect a return value of current loc.");
				break;
			case 2:

				abstract("Lseek to current loc and beginning of existing file opened for O_RDONLY | O_APPEND");
				abstract("from many points in file.");
				abstract("Determine movement through file done by reading.");
				abstract("Expect a return value of current loc.");

				break;
			}

			prep("Open the file and seek to the beginning.");
			start

				expecting ( SUCCESS );
				fd = open( path, modes[mode] );

				expecting ( 0L );
				loc = lseek( fd, 0l, SEEK_SET);
			finis

			testing("Seek to various positions and read in data.");
			start
#ifdef	SV_RFS
				for (i = 1; i <= FILESIZ; i += max(min(FILESIZ-i,BUFSIZ/17),1))
#else
				for (i = 1; i <= FILESIZ; i++)
#endif
				{
					expecting ( 0L );
					loc = lseek( fd, 0l, SEEK_CUR);
					if(loc != 0L)
					{
						break;
					}
					clr_buf ( buf2, i+1 );

					expecting ( i );
					read(fd, &buf2[0], (unsigned)i);


					expecting ( (long)i );
					loc = lseek( fd, 0l, SEEK_CUR);

					if (loc != (long)i)
					{
						break;
					}

					expecting ( 0L );
					loc = lseek( fd, 0l, SEEK_SET);

					if (loc != 0L)
					{
						break;
					}

				}

			finis

			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				close( fd );
			finis

			testing("Contents of file read in match the buffer written.");
			if ( cmp_file( buf1, buf2 ) != FILESIZ )
				error("File contents differ.");
		}
	finis

	done();
	/*NOTREACHED*/
}
