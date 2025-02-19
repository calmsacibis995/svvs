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

#ident	"@(#)fwrite1.c	1.7"

# define FILENAME	"fwrite.data"

typedef char *ptr;

char *modes[] = {
	"a", 
	"r+", 
	"a+", 
};
#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** fwrite
**	Test the fwrite function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fclose(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
*/
main()
{
	int		sum;
	int		mode, blk, nblks;
	char		path[PATH_MAX];
	FILE		*fptr;
	int		ret_val;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ*2+1];

	void		init_buf();
	void		put_file();
	void		get_file();
	void		clr_buf();
	int		cmp_file();
	void		cmp_stat();
	void		rewind();
	int		newcnt();
	char		*temp_path();

	setup ("fread" );
	funct("fwrite");

	exit_on ( ERROR );

	start
		prep("Init a buffer and file names.");
		start

			init_buf( buf1 );

			strcpy( path, temp_path() );
			strcat( path, FILENAME );
		finis


		for( mode = 1; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:

				abstract("Fwrite of various combinations of non-positive size and/or nitems");
				abstract("For existing file opened for append (a).");
				abstract("Expect a return value of 0, possible ferror, undefined errno.");
				break;
			case 1:


				abstract("Fwrite of various combinations of non-positive size and/or nitems");
				abstract("For existing file opened for update (r+).");
				abstract("Expect a return value of 0, possible ferror, undefined errno.");
				break;
			case 2:


				abstract("Fwrite of various combinations of non-positive size and/or nitems");
				abstract("For existing file opened for update and append (a+).");
				abstract("Expect a return value of 0, possible ferror, undefined errno.");
				break;
			}

			put_file( path, 0666, buf1 );

			testing("Call fopen and write data to a file.");
			start

				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );

				blk = 0;

#ifdef	SYSVR_2
				for( blk = -1; blk <= 1 ; blk++ )
				{
#endif
					for( nblks = -1 ; nblks <= 1; nblks++ )
					{
						if (blk > 0 && nblks > 0) break;

						expecting ( 0 );
						fwrite(&buf1[0], blk, nblks, fptr);

						expecting ( FALSE );
						ferror(fptr);

					}
#ifdef	SYSVR_2
				}
#endif

			finis

			testing("Close and unlink the file.");
			start

				expecting ( SUCCESS );
				fclose(fptr);

				expecting ( SUCCESS );
				unlink( path );
			finis

			switch (mode) {

			case 0:

				abstract("Fwrite of various combinations of positive size and nitems");
				abstract("For existing file opened for append (a).");
				abstract("Expect a return value of nitems, no ferror, undefined errno.");
				break;
			case 1:


				abstract("Fwrite of various combinations of positive size and nitems");
				abstract("For existing file opened for update (r+).");
				abstract("Expect a return value of nitems, no ferror, undefined errno.");
				break;
			case 2:


				abstract("Fwrite of various combinations of positive size and nitems");
				abstract("For file opened for append and update (a+).");
				abstract("Expect a return value of nitems, no ferror, undefined errno.");
				break;
			}

			testing("Open the file, write data, compare, close and unlink the file.");
			start

				for( blk = 1; blk <= FILESIZ ; blk = newcnt(blk) )
				{
					put_file( path, 0666, buf1);

					expecting ( NON_NULL );
					fptr = fopen( path, modes[mode] );

					sum = 0;

					for( nblks = 1 ; (sum+nblks) <= FILESIZ/blk; nblks = newcnt( nblks ))
					{
						expecting ( nblks );
						fwrite(&buf1[sum*blk], blk, nblks, fptr);

						expecting ( FALSE );
						ferror(fptr);

						sum += nblks;
					}
					expecting ( FILESIZ-(sum*blk) );
					fwrite(&buf1[(sum*blk)], 1, FILESIZ-(sum*blk), fptr);

					expecting ( FALSE );
					ferror(fptr);

					expecting ( SUCCESS );
					fclose( fptr );

					switch (mode) {
					case 0:
					case 2:
						get_file( path, &buf2[0], 2*FILESIZ);
						if (cmp_file(buf1, buf2 ) != FILESIZ)
							error("existing file contents altered");

						if (cmp_file(buf1, &buf2[FILESIZ] ) != FILESIZ)
							error("File contents differ");
						break;
					case 1:
						get_file( path, &buf2[0], FILESIZ);
						if (cmp_file(buf1, buf2 ) != FILESIZ)
							error("File contents differ");
						break;
					}

					expecting ( SUCCESS );
					unlink( path );
				}
			finis

		}

	finis


	done();
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
	if(i < 2) i = 2;
	else if (i < 5) i = 5;
	else if (i <= 9) i = 1;
	if(i==1) j++;
	for(k=0;k<j;k++) i *= 10;
	return(i);
}

/* This version of get_file is not to be confused with
   the one found in a library elsewhere...The two versions
   are slightly different
*/

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
		severr( "open failed" );

	expecting ( VOID );
	if ( read( fd, buf, (unsigned)cnt ) != cnt )
	{
		expecting ( VOID );
		(void) close( fd );
		expecting ( VOID );
		(void) unlink( file );
		severr( "read failed" );
	}

	expecting ( VOID );
	if ( close( fd ) != 0 )
	{
		expecting ( VOID );
		(void) unlink( file );
		severr( "close failed" );
	}
}
