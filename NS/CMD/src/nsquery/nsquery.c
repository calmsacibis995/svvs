/*
 *	nsquery.c
 *
 *	a test of nsquery(NS_CMD)
 *
 *	this file should NOT be setuid root
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#include <sv_base.h>
#include <stdio.h>	
#include <fcntl.h>	
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)nsquery.c	1.4"

/*
 *	test cases:
 *
 *		nsquery
 *		nsquery	RF_NODENAME
 *		nsquery RF_DOMAIN.
 *		nsquery RF_DOMAIN.RF_NODENAME
 *		nsquery -h
 *		nsquery	-h RF_NODENAME
 *		nsquery	-h RF_DOMAIN.
 *		nsquery	-h RF_DOMAIN.RF_NODENAME
 */

main() {
	char *domain;
	char *dom_nodnm;
	char *dom_client;

	setup( "nsquery" );
	funct( "nsquery" );

	prep("Allocate and initialize buffers.");
	start
		char *malloc();
		exit_on( ERROR );
		domain = malloc( strlen(RF_DOMAIN) + 2 );
		dom_nodnm = malloc( strlen(RF_DOMAIN) + strlen(RF_NODENAME) + 2 );
		dom_client = malloc( strlen(RF_DOMAIN) + strlen(RF_CLIENT) + 2 );
		sprintf( domain, "%s.", RF_DOMAIN );
		sprintf( dom_nodnm, "%s.%s", RF_DOMAIN, RF_NODENAME );
		sprintf( dom_client, "%s.%s", RF_DOMAIN, RF_CLIENT );
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

	abstract("Test cases of 'nsquery' without the '-h' option.");
	start
		abstract("Invoke 'nsquery' with no arguments.");
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( RF_REMOTE_RESOURCE );
			grp_stdout( dom_nodnm );
			grp_stdout( dom_client );
		finis
		abstract("Invoke 'nsquery %s'.", RF_NODENAME );
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", RF_NODENAME, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( dom_nodnm );
		finis
		abstract("Invoke 'nsquery %s.'.", RF_DOMAIN );
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", domain, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( RF_REMOTE_RESOURCE );
			grp_stdout( dom_nodnm );
			grp_stdout( dom_client );
		finis
		abstract("Invoke 'nsquery %s.%s'.", RF_DOMAIN, RF_NODENAME );
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", dom_nodnm, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( dom_nodnm );
		finis
	finis
	abstract("Test cases of 'nsquery' with the '-h' option.");
	start
		abstract("Invoke 'nsquery -h'.");
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", "-h", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( RF_REMOTE_RESOURCE );
			grp_stdout( dom_nodnm );
			grp_stdout( dom_client );
		finis
		abstract("Invoke 'nsquery -h %s'.", RF_NODENAME );
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", "-h", RF_NODENAME, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( dom_nodnm );
		finis
		abstract("Invoke 'nsquery -h %s.'.", RF_DOMAIN );
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", "-h", domain, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( RF_REMOTE_RESOURCE );
			grp_stdout( dom_nodnm );
			grp_stdout( dom_client );
		finis
		abstract("Invoke 'nsquery -h %s.%s'.", RF_DOMAIN, RF_NODENAME );
		start
			testing("Invoke 'nsquery'.");
			start
				sv_cmd( "nsquery", "-h", dom_nodnm, (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_RESOURCE );
			grp_stdout( dom_nodnm );
		finis
	finis

	done();
}
