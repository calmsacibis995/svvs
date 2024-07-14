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

#ifdef	LOCKF
#include	<unistd.h>
#endif

#ident	"@(#)fseek0.c	1.9"

# define HASHFILE	"hlist"
# define SRCFILE	"hdata"
# define FFILESIZ 	(1024*47/2+23)	/* do not change without	*/
/* rebuilding hdata and hlist	*/

typedef char *ptr;

char *modes[] = {
	"r", 
	"r+", 
	"a+", 
};

#define	NMODES	(sizeof(modes)/sizeof(ptr))

/*
** fseek0
**	Test the fseek function for read files
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
	char		path[PATH_MAX];
	FILE		*fptr;
	FILE		*dptr;
	int		ret_val;
	long		curloc;
	long		newloc;
	char		buf[2];
	char		where[2];
	char		d1, d2;
	int		b, d;

	int		fseek();
	long		ftell();
	char		*data_path();

	setup("fseek");
	funct("fseek");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

	start

		for( mode = 0; mode < NMODES; mode++ )
		{

			switch (mode) {

			case 0:

				abstract("Ftell and fseek of existing file opened for read (r) ");
				abstract("at many points in file referenced from start of file.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 1:

				abstract("Ftell and fseek of existing file opened for update (r+) ");
				abstract("at many points in file referenced from current loc in file.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 2:

				abstract("Ftell and fseek of existing file opened for append and update (a+)");
				abstract("at many points in file referenced from end of file.");
				abstract("Expect a return value of 0 from fseek, current loc from ftell, undefined errno.");
				break;
			}


			prep("Open hash and source files.");
			start
				strcpy( path, data_path() );
				strcat( path, HASHFILE );

				expecting ( NON_NULL );
				dptr = fopen( path, "r");

				strcpy( path, data_path() );
				strcat( path, SRCFILE );

				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );

			finis

			testing("Test the contents of the two files.");
			start
				curloc = 0L;
#ifdef SV_RFS
				for (i = 0; i <= 100; i++ )
#else
				for (i = 0; i <= FFILESIZ; i++)
#endif
				{
					if (i < FFILESIZ)
					{
						where[0] = where[1] = '\0';

						expecting ( 2 );
						fread(&where[0], 1, 2, dptr);

						newloc = (long)(((int)(where[0]&0x00ff)<<8) + ((int)where[1]&0x00ff));
					}
					else
						newloc = (long)FFILESIZ;
					switch (mode) {
					case 0:
						expecting ( SUCCESS );
						fseek(fptr, newloc, SEEK_SET);
						break;
					case 1:
						expecting ( SUCCESS );
						fseek(fptr, newloc-curloc, SEEK_CUR);
						break;
					case 2:
						expecting ( SUCCESS );
						fseek(fptr, newloc-(long)FFILESIZ, SEEK_END);
						break;
					}

					expecting ( newloc );
					curloc = ftell( fptr );
					if (curloc != newloc)
					{
						break;
					}
					buf[0] = buf[1] = '\0';

					if (newloc < (long)FFILESIZ-1L)
					{
						curloc += 2L;
						expecting ( 2 );
						fread(&buf[0], 1, 2, fptr);
					}
					else if (newloc < (long)FFILESIZ)
					{
						curloc++;
						expecting ( 1 );
						ret_val = fread(&buf[0], 1, 1, fptr);
					}
					if (newloc < (long)FFILESIZ)
					{
						drs( ((int)newloc & 0xffff), &d1, &d2);
						if (newloc == (long)FFILESIZ-1L) d2 = '\0';
						d = (((int)d1) << 8) + (int)d2;
						b = (((int)buf[0]) << 8) + (int)buf[1];
						if (d != b)
						{
							error("File content did not match expected index\n. Expected index values was %d.\n Actual index value was %d.\n", d, b);	/* ERROR */
							break;
						}
					}
				}

			finis

			cleanup("Close the two files.");
			start

				expecting ( SUCCESS );
				fclose( fptr );

				expecting ( SUCCESS );
				fclose( dptr );
			finis

		}

		abstract("Fseek undoes effects of ungetc.");
		abstract("First char of test file when read.");

	finis

	start

		prep("Open a data file.");
		start
			expecting ( NON_NULL );
			fptr = fopen( path, "r");
		finis

		testing("Read, ungetc, and ftell.");
		testing("Determine if the pointer points to the correct place.");
		start
			for (i=0; i<FFILESIZ/2; i++)
			{
				expecting ( 1 );
				ret_val = fread(&buf[0], 1, 1, fptr);

			}

			if (ungetc('\007', fptr) == EOF)
			{
				severr("ungetc failed");
			}

			expecting ( 0 );
			fseek( fptr, 0L, SEEK_SET );

			expecting ( 0L );
			curloc = ftell( fptr );

			expecting ( 1 );
			fread(&buf[0], 1, 1, fptr);

			if (buf[0] != 0)
				error("Fseek did not undo effects of ungetc");

		finis

		cleanup("Close the file.");
		start
			expecting ( SUCCESS );
			fclose( fptr );
		finis

	finis


	abstract("Fseek on tty returns non-zero");
	abstract("Expecting a non-zero return value");
	start

		prep("Open /dev/console.");
		start
			expecting ( VOID );
			fptr = fopen("/dev/console", "w");
		finis

		testing("Issue the call to fseek.");
		start
			expecting ( VOID );
			if (fseek( fptr, -1L, SEEK_CUR) == 0)
				error("Fseek returned zero for improper seek");
		finis

		expecting ( SUCCESS );
		fclose( fptr );

	finis

	done();
	/*NOTREACHED*/
}

drs(n, d1, d2)
int n;
char *d1, *d2;
{
	int i, sum, degree, offset;

	n++;

	for(sum=0, i=0;(sum+1+(2*i))<n;i++)
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
