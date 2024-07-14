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

#ident	"@(#)rewind2.c	1.6"

# define FILENAME	"rewind.data"
# define min(a,b)	(((a)<(b))?(a):(b))
# define max(a,b)	(((a)>(b))?(a):(b))


typedef char *ptr;

char *modes[] = {
	"a", 
	"w", 
	"a+", 
	"w+", 
};
#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** rewind2
**	Test the rewind function for new write files
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
	void		get_file();
	int		compare_file();
	void		rewind();
	long		ftell();
	char		*data_path();

	setup("rewind");
	funct("ftell");

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

		for( mode = 0; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:

				abstract("Ftell and rewind of new file opened for append (a) at many points in file.");
				abstract("movement through file done by writing.");
				abstract("Expect a return value of filesize from ftell, undefined errno.");
				break;
			case 1:


				abstract("Ftell and rewind of new file opened for write at many points in file.");
				abstract("movement through file done by writing.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 2:


				abstract("Ftell and rewind of new file opened for append and update (a+)");
				abstract("at many points in file.");
				abstract("movement through file done by writing.");
				testing("Expect a return value of 0 from ftell, undefined errno.");
				break;
			case 3:


				abstract("Ftell and rewind of new file opened for write and update (w+)");
				abstract("at many points in file.");
				abstract("movement through file done by writing.");
				abstract("Expect a return value of 0 from ftell, undefined errno.");
				break;
			}

			prep("Open a temporary file, but unlink it first if it exists.");
			prep("Issue an initial call to ftell to prime the test.");
			start
				expecting ( VOID );
				(void) unlink( path );


				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );

				expecting ( 0L);
				loc = ftell( fptr );

				sum = (long)FILESIZ;
			finis

			testing("Call ftell, fwrite, and rewind for various points in the file.");
			start
				int limit;
				limit = (mode==1||mode==3)?FILESIZ:100;

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
							error("Ftell did not return current loc.\nThe expected value was %l.\nThe actual value was %l:\n", sum, loc);	/* ERROR */
							goto brkout;
						}
						break;
					case 1:
					case 3:
						if (loc != (long)i)
						{
							error("Ftell did not return current loc.\nThe expected value was %l.\nThe actual value was %l:\n", (long)i, loc);	/* ERROR */
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

			if( mode == 1 || mode == 3)
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
