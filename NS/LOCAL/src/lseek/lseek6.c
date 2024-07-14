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

#ident	"@(#)lseek6.c	1.9"

# define HASHFILE	"hlist"
# define SRCFILE	"hdata"
# define FILENAME	"lseek.data"
# define INITSIZ	(BUFSIZ*3/2+17)
#ifdef SV_RFS
# define FFILESIZ 	100
#else
# define FFILESIZ 	(1024*47/2+23)	/* do not change without	*/
#endif
/* rebuilding hdata and hlist	*/

int modes[] = {
	O_WRONLY, 
	O_WRONLY | O_APPEND, 
	O_RDWR, 
	O_RDWR | O_APPEND, 
};

#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** lseek6
**	Test the lseek function for old write files
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		open()
**		read()
**		write()
**		close()
**		creat()
*/
main()
{
	int		i;
	int		mode;
	char		path[PATH_MAX];
	int		fd;
	int		dd;
	long		curloc;
	long		newloc;
	char		buf[2];
	char		where[2];
	char		d1, d2;

	long		lseek();
	void		put_file();
	char		*data_path();

	setup("lseek");
	funct("lseek");

	exit_on ( ERROR );

	start

		for( mode = 0; mode < NMODES; mode++ )
		{

			switch (mode) {

			case 0:

				abstract("Lseek of existing file opened for O_WRONLY");
				abstract("at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 1:
				abstract("Lseek of existing file opened for O_WRONLY | O_APPEND.");
				abstract("at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 2:

				abstract("Lseek of existing file opened for O_RDWR");
				abstract("at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 3:

				abstract("Lseek of existing file opened for O_RDWR | O_APPEND");
				abstract("at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			}

			prep("Init file names and open the files.");
			start
				strcpy( path, data_path() );
				strcat( path, HASHFILE );

				expecting ( SUCCESS );
				dd = open( path, O_RDONLY );

				strcpy( path, data_path() );
				strcat( path, FILENAME );

				put_file( path, 0666 );

				expecting ( SUCCESS );
				fd = open( path, modes[mode] );

				curloc = 0L;

			finis


			testing("Seek, read, and compare positions in both files.");
			start
				for (i = 0; i < FFILESIZ; i++)
				{
					where[0] = where[1] = '\0';

					expecting ( 2 );
					read(dd, &where[0], 2);

					newloc = (long)(((int)where[0]<<8) + ((int)where[1]&0x00ff));
					expecting ( VOID );
					curloc = lseek(fd, newloc, SEEK_SET);

					if (curloc != newloc)
					{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", newloc, curloc);	/* ERROR */
						break;
					}

					drs( ((int)newloc & 0xffff), &d1, &d2);

					expecting ( 1 );
					write(fd, &d1, 1);
				}

			finis

			expecting ( 0L );
			curloc = lseek(dd, 0l, SEEK_SET);
			if (curloc != 0L)
			{
				break;
			}

			if (mode == 0 || mode == 1)
			{
				expecting ( SUCCESS );
				close( fd );

				expecting ( SUCCESS );
				fd = open( path, O_RDONLY );
			}
			else
			{
				expecting ( 0L );
				curloc = lseek(fd, 0L, SEEK_SET);

				if (curloc != 0L)
				{
					break;
				}
			}
			curloc = 0L;
			if (mode ==  1 || mode == 3)
			{
				testing("unchanged existing contents of file, new contents match expected");
				testing("values");
				testing("matching contents.");

				for (i = 0; i < INITSIZ; i++)
				{
					expecting ( 1 );
					read(fd, &buf[0], 1);

					drs( i, &d1, &d2);

					if (buf[0] != d1)
					{
						error("File contents of altered.");
						break;
					}
				}
				for (i = 0; i < FFILESIZ; i++)
				{
					where[0] = where[1] = '\0';

					expecting ( 2 );
					read(dd, &where[0], 2);

					newloc = (long)(((int)where[0]<<8) + ((int)where[1]&0x0ff));
					drs( ((int)newloc & 0xffff), &d1, &d2);

					expecting ( 1 );
					read(fd, &buf[0], 1);

					if (buf[0] != d1)
					{
						error("Unexpected file contents.");
						break;
					}
				}
			}
			else
			{
				testing("File contents match expected values.");
				start
				for (i = 0; i < FFILESIZ; i++)
				{
					drs( ((int)i & 0xffff), &d1, &d2);

					expecting ( 1 );
					read(fd, &buf[0], 1);

					if (buf[0] != d1)
					{
						error("Unexpected file contents.");
						break;
					}
				}
				finis
			}

			cleanup("Close both files.");
			start
				expecting ( SUCCESS );
				close( fd );

				expecting ( SUCCESS );
				close( dd );
			finis
		}
	finis

	done();
	/*NOTREACHED*/
}
void put_file( file, mode ) /* This routine is slightly different than the one in sv_tools */
char *file;
int mode;
{
	int		fd;
	int		i;
	char		d1, d2;

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	expecting ( VOID );
	(void) unlink( file );
	errno = 0;

	expecting ( VOID );
	fd = creat( file, mode );
	if ( fd < 0 )
	{
		severr( "creat failed" );
	}


	for ( i = 0; i < INITSIZ ; i++ )
	{
		drs ( i, &d1, &d2 );
		expecting ( VOID );
		if ( write( fd, &d1, 1 ) != 1 )
		{
			expecting ( VOID );
			(void) close( fd );
			expecting ( VOID );
			(void) unlink( file );
			{
				severr( "write failed" );
			}
			break;
		}
	}
	expecting ( VOID );
	if ( close( fd ) != 0 )
	{
		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "close failed" );
		}
	}
}
drs(n, d1, d2)
int n;
char *d1, *d2;
{
	int i, sum, degree, offset;

	n++;

	for(sum=0, i=0;(sum+1+(2*i))<n;i++)
		sum += 2*i+1;

	degree = i;
	offset = n-sum-1;

	if(offset == degree*2)
	{
		*d1 = degree;
		*d2 = 0;
	}
	else {
		if(offset%2)
		{
			*d1 = degree;
			*d2 = (offset+1)/2;
		}
		else 
		{
			*d1 = (offset)/2;
			*d2 = degree;
		}
	}
}
