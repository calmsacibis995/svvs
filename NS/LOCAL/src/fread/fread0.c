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

#ident	"@(#)fread0.c	1.7"

# define FALSE		0
# define TRUE		1
# define FILENAME	"fread.data"

typedef char *ptr;

char *modes[] = {
	"r", 
	"r+", 
	"a+", 
};
#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** fread
**	Test the fread function.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		fclose(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
**		time()
*/

main()
{
	ttime		t;
	int		mode, blk, nblks;
	char		path[PATH_MAX];
	FILE		*fptr;
	int		ret_val;
	struct stat	st_buf1;
	struct stat	st_buf2;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ+1];

	ttime		time();
	void		init_buf();
	void		put_file();
	void		get_file();
	void		clr_buf();
	int		cmp_file();
	void		cmp_stat();
	void		rewind();
	int		newcnt();
	char		*temp_path();

	setup ( "fread" );
	funct("fread");

	prep("init a buffer and file names.");
	start

		init_buf( buf1 );
		strcpy( path, temp_path() );
		strcat( path, FILENAME );
	finis

	prep("Write data to a file for reading.");
	start

		put_file( path, 0666, buf1 );
	finis


	for( mode = 0; mode < NMODES; mode++ ) {
		switch (mode) {
		case 0:
			abstract("Fread various combinations of non-positive size and/or nitems");
			abstract("for file opened for read (r).");
			abstract("Expect a return value of 0, feof of false, possible ferror, unchanged ");
			abstract("file status, undefined errno.");
			break;

		case 1:
			abstract("Fread various combinations of non-positive size and/or nitems");
			abstract("for file opened for update (r+).");
			abstract("Expect a return value of 0, feof of false, possible ferror, unchanged ");
			abstract("file status, undefined errno.");
			break;

		case 2:
			abstract("Fread various combinations of non-positive size and/or nitems");
			abstract("for file opened for append and update (a+).");
			abstract("Expect a return value of 0, feof of false, possible ferror, unchanged ");
			abstract("file status, undefined errno.");
			break;
		}
		start

			prep("Stat the data file, and call fopen to open.");
			start

				expecting ( SUCCESS );
				stat( path, &st_buf1 );

				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );

				expecting ( SUCCESS );
				t = time( (ttime *)0) + 2l;

				expecting ( SUCCESS );
				while (time( (ttime *)0) < t) ;
			finis

			testing("Call fread, feof, and ferror.");
			start

				blk = 0;
#ifdef	SYSVR_2
				for( blk = -1; blk <= 1 ; blk++ )
				{
#endif
					for( nblks = -1 ; nblks <= 1; nblks++ )
					{
						if (blk > 0 && nblks > 0) break;
						expecting ( FAILURE );
						fread(&buf2[0], blk, nblks, fptr);

						expecting ( FALSE );
						feof(fptr);

						expecting ( FALSE );
						ret_val = ferror(fptr);
					}
#ifdef	SYSVR_2
				}
#endif
			finis


			testing("Stat the file to verify it has not been altered.");
			start

				expecting ( SUCCESS );
				stat( path, &st_buf2 );
				cmp_stat( &st_buf1, &st_buf2, NO_TRUNC );
			finis


			cleanup("Close the file.");
			start

				expecting ( SUCCESS );
				fclose( fptr );
			finis

		finis


		switch (mode) {

		case 0:
			abstract("Fread various combinations of positive size and nitems");
			abstract("for file opened for read (r).");
			abstract("Expect a return value of nitems, proper feof, no ferror, file status");
			abstract("unchanged except for updated access time, undefined errno.");
			break;

		case 1:
			abstract("Fread various combinations of positive size and nitems");
			abstract("for file opened for update (r+).");
			abstract("Expect a return value of nitems, proper feof, no ferror, file status");
			abstract("unchanged except for updated access time, undefined errno.");
			break;
		case 2:
			abstract("Fread various combinations of positive size and nitems");
			abstract("for file opened for append and update (a+).");
			abstract("Expect a return value of nitems, proper feof, no ferror, file status");
			abstract("unchanged except for updated access time, undefined errno.");
			break;
		}
		start

			prep("Open the file.");
			start

				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );
			finis

			testing("Call fread, feof, and ferror.");
			start

				for( blk = 1; blk <= FILESIZ ; blk = newcnt(blk) ){
					for( nblks = 1 ; nblks <= FILESIZ/blk; nblks = newcnt( nblks )) {
						clr_buf( &buf2[0], (nblks*blk+1) );

						expecting ( nblks );
						ret_val = fread(&buf2[0], blk, nblks, fptr);

						expecting ( FALSE );
						feof(fptr);

						expecting ( FALSE );
						ferror(fptr);

						expecting (FILESIZ-(blk*nblks) );
						fread(&buf2[(blk*nblks)], 1, FILESIZ-(blk*nblks), fptr);
						if (cmp_file(buf1, buf2 ) != FILESIZ) {
							error("file contents differ");
						}

						expecting ( 0 );
						fread(&buf2[0], 1, 1, fptr);

						expecting ( FALSE );
						ferror(fptr);

						expecting ( TRUE );
						feof(fptr);

						expecting ( VOID );
						(void) rewind(fptr);
					}
				}
			finis

			testing("Call stat to verify the access time has changed.");
			start

				expecting ( SUCCESS );
				stat( path, &st_buf2 );

				cmp_stat( &st_buf1, &st_buf2, ACCESS );
			finis

			cleanup("Close the file.");
			start

				expecting ( SUCCESS );
				fclose( fptr );
			finis

		finis

	}
	cleanup("Unlink the file.");
	start

		expecting ( SUCCESS );
		unlink( path );
	finis

	done();
	/*NOTREACHED*/
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
