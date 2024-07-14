/*LINTLIBRARY*/
#include	<sv_base.h>
#include	<sv_env.h>
#include	<stdio.h>
#include	<svvs.h>
#include	<sv_macro.h>

#ifdef	UNDEF
#ident	"@(#)put_file.c	1.3"
#endif

void put_file( file, mode, buf ) 
char *file;
int mode;
char *buf;
{
	int		fd;

	exit_on ( ERROR | SEVERR );

	expecting ( VOID );
	(void) unlink( file );

	expecting ( SUCCESS );
	fd = creat( file, sv_mode(mode) );

	expecting ( VOID );
	if ( write( fd, buf, FILESIZ ) != FILESIZ )
	{
		expecting ( VOID );
		(void) close( fd );

		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "put_file: write failed" );
		}
	}

	expecting ( SUCCESS );
	if ( close( fd ) != 0 )
	{
		expecting ( VOID );
		(void) unlink( file );
		{
			severr( "put_file: close failed" );
		}
	}
}
