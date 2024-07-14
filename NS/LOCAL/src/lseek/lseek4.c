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

#ident	"@(#)lseek4.c	1.9"

# define FILENAME	"lseek.data"
# define min(a,b)	(((a)<(b))?(a):(b))
# define max(a,b)	(((a)>(b))?(a):(b))


int modes[] = {
	O_RDWR | O_CREAT, 
};
#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** lseek
**	Test the lseek function for existing update files
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		open()
**		read()
**		write()
**		close()
*/
main()
{
	int		i;
	long		sum;
	int		mode;
	char		path[PATH_MAX];
	int		fd;
	long		loc;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ+1];

	void		init_buf();
	void		get_file();
	int		cmp_file();
	long		lseek();
	char		*data_path();

	setup("lseek");
	funct("lseek");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	start

		prep("Init a buffer and file names.");
		start
			init_buf( buf1 );

			strcpy( path, data_path() );
			strcat( path, FILENAME );
		finis

		for( mode = 0; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:

				abstract("Lseek to beginning of new file opened for O_RDWR from many points in file.");
				abstract("movement through file done by reading and writing.");
				abstract("Expect a return value of current loc.");
				break;
			case 1:
				abstract("movement through file done by reading and writing.");
				abstract("Expect a return value of current loc.");
				break;
			}

			prep("Unlink path and open it.");
			start
				expecting ( VOID );
				(void) unlink( path );

				expecting ( SUCCESS );
				fd = open( path, modes[mode], 0664 );

				sum = 0L;
			finis

			testing("Read, write and seek to various locations in the file.");
			start
				int limit;
				limit = (!mode)?FILESIZ:100;
#ifdef	SV_RFS
				for (i = 1; i <= limit; i += max(min(limit-i,BUFSIZ/17),1))
#else
				for ( i = 1; i <= limit; i++ )
#endif
				{
					expecting ( 0L );
					loc = lseek(fd, 0L, SEEK_SET);
					if (loc != 0L)
					{
						break;
					}

					expecting ( i );
					write(fd, &buf1[0], (unsigned)i);

					expecting ( VOID );
					loc = lseek(fd, 0L, SEEK_CUR);
					switch( mode ) {
					case 1:
						sum += i;
						if (loc != sum)
						{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", sum, loc);	/* ERROR */
							goto brkout;
						}
						break;
					case 0:
						if (loc != (long)i)
						{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", (long)i, loc);	/* ERROR */
							goto brkout;
						}
						break;
					}

					expecting ( 0L );
					loc = lseek(fd, 0L, SEEK_SET);
					if (loc != 0L)
					{
						break;
					}

					expecting ( i );
					read(fd, &buf2[0], (unsigned)i);

					expecting ( VOID );
					loc = lseek(fd, 0L, SEEK_CUR);
					if (loc != (long)i)
					{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", (long)i, loc);	/* ERROR */
						break;
					}
				}
			finis
brkout:

			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				close( fd );
			finis

			if( mode == 0 )
			{
				testing("Contents of file read match buffer written.");
				start
					get_file( path, buf2 );

					if ( cmp_file( buf1, buf2 ) != FILESIZ )
						error("File contents differ.");
				finis
			}
		}

		cleanup("Unlink the file.");
		start
			unlink( path );
		finis

	finis

	done();
	/*NOTREACHED*/
}
