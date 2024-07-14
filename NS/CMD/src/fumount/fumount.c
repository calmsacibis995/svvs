/*
 *	fumount.c
 *
 *	a test of fumount(NS_CMD)
 *
 *	this file should be setuid root
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#include <sv_base.h>
#include <stdio.h>	
#include <fcntl.h>	
#include <sys/types.h>	
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

#include <sv_rfs.h>

#ident	"@(#)fumount.c	1.3"

/*
 *	test cases:
 *
 *		su
 *		fumount RF_RESOURCE
 *		adv RF_RESOURCE RF_PATHNAME
 *		fumount -w 5 RF_RESOURCE
 *		adv RF_RESOURCE RF_PATHNAME
 *		fumount foo.		(ERROR - invalid resource name)
 *		fumount foo/		(ERROR - invalid resource name)
 *		fumount RF_RESOURCE	(ERROR - not adv and not remote mounted)
 *		su svvs
 *		fumount RF_RESOURCE	(ERROR - not root)
 *		
 *	caveats:
 *
 *		If interrupted may leave RF_RESOURCE unadv and/or fumounted
 *	
 */

main() {
	char *rf_lcl;

	setup( "fumount" );
	funct( "fumount" );

	prep("Allocate and initialize buffers.");
	start
		char *malloc();
		exit_on( ERROR );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
	finis

	abstract("Normal test cases of 'fumount' while running as root.");
	start
		break_on( ERROR );
		prep("Call setuid to set the real userid to root.");
		start
			setuid( ROOT_UID );
		finis
		abstract("Invoke 'fumount %s'.", RF_RESOURCE);
		start
			testing("Invoke 'fumount'.");
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			chk_stderr( 0 );
			cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION,
			  RF_RESOURCE, rf_lcl );
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION,
			  RF_RESOURCE, rf_lcl, (char *)0 );
			finis
		finis	
		abstract("Invoke 'fumount -w 5 %s'.", RF_RESOURCE);
		start
			time_t time();
			time_t before, after;
			prep("Call time() to get the current time.");
			start
				before = time( (time_t *)0 );
			finis
			testing("Invoke 'fumount'.");
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			chk_stderr( 0 );
			testing("Verify the current time is at least 5 seconds");
			testing("beyond the time 'fumount' was invoked.");
			start
				after = time( (time_t *)0 );
				if( ( before + 5 ) < after )
					error("'fumount	-w 5 %s' did not wait 5 seconds.",
					  RF_RESOURCE);
			finis
			cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION,
			  RF_RESOURCE, rf_lcl );
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION,
			  RF_RESOURCE, rf_lcl, (char *)0 );
			finis
		finis
	finis
	abstract("Error test cases of 'fumount' while running as root.");
	start
		prep("Call setuid to set the real userid to root.");
		start
			setuid( ROOT_UID );
		finis
		abstract("Invoke 'fumount foo.' expecting failure due to");
		abstract("an invalid resource name.");
		start
			testing("Invoke 'fumount'.");
			start
				expecting( FAILURE );
				sv_cmd( "fumount", "foo.", (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		abstract("Invoke 'fumount foo/' expecting failure due to");
		abstract("an invalid resource name.");
		start
			testing("Invoke 'fumount'.");
			start
				expecting( FAILURE );
				sv_cmd( "fumount", "foo/", (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		abstract("Invoke 'fumount %s' when %s is not advertised",
		  RF_RESOURCE, RF_RESOURCE);
		abstract("and not remotely mounted.  First invoke");
		abstract("'fumount %s' to unadvertise and unmount %s,",
		  RF_RESOURCE, RF_RESOURCE);
		abstract("and then invoke 'fumount %s' again.", RF_RESOURCE);
		start
			prep("Invoke 'fumount' the first time.");
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			testing("Invoke 'fumount' a second time.");
			start
				expecting( FAILURE );
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			chk_stderr( 1 );
			cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION,
			  RF_RESOURCE, rf_lcl );
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION,
			  RF_RESOURCE, rf_lcl, (char *)0 );
			finis
		finis	
	finis
	abstract("Test cases of 'fumount' while running as non-root.");
	start
		break_on( ERROR );
		prep("Call setuid to set the userid to %d.", UID);
		start
			setuid( UID );
		finis
		abstract("Invoke 'fumount %s', expecting failure.", RF_RESOURCE );
		start
			testing("Invoke 'fumount'.");
			start
				expecting( FAILURE );
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			chk_stderr( 3 );
		finis	
	finis
	done();
}
