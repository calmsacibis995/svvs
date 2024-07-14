#include <sv_base.h> /*replacement macros */

#include    <sys/types.h>
#include    <errno.h>
#include    <stdio.h>
#include    <sys/stat.h>
#include    <string.h>
#include    <fcntl.h>
/*    #include    <limits.h>    */
#include    <svvs.h>
#include    <sv_env.h>
#include    <sv_macro.h>

#ifdef	LOCKF
#include	<unistd.h>
#endif

#ident    "@(#)fseek1.c	1.9"

# define HASHFILE    "hlist"
# define SRCFILE    "hdata"
# define FILENAME    "fseek.data"
# define INITSIZ    (BUFSIZ*3/2+17)
#ifdef SV_RFS
# define FFILESIZ     100
#else
# define FFILESIZ     (1024*47/2+23)    /* do not change without    */
#endif
/* rebuilding hdata and hlist    */

typedef char *ptr;

char *modes[] = {
	"a", 
	"r+", 
	"a+", 
};

#define    NMODES    (sizeof(modes)/sizeof(ptr))

/*
** fseek1
**    Test the fseek function for old write files
**
**    This test is dependent upon:
**        
**        <stdio.h>
**        fopen()
**        fread()
**        fclose()
**        creat()
*/
main()
{
	int        i;
	int        mode;
	char        path[PATH_MAX];
	FILE        *fptr;
	FILE        *dptr;
	int        ret_val;
	long        curloc;
	long        newloc;
	char        buf[2];
	char        where[2];
	char        d1, d2;

	int        fseek();
	long        ftell();
	void        put_file();
	char        *data_path();

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

				abstract("Ftell and fseek of existing file opened for append (a) ");
				abstract("at many points in file.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 1:


				abstract("Ftell and fseek of existing file opened for update (r+) ");
				abstract("at many points.");
				abstract("Expect a return value of 0 and current loc for ftell, undefined errno.");
				break;
			case 2:


				abstract("Ftell and fseek of existing file opened for append and update (a+)");
				abstract("at many points in file.");
				abstract("Expect a return value of 0 from fseek, current loc from ftell, undefined errno.");
				break;
			}

			prep("Open hashfile and data files.");
			start
				strcpy( path, data_path() );
				strcat( path, HASHFILE );

				expecting ( NON_NULL );
				dptr = fopen( path, "r" );

				strcpy( path, data_path() );
				strcat( path, FILENAME );

				put_file( path, 0666 );

				expecting ( NON_NULL );
				fptr = fopen( path, modes[mode] );

			finis

			testing("Seek and compare data.");
			start
				curloc = 0L;
				for (i = 0; i < FFILESIZ; i++)
				{
					where[0] = where[1] = '\0';

					expecting ( 2 );
					fread(&where[0], 1, 2, dptr);

					newloc = (long)(((int)(where[0]&0x00ff)<<8) + ((int)(where[1]&0x00ff)));
					expecting ( 0 );
					fseek(fptr, newloc, SEEK_SET);

					expecting ( newloc );
					curloc = ftell( fptr );
					if (curloc != newloc)
					{
						break;
					}

					drs( ((int)newloc & 0xffff), &d1, &d2);

					expecting ( 1 );
					fwrite(&d1, 1, 1, fptr);

				}

			finis

			testing("Do an ftell.");
			start
				expecting ( VOID );
				rewind(dptr);

				if (mode == 0)
				{
					expecting ( SUCCESS );
					fclose ( fptr );

					expecting ( NON_NULL );
					fopen( path, "r" );
				}
				else
				{
					expecting ( 0 );
					fseek(fptr, 0L, SEEK_SET);

					expecting ( 0L );
					curloc = ftell( fptr );
					if (curloc != 0L)
					{
						break;
					}
				}
			finis
			curloc = 0L;
			if (mode !=  1)
			{

				testing("Unchanged existing contents of file, new contents match expected");
				testing("values");
				testing("matching contents.");

				start
					for (i = 0; i < INITSIZ; i++)
					{
						expecting ( 1 );
						fread(&buf[0], 1, 1, fptr);

						drs( i, &d1, &d2);

						if (buf[0] != d1)
						{
							error("existing contents of file altered");
							break;
						}
					}
					for (i = 0; i < FFILESIZ; i++)
					{
						where[0] = where[1] = '\0';

						expecting ( VOID );
						ret_val = fread(&where[0], 1, 2, dptr);
						if (ret_val != 2 )
						{
							severr("Fread of list file failed");
						}

						newloc = (long)(((int)(where[0]&0x00ff)<<8) + ((int)(where[1]&0x00ff)));
						drs( ((int)newloc & 0xffff), &d1, &d2);

						expecting ( 1 );
						fread(&buf[0], 1, 1, fptr);

						if (buf[0] != d1)
						{
							error("unexpected contents of file");
							break;
						}
					}
				finis
			}
			else
			{

				testing("File contents match expected values.");
				start

					for (i = 0; i < FFILESIZ; i++)
					{
						drs( ((int)i & 0xffff), &d1, &d2);

						expecting ( 1 );
						fread(&buf[0], 1, 1, fptr);

						if (buf[0] != d1)
						{
							error("unexpected contents of file");
							break;
						}
					}
				finis
			}

			cleanup("Close open files.");
			start
				expecting ( SUCCESS );
				fclose( fptr );

				expecting ( SUCCESS );
				fclose( dptr );
			finis

		}
	finis

	done();

	/*NOTREACHED*/
}

void put_file( file, mode )
char *file;
int mode;
{
	int        fd;
	int        i;
	char        d1, d2;

	expecting ( VOID );
	(void) unlink( file );

	expecting ( VOID );
	fd = creat( file, mode );
	if ( fd < 0 )
	{
		severr( "creat failed" );
	}


	for ( i = 0; i < INITSIZ ; i++ )
	{
		drs ( i, &d1, &d2 );
		expecting ( VOID );
		if ( write( fd, &d1, 1 ) != 1 )
		{
			(void) close( fd );
			(void) unlink( file );
			{
				severr( "write failed" );
			}
			break;
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
