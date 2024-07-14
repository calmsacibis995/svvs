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

#ident	"@(#)freadwrit0.c	1.6"

# define FILENAME	"freadwrite.data"

typedef char *ptr;

char *modes[] = {
	"a+", 
	"w+", 
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
	int		sum;
	int		mode, blk, nblks;
	char		path[PATH_MAX];
	FILE		*fptr;
	int		ret_val;
	char		buf1[FILESIZ+1];
	char		buf2[FILESIZ+1];

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

			strcpy( path, temp_path() );
			strcat( path, FILENAME );
		finis


		for( mode = 0; mode < NMODES; mode++ )
		{
			switch (mode) {

			case 0:


				abstract("Fwrite and fread of various combinations of positive size");
				abstract("and nitems for new file opened for update and append (a+).");
				abstract("Expect a return value of nitems, no ferror, undefined errno.");


				break;
			case 1:


				abstract("Fwrite and fread of various combinations of positive size");
				abstract("and nitems for new file opened for write and update (w+).");
				abstract("Expect a return value of nitems, no ferror, undefined errno.");


				break;
			}

			testing("Call fwrite, feof, and ferror.");
			start

				for( blk = 1; blk <= FILESIZ ; blk = newcnt(blk) )
				{
					prep("Call fopen, and write data to file and read it back.");
					start

						expecting ( NON_NULL );
						fptr = fopen( path, modes[mode] );
						sum = 0;

						for( nblks = 1 ; (sum+nblks) <= FILESIZ/blk; nblks = newcnt( nblks ))
						{
							expecting ( nblks );
							ret_val = fwrite(&buf1[sum*blk], blk, nblks, fptr);

							expecting ( FALSE );
							ferror(fptr);


							sum += nblks;
						}

						expecting ( FILESIZ-(sum*blk) );
						ret_val = fwrite(&buf1[(sum*blk)], 1, FILESIZ-(sum*blk), fptr);

						expecting ( FALSE );
						ret_val = ferror(fptr);


						expecting ( VOID );
						rewind(fptr);
						clr_buf( buf2, FILESIZ+1 );

						expecting ( FILESIZ-(sum*blk) );
						ret_val = fread(&buf2[0], 1, FILESIZ-(sum*blk), fptr);

						expecting ( FALSE );
						ferror(fptr);

					finis


					testing("Determine whether the contents of the file are correct.");
					testing("We do this by doing an fread and then a comparison.");
					start

						sum = FILESIZ-(sum*blk);

						while( nblks = backdown( nblks ) ) {
							expecting ( nblks );
							fread(&buf2[sum], blk, nblks, fptr);

							sum += blk*nblks;

							expecting ( VOID );
							ret_val = ferror(fptr);

						}

						if (cmp_file(buf1, buf2 ) != FILESIZ)
						{
							error("File contents differ");
						}

					finis

					cleanup("Close the file and remove it.");
					start

						expecting ( SUCCESS );
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
