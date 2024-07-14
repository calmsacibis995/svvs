/*LINTLIBRARY*/
#include	<sys/types.h>
#include	<stdio.h>
#include	<sys/stat.h>
#include	<fcntl.h>

#include	<sv_base.h>
#include	<sv_env.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)get_file.c	1.2"
#endif

void get_file( file, buf )
char *file;
char *buf;
{
	int	fd;
	int	i;

	exit_on ( ERROR | SEVERR );

	expecting ( VOID );
	fd = open( file, O_RDONLY );
	if ( fd < 0 )
	{
		severr( "get_file: fopen failed" );
	}

	for(i=0; i<FILESIZ; i++) buf[i] = '\0';

	expecting ( VOID );
	if ( read( fd, buf, FILESIZ ) != FILESIZ )
	{
		expecting ( VOID );
		(void) close( fd );

		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "get_file: read failed" );
		}
	}

	expecting ( VOID );
	if ( close( fd ) != 0 )
	{
		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "get_file: close failed" );
		}
	}
}
