#include <sv_base.h> /*replacement macros */

#ifndef	SYSVR_2
#include	<unistd.h>
#endif

# include	<fcntl.h>
# include	<sys/types.h>
# include	<sys/stat.h>
# include 	<stdio.h>
# include	<string.h>
# include	<errno.h>
# include	<svvs.h>
#include	<sv_env.h>
#include	<sv_macro.h>
/*	# include	<limits.h>	*/

#ident	"@(#)readwrite1.c	1.6"


# define FILENAME	"readwrite.data"

int modes[] = {
	O_RDWR, 
	O_RDWR | O_APPEND, 
};
#define	NMODES	(sizeof(modes)/sizeof(int))

/*
** readwrite
**	Test the read and write functions together.
**
**	This test is dependent upon:
**		
**		<stdio.h>
**		close(OS)
**		stat(OS)
**		<sys/types.h>
**		<sys/stat.h>
*/
main()
{
	int		mode, nbytes;
	char		path[PATH_MAX];
	int		fd;
	char		buf1[FILESIZ+1];
	char		buf2[2*FILESIZ+1];

	void		init_buf();
	void		put_file();
	void		clr_buf();
	int		cmp_file();
	long		lseek();
	char		*data_path();

	setup("write");
	funct("write");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

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

				abstract("Write and read of various combinations of nbytes");
				abstract("for existing file opened for O_RDWR.");
				abstract("Expect a return value of nbytes.");
				break;
			case 1:

				abstract("Write and read of various combinations of positive size");
				abstract("and nbytes for existing file opened for O_RDWR | APPEND.");
				abstract("Expect a return value of nbytes.");
				break;
			}


			testing("Open, write, read, close, and compare file contents.");
			start
				for( nbytes = 0 ; nbytes <= FILESIZ; nbytes=newcnt(nbytes))
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
					write(fd, &buf1[nbytes], (unsigned)(FILESIZ-nbytes));

					expecting ( 0l );
					lseek(fd, 0L, SEEK_SET);

					if (mode == 1)
						clr_buf( buf2, 2*FILESIZ+1 );
					else
						clr_buf( buf2, FILESIZ+1 );


					expecting ( FILESIZ-nbytes );
					read(fd, &buf2[0], (unsigned)(FILESIZ-nbytes));

					if (mode == 1)
					{

						expecting ( FILESIZ );
						read(fd, &buf2[FILESIZ-nbytes], FILESIZ);

						expecting ( nbytes );
						read(fd, &buf2[2*FILESIZ-nbytes], (unsigned)nbytes);
					}
					else
					{
						expecting ( nbytes );
						read(fd, &buf2[FILESIZ-nbytes], (unsigned)nbytes);
					}

					if (cmp_file(buf1, buf2 ) != FILESIZ)
					{
						error("file contents differ for first cmp");
					}

					if (mode == 1)
					{
						if (cmp_file(buf1, &buf2[FILESIZ] ) != FILESIZ)
						{
							error("file contents differ for second cmp");
						}

					}

					expecting ( SUCCESS );
					close( fd );


					expecting ( SUCCESS );
					unlink( path );
				}
			}

		finis
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
	if(!i) i = 1;
	else if(i < 2) i = 2;
	else if (i < 5) i = 5;
	else if (i <= 9) i = 1;
	if(i==1) j++;
	for(k=0;k<j;k++) i *= 10;
	return(i);
}