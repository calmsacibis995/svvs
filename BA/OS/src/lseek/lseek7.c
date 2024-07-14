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

#ident	"@(#)lseek7.c	1.10"

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
	O_WRONLY | O_CREAT, 
	O_WRONLY | O_CREAT | O_APPEND, 
	O_RDWR | O_CREAT, 
	O_RDWR | O_CREAT | O_APPEND, 
};

#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** lseek7
**	Test the lseek function for new write files
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
	int		ret_val;
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

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	exit_on ( ERROR );

	start

		for( mode = 0; mode < NMODES; mode++ )
		{

			switch (mode) {

			case 0:

				abstract("Lseek of new file opened for O_WRONLY at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 1:

				abstract("Lseek of new file opened for O_WRONLY | O_APPEND at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 2:

				abstract("Lseek of new file opened for O_RDWR at many points in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 3:
				abstract("Lseek of new file opened for O_RDWR | O_APPEND at many points in file.");
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

				expecting ( VOID );
				(void) unlink(path);

				expecting ( SUCCESS );
				fd = open( path, modes[mode], 0664 );

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
			lseek(dd, 0L, SEEK_SET);

			if( mode == 0 || mode == 1)
			{
				expecting ( SUCCESS );
				close( fd );

				expecting ( SUCCESS );
				fd = open( path, O_RDONLY );
			}
			else
			{
				break_on ( ERROR );
				expecting ( 0L );
				lseek(fd, 0L, SEEK_SET);
			}

			curloc = 0L;

			testing("File contents match expected values.");
			start
			if (mode == 1 || mode == 3)
			{
				for (i = 0; i < FFILESIZ; i++)
				{
					where[0] = where[1] = '\0';

					expecting ( 2 );
					read(dd, &where[0], 2);

					newloc = (long)(((int)where[0]<<8) + ((int)where[1]&0xff));
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
#ifndef SV_RFS
			else
			{
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
			}
#endif
			finis

			cleanup("Close the files.");
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
