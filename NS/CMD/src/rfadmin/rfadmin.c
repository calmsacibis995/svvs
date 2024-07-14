/*
 *	rfadmin.c
 *
 *	a test of rfadmin(NS_CMD)
 *
 *	this file should be setuid root
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#include <sv_base.h>
#include <stdio.h>
#include <svvs.h>
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)rfadmin.c	1.4"

/*
 *
 *	test cases:
 *
 *		su	(implied - should be setuid root)
 *		rfadmin
 *		su svvs
 *		rfadmin	(ERROR - not root)
 *
 */

main() {

	char *primary;
	char *secondary;

	setup( "rfadmin" );
	funct( "rfadmin" );

	abstract("Invoke 'rfadmin' with no arguments while root.");
	start
		prep("Allocate and initialize a buffer.");
		prep("Set real uid to root.");
		start
			char *malloc();
			exit_on( ERROR );
			primary = malloc( strlen(RF_DOMAIN) + strlen(RF_PRIMARY) + 2 );
			sprintf( primary, "%s.%s", RF_DOMAIN, RF_PRIMARY );
			secondary = malloc( strlen(RF_DOMAIN) + strlen(RF_SECONDARY)
			  + 2 );
			sprintf( secondary, "%s.%s", RF_DOMAIN, RF_SECONDARY );
			setuid( ROOT_UID );
		finis
		break_on( ERROR );
		testing("Invoke 'rfadmin'.");
		start
			sv_cmd( "rfadmin", (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
		testing("Verify 'rfadmin' wrote either the primary name server,");
		testing("'%s', or the secondary name server, '%s', to stdout.",
			primary, secondary);
		start
			if( !grep_stdout( primary ) && !grep_stdout( secondary ) )
				error("%s\n%s",
				  "'rfadmin' wrote neither the primary nor the",
				  "secondary name server to stdout");
		finis
	finis

	abstract("Invoke 'rfadmin' while not root.");
	start
		break_on( ERROR );
		prep("Call setuid to set user id to %d.",UID);
		start
			setuid( UID );
		finis
		testing("Invoke 'rfadmin'.");
		start
			expecting( FAILURE );
			sv_cmd( "rfadmin", (char *)0 );
		finis
		chk_stderr( 1 );
		chk_stdout( 0 );
	finis

	done();
}
grep_stdout( target )
char *target;
{
	static char buf[1024];
	FILE 	*fp;
	FILE	*fopen();
	char 	*sv_stdoutfile();
	int		l;
	int		found_it;
	char	*p;

	expecting( SUCCESS );
	fp = fopen(sv_stdoutfile(),"r");
	expecting( VOID );
	l = strlen( target );
	found_it = 0;
	while( fgets( buf, 1024, fp ) && !found_it ) {
		p = buf;
		while( *p && !found_it ) {
			if( !strncmp( p, target, l ) )
				found_it = 1;
			p++;
		}
	}
	fclose( fp );
	return( found_it );
}
