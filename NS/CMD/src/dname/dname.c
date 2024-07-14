/*
 *	dname.c
 *
 *	a test of dname(NS_CMD)
 *
 *	this file should be setuid root
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

#include <sv_rfs.h>

#ident	"@(#)dname.c	1.3"

/*
 *	test cases:
 *		su
 *		dname
 *		dname -d
 *		fumount RF_RESOURCE
 *		umount -d RF_REMOTE_RESOURCE
 *		rfstop
 *		dname -D RF_NEWDOMAIN.
 *		dname -D RF_DOMAIN.
 *		rfstart
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		mount RF_REMOTE_RESOURCE RF_REMOTE_MOUNT
 *		dname -D RF_DOMAIN.	(ERROR - rfs running)
 *		dname -D BAD_DOMAIN	(ERROR - invalid domain name)
 *		su svvs
 *		dname
 *		dname -d
 *		dname -D RF_DOMAIN.	(ERROR - not root)
 *
 *	caveats:
 *		If interrupted, may change domain name or leave rfs stopped.
 *
 */

main() {
	char	*rf_lcl;
	char	*rf_rmt;

	setup( "dname" );
	funct( "dname" );

	prep("Allocate and initialize buffers.");
	start
		char *malloc();
		exit_on( ERROR );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );
		rf_rmt = malloc( strlen(RF_REMOTE_MOUNT) + strlen(SVVSPATH) + 1 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
		sprintf( rf_rmt, "%s%s", SVVSPATH, RF_REMOTE_MOUNT );
	finis

	abstract("Test cases of 'dname' while running as root.");
	start
		break_on( ERROR );
		prep("Call setuid to set the real userid to root.");
		start
			setuid( ROOT_UID );
		finis
		continue_on( ERROR );
		abstract("Invoke 'dname' with no arguments.");
		start
			break_on( ERROR );
			testing("Invoke 'dname'.");
			start
				sv_cmd( "dname", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_DOMAIN );
		finis	
		abstract("Invoke 'dname -d'.");
		start
			break_on( ERROR );
			testing("Invoke 'dname'.");
			start
				sv_cmd( "dname", "-d", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_DOMAIN );
		finis	
		abstract("Stop rfs and invoke 'dname' to change the domain name.");
		abstract("Invoke 'dname' again to reset the domain name.  Invoke");
		abstract("'dname' with an invalid domain name.  Restart rfs.");
		start
			prep("Invoke 'fumount %s'.", RF_RESOURCE);
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			prep("Invoke 'umount -d %s'.", RF_REMOTE_RESOURCE );
			start
				sv_cmd( "umount", "-d", RF_REMOTE_RESOURCE, (char *)0 );
			finis
			prep("Invoke 'rfstop'.");
			start
				sv_cmd( "rfstop", (char *)0 );
			finis
 			testing("Invoke 'dname -D %s'.", RF_NEWDOMAIN );
			start
				sv_cmd( "dname", "-D", RF_NEWDOMAIN, (char *)0 );
			finis
			chk_stderr( 0 );
 			testing("Invoke 'dname' to verify the domain name was");
			testing("changed to '%s'.", RF_NEWDOMAIN );
			start
				sv_cmd( "dname", (char *)0 );
				chk_stderr( 0 );
				chk_stdout( 1 );
				grp_stdout( RF_NEWDOMAIN );
			finis
			testing("Invoke 'dname -D %s'.", RF_DOMAIN );
			start
				sv_cmd( "dname", "-D", RF_DOMAIN, (char *)0 );
			finis
			chk_stderr( 0 );
 			testing("Invoke 'dname' to verify the domain name was");
			testing("changed back to '%s'.", RF_DOMAIN );
			start
				sv_cmd( "dname", (char *)0 );
				chk_stderr( 0 );
				chk_stdout( 1 );
				grp_stdout( RF_DOMAIN );
			finis
			testing("Invoke 'dname -D %s', expecting", RF_BADDOMAIN );
			testing("failure due to an invalid domain name.");
			start
				expecting( FAILURE );
				sv_cmd( "dname", "-D", RF_BADDOMAIN, (char *)0 );
			finis
			chk_stderr( 1 );
 			testing("Invoke 'dname' to verify the domain name was");
			testing("not changed by the previous failure.");
			start
				sv_cmd( "dname", (char *)0 );
				chk_stderr( 0 );
				chk_stdout( 1 );
				grp_stdout( RF_DOMAIN );
			finis
			cleanup("Invoke 'rfstart'.");
			start
				sv_cmd( "rfstart", (char *)0 );
			finis
			cleanup("Invoke 'adv -d %s %s'.",
			  RF_DESCRIPTION, RF_RESOURCE, rf_lcl );
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, (char *)0 );
			finis
			cleanup("Invoke 'mount -d %s %s'.",
			  RF_REMOTE_RESOURCE, rf_rmt );
			start
				sv_cmd( "mount", "-d", RF_REMOTE_RESOURCE,
				  rf_rmt, (char *)0 );
			finis
		finis
		abstract("Invoke 'dname -D %s' while rfs is running.",
		  RF_NEWDOMAIN);
		start
			testing("Invoke 'dname'.");
			start
				expecting( FAILURE );
				sv_cmd( "dname", "-D", RF_NEWDOMAIN, (char *)0 );
			finis
			chk_stderr( 1 );
		finis			
	finis
	abstract("Test cases of 'dname' while running as non-root.");
	start
		break_on( ERROR );
		prep("Call setuid to set the userid to %d.", UID);
		start
			setuid( UID );
		finis
		continue_on( ERROR );
		abstract("Invoke 'dname' with no arguments.");
		start
			break_on( ERROR );
			testing("Invoke 'dname'.");
			start
				sv_cmd( "dname", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_DOMAIN );
		finis	
		abstract("Invoke 'dname -d'.");
		start
			break_on( ERROR );
			testing("Invoke 'dname'.");
			start
				sv_cmd( "dname", "-d", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( RF_DOMAIN );
		finis	
		abstract("Invoke 'dname -D %s', expecting failure.",RF_NEWDOMAIN);
		start
			break_on( ERROR );
			testing("Invoke 'dname'.");
			start
				expecting( FAILURE );
				sv_cmd( "dname", "-D", RF_NEWDOMAIN, (char *)0 );
			finis
			chk_stdout( 0 );
			chk_stderr( 1 );
		finis	
	finis
	done();
}
