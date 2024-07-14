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

#ident	"@(#)write1.c	1.5"

# define FILENAME	"write.data"

int modes[] = {
	O_WRONLY, 
	O_WRONLY | O_APPEND, 
	O_RDWR, 
};
#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** write
**	Test the write function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		open(OS)
**		close(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
**		creat()
*/
main()
{
	int		mode, nbytes;
	char		path[PATH_MAX];
	int		fd;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ*2+1];

	void		init_buf();
	void		put_file();
	void		get_file();
	void		clr_buf();
	int		cmp_file();
	char		*data_path();

	setup("write");
	funct("write");

	funct("WRITE, write to files.");

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

				abstract("Write of various combinations of nbytes for existing file opened");
				abstract("for O_WRONLY.");
				abstract("Expect a return value of nitems.");
				break;
			case 1:

				abstract("Write of various combinations of nbytes");
				abstract("for existing file opened for O_WRONLY | O_APPEND");
				abstract("Expect a return value of nitems.");
				break;
			case 2:

				abstract("Write of various combinations of nbytes");
				abstract("for existing file opened for O_RDWR");
				abstract("Expect a return value of nitems.");
				break;
			}

			testing("Open, write, close, and compare file contents.");
			start
				for(nbytes = 0; nbytes <= FILESIZ; nbytes = newcnt(nbytes))
				{
					if(mode == 1)
					{
						put_file( path, 0666, buf1 );
					}
					else
					{
						clr_buf( buf2, FILESIZ );
						put_file( path, 0666, buf2 );
					}

					expecting ( SUCCESS );
					fd = open( path, modes[mode], 0664 );

					expecting ( nbytes );
					write(fd, &buf1[0], (unsigned)nbytes);

					expecting ( FILESIZ-nbytes );
					write(fd, &buf1[nbytes],  (unsigned)(FILESIZ-nbytes));

					expecting ( SUCCESS );
					close( fd );

					if (mode == 1)
					{
						get_file( path, &buf2[0], FILESIZ*2);

						if (cmp_file(buf1, buf2 ) != FILESIZ)
						{
							error("file contents differ from first comparison ");
						}

						if (cmp_file(buf1, &buf2[FILESIZ] ) != FILESIZ)
						{
							error("file contents differ from second comparison ");
						}

					}
					else 
					{
						get_file( path, &buf2[0], FILESIZ);

						if (cmp_file(buf1, buf2 ) != FILESIZ)
						{
							error("file contents differ ");
						}
					}

					expecting ( SUCCESS );
					if ( unlink( path ) != 0 )
					{
						severr("unlink failed");
					}
				}
			finis

		}
	finis


	done();
	/*NOTREACHED*/
}

void put_file( file, mode, buf )
char *file;
int mode;
char *buf;
{
	int		fd;

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

	expecting ( VOID );
	if ( write( fd, buf, FILESIZ ) != FILESIZ )
	{
		expecting ( VOID );
		(void) close( fd );
		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "write failed" );
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
void get_file( file, buf, cnt )
char *file;
char *buf;
int cnt;
{
	int	fd;

	clr_buf( buf, cnt+1 );

	expecting ( VOID );
	fd = open( file, O_RDONLY );
	if ( fd < 0 )
	{
		severr( "open failed" );
	}

	expecting ( VOID );
	if ( read( fd, buf,  (unsigned)cnt ) != cnt )
	{
		expecting ( VOID );
		(void) close( fd );
		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "read failed" );
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
