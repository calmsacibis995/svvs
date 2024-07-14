/*
 *	rmntstat.c
 *
 *	a test of rmntstat(NS_CMD)
 *
 *	this file should be setuid root
 *
 *	tabstops should be set to 4 to view this file
 */

#include <sv_base.h>
#include <stdio.h>	
#include <fcntl.h>	
#include <svvs.h>	
#include <sv_rfs.h>
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)rmntstat.c	1.6"

/*
 *	test cases:
 *	
 *		su	
 *		rmntstat
 *		rmntstat RF_RESOURCE
 *		rmntstat -h
 *		rmntstat -h RF_RESOURCE
 *		rmntstat RF_REMOTE_RESOURCE
 *		rmntstat foo.
 *		rmntstat foo/
 *		su svvs
 *		rmntstat	( ERROR - not root )
 */

main() {
	char *rf_lcl;
	char *client;

	setup( "rmntstat" );
	funct( "rmntstat" );

	prep("Allocate and initialize buffers.");
	prep("Set real uid to root.");
	start
		char *malloc();

		exit_on( ERROR );
		rf_lcl = malloc( strlen( SVVSPATH ) + strlen( RF_LOCAL ) + 1 );
		client = malloc( strlen( RF_DOMAIN ) + strlen( RF_CLIENT ) + 2 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
		sprintf( client, "%s.%s", RF_DOMAIN, RF_CLIENT );
		setuid( ROOT_UID );
	finis

	prep("Sleep for %d seconds to allow the network to stabilize.",
	  RF_SLEEP );
	prep("This is to give the client system, '%s', time to mount",
	  RF_CLIENT );
	prep("the locally advertised resource, '%s'.",RF_RESOURCE );
	start
		expecting( VOID );
		sleep( RF_SLEEP );
	finis

	abstract("Normal invocations of 'rmntstat' without the '-h' option.");
	start
		abstract("Invoke 'rmntstat' with no arguments.");
		start
			break_on( ERROR );
			testing("Invoke 'rmntstat'.");
			start
				sv_cmd( "rmntstat", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( rf_lcl );
			grp_stdout( client );
		finis	
		abstract("Invoke 'rmntstat %s'.", RF_RESOURCE);
		start
			break_on( ERROR );
			testing("Invoke 'rmntstat'.");
			start
				sv_cmd( "rmntstat", RF_RESOURCE, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( rf_lcl );
			grp_stdout( client );
		finis	
	finis
	abstract("Normal invocations of 'rmntstat' with the '-h' option.");
	start
		abstract("Invoke 'rmntstat -h' with no arguments.");
		start
			break_on( ERROR );
			testing("Invoke 'rmntstat'.");
			start
				sv_cmd( "rmntstat", "-h", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( rf_lcl );
			grp_stdout( client );
		finis	
		abstract("Invoke 'rmntstat -h %s'.", RF_RESOURCE);
		start
			break_on( ERROR );
			testing("Invoke 'rmntstat'.");
			start
				sv_cmd( "rmntstat", "-h", RF_RESOURCE, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( rf_lcl );
			grp_stdout( client );
		finis	
	finis
	abstract("Invocations of 'rmntstat' which should fail.");
	start
 		abstract("Invoke 'rmntstat' with a resource that does");
		abstract("not physically reside on the local machine.");
		start
			testing("Invoke 'rmntstat %s'.",RF_REMOTE_RESOURCE);
			start
				expecting( FAILURE );
				sv_cmd( "rmntstat", RF_REMOTE_RESOURCE, (char *)0 );
			finis
			chk_stderr( 1 );
		finis	
		abstract("Invoke 'rmntstat' with an invalid resource name");
		abstract("containing a '.'.");
		start
			testing("Invoke 'rmntstat foo.'.");
			start
				expecting( FAILURE );
				sv_cmd( "rmntstat", "foo.", (char *)0 );
			finis
			chk_stderr( 1 );
		finis	
		abstract("Invoke 'rmntstat' with an invalid resource name");
		abstract("containing a '/'.");
		start
			testing("Invoke 'rmntstat foo/'.");
			start
				expecting( FAILURE );
				sv_cmd( "rmntstat", "foo/", (char *)0 );
			finis
			chk_stderr( 1 );
		finis	
		abstract("Invoke 'rmntstat' while not root.");
		start
			prep("Call setuid to set user id to %d.",UID);
			start
				setuid( UID );
			finis
			testing("Invoke 'rmntstat'.");
			start
				expecting( FAILURE );
				sv_cmd( "rmntstat", (char *)0 );
			finis
			chk_stderr( 1 );
		finis	
	finis
	done();
}
