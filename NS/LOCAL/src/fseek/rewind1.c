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

#ident	"@(#)rewind1.c	1.5"

# define FILENAME	"rewind.data"
# define min(a,b)	(((a)<(b))?(a):(b))
# define max(a,b)	(((a)>(b))?(a):(b))

typedef char *ptr;

char *modes[] = {
	"a", 
	"r+", 
	"a+", 
};
#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** rewind1
**	Test the rewind function for old write files
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fopen()
**		fread()
**		fclose()
**		creat()
*/
main()
{
	int		i;
	long		sum;
	int		mode;
	char		path[PATH_MAX];
	FILE		*fptr;
	int		ret_val;
	long		loc;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ+1];

	void		init_buf();
	void		put_file();
	void		get_file();
	void		clr_buf();
	int		compare_file();
	void		rewind();
	long		ftell();
	char		*data_path();

	setup("rewind");
	funct("rewind");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	start
		prep("init a buffer and file names.");
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

				abstract("Ftell and rewind of existing file opened for append (a) at many points in file.");
				abstract("Movement through file done by writing.");
				abstract("Expect a return value of filesize from ftell, undefined errno.");
				break;
			case 1:


				abstract("Ftell and rewind of existing file opened for update (r+) at many points in file.");
				abstract("Movement through file done by writing.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 2:


				abstract("Ftell and rewind of existing file opened for append and update (a+)");
				abstract("at many points in file.");
				abstract("Movement through file done by writing.");
				abstract("Expect a return value of 0 from ftell, undefined errno.");
				break;
			}

			prep("Open a temporary file.");
			start
				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );
			finis

			testing("Call ftell, fread, and rewind for various points in the file.");
			start
				int limit;
			
				expecting ( VOID );
				loc = ftell( fptr );

				if (mode == 1 || mode == 2)
				{
					if ( loc != 0L )
					{
						error("Ftell after opening file did not return 0.\n Ftell instead returned %l:\n", loc);	/* ERROR */
						break;
					}
				}
				else
				{
					if( loc != (long)FILESIZ )
					{
						error("Ftell after opening file did not return filesize");
						break;
					}
				}

				sum = (long)FILESIZ;
				limit = (mode == 1) ? FILESIZ : 100;

#ifdef	SV_RFS
				for (i = 1; i <= limit; i += max(min(limit-i,BUFSIZ/17),1))
#else
				for ( i = 1; i <= limit; i++ )
#endif

				{
					expecting ( i );
					fwrite(&buf1[0], 1, i, fptr);


					expecting ( VOID );
					loc = ftell(fptr);
					switch( mode ) {
					case 0:
					case 2:
						sum += i;
						if (loc != sum && loc != (long)i)
						{
							error("Ftell did not return current loc.\n The expected value was %l.\nThe actual value was %l\n", sum, loc);	/* ERROR */
							goto brkout;
						}
						break;
					case 1:
						if (loc != (long)i)
						{
							error("Ftell did not return current loc.\n The expected value was %l.\nThe actual value was %l\n", (long)i, loc);	/* ERROR */
							goto brkout;
						}
						break;
					}
					expecting ( VOID );
					rewind( fptr );
				}
			finis
brkout:
			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				fclose( fptr );
			finis

			if( mode == 1 )
			{

				testing("Contents of file read match buffer written.");
				testing("Expecting matching contents.");
				get_file( path, buf2 );

				if ( compare_file( buf1, buf2 ) != FILESIZ )
					error("File contents differ.");
			}
		}
		cleanup("Unlink the file.");
		start
			expecting ( SUCCESS );
			unlink( path );
		finis

	finis
	done();
}

void clr_buf(buf, cnt)
char *buf;
int cnt;
{
	int		i;

	for(i=0; i<cnt; i++) *buf++ = '\0';
}

void init_buf(buf)
char *buf;
{
	int 		i;
	long 		rnd;
	char 		c;

	rnd = 17l;
	for ( i = 0; i < FILESIZ ; i++ )
	{
		do {
			c = ((rnd = rnd * 1103515245L + 12345)>>16) & 0x7f;
		} while ( c < 0x20 || c > 0x7e ) ;
		buf[i] = c;
	}
	buf[FILESIZ] = 0xff;
}

void put_file( file, mode, buf )
char *file;
int mode;
char *buf;
{
	int		fd;

	expecting ( VOID );
	(void) unlink( file );

	expecting ( VOID );
	fd = creat( file, mode );
	if ( fd < 0 )
	{
		severr( "creat failed" );
	}

	expecting ( VOID );
	if ( write( fd, buf, FILESIZ ) != FILESIZ )
	{
		(void) close( fd );
		(void) unlink( file );
		{
			severr( "write failed" );
		}
	}

	expecting ( VOID );
	if ( close( fd ) != 0 )
	{
		(void) unlink( file );
		{
			severr( "close failed" );
		}
	}
}

void get_file( file, buf ) char *file;
char *buf;
{
	int	fd;

	expecting ( VOID );
	fd = open( file, O_RDONLY );
	if ( fd < 0 )
	{
		severr( "open failed" );
	}

	expecting ( VOID );
	if ( read( fd, buf, FILESIZ ) != FILESIZ )
	{
		(void) close( fd );
		(void) unlink( file );
		{
			severr( "read failed" );
		}
	}

	expecting ( VOID );
	if ( close( fd ) != 0 )
	{
		(void) unlink( file );
		{
			severr( "close failed" );
		}
	}
}

compare_file( buf1, buf2 ) char *buf1;
char *buf2;
{
	int i=0;

	while( *buf1++ == *buf2++ ) i++;

	return i;
}
