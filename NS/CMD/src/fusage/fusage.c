/*
 *	fusage.c
 *
 *	a test of fusage(NS_CMD)
 *
 *	this file should be setuid root
 *
 *	tabstops should be set to 4 to view this file
 *
 *	note: incorporates SVID change that fusage is restricted to root.
 *
 */

#include <sv_base.h>
#include <stdio.h>	
#include <fcntl.h>	
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

#include <sv_rfs.h>

#ident	"@(#)fusage.c	1.7"

/*
 *	test cases:
 *		su root	( implied - should be setuid root )
 *		fusage
 *		fusage	RF_LOCAL
 *		fusage	RF_RESOURCE
 *		fusage	RF_BLK_SPEC
 *		su svvs
 *		fusage	( ERROR - not root )
 */

main() {
	char *rf_lcl;

	setup( "fusage" );
	funct( "fusage" );

	prep("Allocate and initialize buffers. Set real uid to root.");
	start
		char *malloc();
		exit_on( ERROR );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
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

	abstract("Invoke 'fusage' with no arguments.");
	start
		testing("Invoke 'fusage'.");
		start
			sv_cmd( "fusage", (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
		grp_stdout( RF_BLK_SPEC );
		grp_stdout( RF_RESOURCE );
		grp_stdout( rf_lcl );
		grp_stdout( RF_NODENAME );
		grp_stdout( RF_CLIENT );
	finis

	abstract("Invoke 'fusage %s'.", rf_lcl);
	start
		testing("Invoke 'fusage'.");
		start
			sv_cmd( "fusage", rf_lcl, (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
		grp_stdout( rf_lcl );
		grp_stdout( RF_CLIENT );
	finis
	abstract("Invoke 'fusage %s'.", RF_RESOURCE );
	start
		testing("Invoke 'fusage'.");
		start
			sv_cmd( "fusage", RF_RESOURCE, (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
		grp_stdout( RF_RESOURCE );
		grp_stdout( RF_CLIENT );
	finis
	abstract("Invoke 'fusage %s'.", RF_BLK_SPEC );
	start
		testing("Invoke 'fusage'.");
		start
			sv_cmd( "fusage", RF_BLK_SPEC, (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
		grp_stdout( RF_BLK_SPEC );
		grp_stdout( RF_NODENAME );
	finis

	abstract("Invoke 'fusage' while not root.");
	start
		break_on( WARNING | ERROR );
		prep("Call setuid to set user id to %d.",UID);
		start
			setuid( UID );
		finis
		testing("Invoke 'fusage'.");
		start
			expecting( VOID );
			if( sv_cmd( "fusage", (char *)0 ) )
				warning( "%s\nfusage failed unexpectedly",
				  ADVISORY );
		finis
		chk_stdout( 3 );
		chk_stderr( 2 );
	finis
	done();
}
