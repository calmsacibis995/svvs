#include <sv_base.h> /*replacement macros */
#include	<errno.h>
#include	<string.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
/*	#include	<limits.h>	*/

#ident	"@(#)freadwrit1.c	1.6"

# define FILENAME	"freadwrite.data"

typedef char *ptr;

char *modes[] = {
	"r+", 
	"a+", 
};
#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** freadwrite
**	Test the fread and fwrite functions together.
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
	int		factor;
	int		sum;
	int		mode,  blk,  nblks;
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

	setup("fread");
	funct("fread");

	exit_on ( ERROR );

	start

		prep("Init a buffer and file names.");
		start

			init_buf( buf1 );

			strcpy( path,  temp_path() );
			strcat( path,  FILENAME );
		finis


		for( mode = 0; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:

				abstract("Fwrite and fread of various combinations of positive size");
				abstract("and nitems for existing file opened for update (r+).");
				abstract("Expect a return value of nitems,  no ferror,  undefined errno.");


				break;
			case 1:

				abstract("Fwrite and fread of various combinations of positive size");
				abstract("and nitems for existing file opened for write and update (a+).");
				abstract("Expect a return value of nitems,  no ferror,  undefined errno.");


				break;
			}

			prep("Call fopen, and write data to file and read it back.");
			start

				for( blk = 1; blk <= FILESIZ ; blk = newcnt(blk) )
				{
					testing("Call fwrite, feof, and ferror.");
					start

						put_file( path,  0666,  buf1 );

						expecting ( NON_NULL );
						fptr = fopen( path,  modes[mode] );

						sum = 0;

						for( nblks = 1 ; (sum+nblks) <= FILESIZ/blk; nblks = newcnt( nblks ))
						{
							expecting ( nblks );
							fwrite(&buf1[sum*blk],  blk,  nblks,  fptr);

							expecting ( FALSE );
							ferror(fptr);

							sum += nblks;
						}

						expecting ( FILESIZ-(sum*blk) );
						ret_val = fwrite(&buf1[(sum*blk)],  1,  FILESIZ-(sum*blk),  fptr);

						expecting ( FALSE );
						ferror(fptr);
					finis


					testing("Determine whether the contents of the file are correct.");
					start
						expecting ( VOID );
						rewind(fptr);

						if (mode == 0)
							factor = 1;
						else
							factor = 2;

						clr_buf( buf2,  factor*FILESIZ+1 );

						expecting ( factor*(FILESIZ-(sum*blk)) );
						fread(&buf2[0],  1,  factor*(FILESIZ-(sum*blk)),  fptr);

						expecting ( FALSE );
						ferror(fptr);

						sum = factor*(FILESIZ-(sum*blk));

						while( nblks = backdown( nblks ) ) {
							expecting ( nblks*factor );
							ret_val = fread(&buf2[sum],  blk,  factor*nblks,  fptr);

							sum += blk*nblks*factor;

							expecting ( FALSE );
							ferror(fptr);

						}
						if ( mode == 0 )
						{
							if (cmp_file(buf1,  buf2 ) != FILESIZ)
							{
								error("File contents differ");
							}
						}
						else
						{
							if (cmp_file(buf1,  buf2 ) != FILESIZ)
							{
								error("existing file contents altered");
							}

							if (cmp_file(buf1,  &buf2[FILESIZ] ) != FILESIZ)
							{
								error("File contents differ");
							}
						}

					finis

					expecting ( SUCCESS );
					cleanup("Close the file and remove it.");
					start

						fclose( fptr );

						expecting ( SUCCESS );
						unlink( path );
					finis

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

backdown(i)
int i;
{
	int k;
	int j = 0;

	while(i/10) {
		i/=10;
		j++;
	}
	if(i < 2) i = 5;
	else if (i < 5) i = 1;
	else if (i <= 9) i = 2;
	if(i==5)
		if(!j--) return(0);
	for(k=0;k<j;k++) i *= 10;
	return(i);
}
