#include <sv_base.h> /*replacement macros */

#include	<sys/signal.h>  /*   need definition of SIGSYS  */
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

#ident	"@(#)lseek5.c	1.10"

# define FFILESIZ 	(1024*47/2+23)	/* do not change without	*/
/* rebuilding hdata and hlist	*/
# define HASHFILE	"hlist"
# define SRCFILE	"hdata"

int modes[] = {
	O_RDONLY, 
	O_RDWR, 
	O_RDWR | O_APPEND, 
};

#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** lseek5
**	Test the lseek function for read files
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
	int		mode;
	char		path[PATH_MAX];
	int		fd;
	int		dd;
	long		curloc;
	long		newloc;
	char		buf[2];
	char		where[2];
	char		d1, d2;
	int		b, d;
	int		pipedes [2];

	long		lseek();
	char		*data_path();

	siggy	dummy ();

	setup("lseek");
	funct("lseek");

	exit_on ( ERROR );

	start

		for( mode = 0; mode < NMODES; mode++ )
		{

			switch (mode) {

			case 0:

				abstract("Lseek of existing file opened for O_RDONLY.");
				abstract("at many points in file referenced from start of file.");
				abstract("Expect a return value of current loc.");
				break;
			case 1:

				abstract("Lseek of existing file opened for O_RDWR");
				abstract("at many points in file referenced from current loc in file.");
				abstract("Expect a return value of current loc.");
				break;
			case 2:

				abstract("Lseek of existing file opened for O_RDWR | O_APPEND");
				abstract("at many points in file referenced from end of file.");
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
				strcat( path, SRCFILE );

				expecting ( SUCCESS );
				fd = open( path, modes[mode] );

				curloc = 0L;
			finis

			testing("Seek, read, and compare positions in both files.");
			start
#ifdef SV_RFS
				for (i = 0; i <= 100; i++)
#else
				for (i = 0; i <= FFILESIZ; i++)
#endif
				{
					if (i < FFILESIZ)
					{
						where[0] = where[1] = '\0';

						expecting ( 2 );
						read(dd, &where[0], 2);

						newloc = (long)(((int)where[0]<<8) + ((int)where[1]&0x00ff));
					}
					else
						newloc = (long)FFILESIZ;
					switch (mode) {
					case 0:
						expecting ( VOID );
						curloc = lseek(fd, newloc, SEEK_SET);
						break;
					case 1:
						expecting ( VOID );
						curloc = lseek(fd, newloc-curloc, SEEK_CUR);
						break;
					case 2:
						expecting ( VOID );
						curloc = lseek(fd, newloc-(long)FFILESIZ, SEEK_END);
						break;
					}

					if (curloc != newloc)
					{
							error("lseek to current loc did not return current loc.\nExpected loc was %l.\nActual loc was %l.\n", newloc, curloc);	/* ERROR */
						break;
					}
					buf[0] = buf[1] = '\0';

					if (newloc < (long)FFILESIZ-1L)
					{
						curloc += 2L;
						expecting ( 2 );
						read(fd, &buf[0], 2);
					}
					else if (newloc < (long)FFILESIZ)
					{
						curloc++;
						expecting ( 1 );
						read(fd, &buf[0], 1);
					}
					if (newloc < (long)FFILESIZ)
					{
						drs( ((int)newloc & 0xffff), &d1, &d2);
						if (newloc == (long)FFILESIZ-1L) d2 = '\0';
						d = (((int)d1) << 8) + (int)d2;
						b = (((int)buf[0]) << 8) + (int)buf[1];
						if (d != b)
						{
							error("File content did not match expected index.\nThe expected index value was %d.\nThe actual index value was %d.\n", d, b);	/* ERROR */
							break;
						}
					}
				}

			finis


			cleanup("Close both files.");
			start
				expecting ( SUCCESS );
				close( fd );

				expecting ( SUCCESS );
				close( dd );
			finis
		}

	finis



	prep("Open a temp file.");
	start
		strcpy( path, data_path() );
		strcat( path, HASHFILE );

		expecting ( SUCCESS );
		fd = open( path, O_RDONLY );
	finis


	testing("lseek with whence not 0, 1, or 2");
	testing("should return -1 with errno = EINVAL");
	start
		expecting (VOID);
		signal (SIGSYS, dummy);

		expecting (EINVAL);
		lseek (fd, 0L, 3);

		expecting (VOID);
		signal (SIGSYS, dummy);

		expecting (EINVAL);
		lseek (fd, 0L, -1);
	finis


	cleanup("Close the temp file.");
	start
		expecting ( SUCCESS );
		close( fd );
	finis



	testing("Lseek on an invalid file descriptor");
	testing("should return (-1) with errno = EBADF.");
	start
		expecting ( EBADF );
		lseek( fd, 0L, SEEK_SET);
	finis


	testing("Lseek on a pipe");
	testing("should return (-1) with errno = ESPIPE.");
	start
		expecting (SUCCESS);
		pipe (pipedes);
		expecting ( ESPIPE );
		lseek( pipedes [0], 0L, SEEK_SET);
	finis


	done();
	/*NOTREACHED*/
}

drs(n,d1,d2)
int n;
char *d1,*d2;
{
	int i, sum, degree, offset;

	n++;

	for(sum=0,i=0;(sum+1+(2*i))<n;i++)
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

/*
 *  invoke a dummy routine if a signal is caught,
 *  thus excersizing more than we would if we set SIG_IGN
 */
siggy dummy()
{
}
