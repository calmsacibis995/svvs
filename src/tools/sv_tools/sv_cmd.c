/*LINTLIBRARY*/
#include <varargs.h>
#include <stdio.h>
#include <sv_env.h>
/*
 *	Z funct for commands
 */
#ident	"@(#)sv_cmd.c	1.2"

static int success( val )
int *val;
{
	return( (*val) == 0 );
}

static int failure( val )
int *val;
{
	return( (*val) != 0 );
}

static char *format( val )
int *val;
{
	static char buf[STRVAL];

	(void) sprintf( buf, "%d", *val );
	return( buf );
}

static int compare( a, b )
int *a;
int *b;
{
	return( *a == *b );
}

int *sv_geti();

static struct ftable Tcommand = {
	"                                ",	/* reserve space for cmd_name */
	"an exit status of 0",
	"a non-zero exit status",
	format,
	compare,
	success,
	failure,
	(pfunctptr)sv_geti,
	0,
};

#define cmd_name Tcommand.sv_name
static char *sv_argv[ 64 ];
static char stdoutfile[ 512 ] ="";
static char stderrfile[ 512 ] ="";

char *sv_stdoutfile() {
	return( stdoutfile );
}	
char *sv_stderrfile() {
	return( stderrfile );
}	
char *sv_cmdname() {
	return( cmd_name );
}	
sv_cmd( va_alist )
va_dcl
{
	va_list	ap;
	int cpid;
	int wpid;
	int sts;
	int i = 0;
	
	va_start( ap );
	while( sv_argv[ i++ ] = va_arg( ap, char * ) );
	va_end( ap );
	strcpy( cmd_name, sv_argv[0] );
	if (!*stdoutfile) {
		strcpy( stdoutfile, sv_temp_path() );
		strcat( stdoutfile, "stdout" );
		strcpy( stderrfile, sv_temp_path() );
		strcat( stderrfile, "stderr" );
	}
	if( !(cpid = fork()) ) {
		expecting( VOID );
		unlink( stdoutfile );
		unlink( stderrfile );
		exit_on( SEVERR | ERROR );
		expecting ( SUCCESS );
		if( !freopen(stderrfile,"w",stderr) )
			severr("Redirection of stderr with freopen failed.");
		if( !freopen(stdoutfile,"w",stdout) )
			severr("Redirection of stdout with freopen failed.");
		execvp( cmd_name, sv_argv );
		severr("exec failed.");	/* won't print, still exit w/ fail */
	}
	if( cpid < 0 )
		severr("Cannot fork.");
	wpid = wait(&sts);
	if ( wpid != cpid ) {
		severr( "wait(): returned unexpected value: %d",wpid );
	}
	evaluate( &Tcommand, (pointer)&sts );
	return( sts );
}
