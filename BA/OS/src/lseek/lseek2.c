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

#ident	"@(#)lseek2.c	1.10"

# define FILENAME	"lseek.data"
# define min(a,b)	(((a)<(b))?(a):(b))
# define max(a,b)	(((a)>(b))?(a):(b))

int modes[] = {
	O_WRONLY | O_CREAT, 
	O_WRONLY | O_CREAT | O_APPEND, 
	O_RDWR | O_CREAT, 
	O_RDWR | O_CREAT | O_APPEND, 
};
#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** lseek2
**	Test the lseek function for new write files
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

				abstract("Lseek to current loc and beginning of new file opened for O_WRONLY from many points in file.");
				abstract("movement through file done by writing.");
				abstract("Expect a return value of current loc.");
				break;
			case 1:
				abstract("Lseek to current loc and beginning of new file opened for O_WRONLY | O_APPEND from many points in file.");
				abstract("movement through file done by writing.");
				abstract("Expect a return value of current loc.");
				break;
			case 2:
				abstract("Lseek to current loc and beginning of new file opened for O_RDWR from many points in file.");
				abstract("movement through file done by writing.");
				abstract("Expect a return value of current loc.");
				break;
			case 3:
				abstract("Lseek to current loc and beginning of new file opened for O_RDWR | O_APPEND from many points in file.");
				abstract("movement through file done by writing.");
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

			testing("Write data to the file and");
			testing("seek to various locations depending on the mode.");
			start
				int limit;
				limit = (!mode||mode==2)?FILESIZ:100;

#ifdef	SV_RFS
				for (i = 1; i <= limit; i += max(min(limit-i,BUFSIZ/17),1))
#else
				for ( i = 1; i <= limit; i++ )
#endif
				{
					expecting ( i );
					write(fd, &buf1[0], (unsigned)i);


					expecting ( VOID );
					loc = lseek(fd, 0L, SEEK_CUR);
					switch( mode ) {
					case 1:
					case 3:
						sum += i;
						if (loc != sum)
						{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", sum, loc);	/* ERROR */
							goto brkout;
						}
						break;
					case 0:
					case 2:
						if (loc != (long)i)
						{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", (long)i, loc);	/* ERROR */
							goto brkout;
						}
						break;
					}
					expecting ( 0L );
					loc = lseek( fd, 0L, SEEK_SET );
					if (loc != 0L)
					{
						goto brkout;
					}
				}
			finis
brkout:
			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				close( fd );
			finis

			if( mode == 0 || mode == 2 )
			{
				testing("Contents of file read match buffer written.");
				testing("matching contents.");
				get_file( path, buf2 );

				if ( cmp_file( buf1, buf2 ) != FILESIZ )
					error("File contents differ.");
			}
		}

		cleanup("Unlink path.");
		start
			expecting ( SUCCESS );
			unlink( path );
		finis

	finis


	done();
	/*NOTREACHED*/
}
