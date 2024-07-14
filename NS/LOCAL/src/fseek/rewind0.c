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

#ident	"@(#)rewind0.c	1.5"

# define FILENAME	"rewind.data"
# define min(a,b)	(((a)<(b))?(a):(b))
# define max(a,b)	(((a)>(b))?(a):(b))
typedef char *ptr;

char *modes[] = {
	"r", 
	"r+", 
	"a+", 
};
#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** rewind0
**	Test the rewind function for read files
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
	int		mode;
	long		loc;
	char		path[PATH_MAX];
	FILE		*fptr;
	int		ret_val;
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

	abstract("Testing rewind.");
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

				abstract("Ftell and rewind of existing file opened for read (r) at many points in file.");
				abstract("Movement through file done by reading.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 1:


				abstract("Ftell and rewind of existing file opened for update (r+) at many points in file.");
				abstract("Movement through file done by reading.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 2:


				abstract("Ftell and rewind of existing file opened for append and update (a+)");
				abstract("at many points in file.");
				abstract("Movement through file done by reading.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			}

			prep("Open a temporary file.");
			start
				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );
			finis


			testing("Call ftell, fread, and rewind for various points in the file.");
			start
#ifdef	SV_RFS
				for (i = 1; i <= FILESIZ; i += max(min(FILESIZ-i,BUFSIZ/17),1))
#else
				for (i = 1; i <= FILESIZ; i++)
#endif
				{
					expecting ( 0L );
					loc = ftell( fptr );

					clr_buf ( buf2, i+1 );

					expecting ( i );
					ret_val = fread(&buf2[0], 1, i, fptr);


					expecting ( (long)i );
					loc = ftell(fptr);

					expecting ( VOID );
					rewind( fptr );
				}

			finis

			cleanup("Close the file.");
			start
				expecting ( SUCCESS );
				fclose( fptr );
			finis


			testing("Contents of file read match buffer written.");
			start
				if ( compare_file( buf1, buf2 ) != FILESIZ )
					error("contents differ");
			finis

		}
	finis

	abstract("Rewind undoes effects of getc.");
	abstract("First char of test file when read.");

	start

		prep("Open a temporary file.");
		prep("Read data from it and call ungetc to push one back.");
		start
			expecting ( NON_NULL );
			fptr = fopen( path, "r");

			expecting ( FILESIZ / 2 );
			fread(&buf2[0], 1, (FILESIZ/2), fptr);

			expecting ( VOID );
			if (ungetc('\007', fptr) == EOF)
			{
				severr("ungetc failed");
			}

		finis

		testing("Do a call to rewind.");
		testing("Re-read the file and verify contents.");
		start
			expecting ( VOID );
			rewind( fptr );

			expecting ( FILESIZ / 2 );
			fread(&buf2[0], 1, (FILESIZ/2), fptr);


			if (buf2[0] != buf1[0])
				error("Rewind did not undo effects of ungetc\nExpected character was %c.\nActual read in character was %c.\n", (int)buf1[0], (int)buf2[0]);	/* ERROR */

		finis

		cleanup("Close and unlink the file.");
		start
			expecting ( SUCCESS );
			fclose( fptr );

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
void get_file( file, buf )
char *file;
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
compare_file( buf1, buf2 )
char *buf1;
char *buf2;
{
	int i=0;

	while( *buf1++ == *buf2++ ) i++;

	return i;
}
