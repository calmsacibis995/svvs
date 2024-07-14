/*
 *	unadv.c
 *
 *	a test of unadv(NS_CMD)
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

#ident	"@(#)unadv.c	1.3"

/*
 *	test cases:
 *	
 *		su
 *		unadv RF_RESOURCE
 *		fumount RF_RESOURCE
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *	
 *		unadv RF_RESOURCE
 *		unadv RF_RESOURCE (ERROR - not adv)
 *		fumount RF_RESOURCE
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *	
 *		su svvs
 *		unadv RF_RESOURCE (ERROR - not root)
 *		
 *	caveats:
 *		If interrupted may leave RF_RESOURCE unadvertised
 *	
 */

main() {
	char *rf_lcl;

	setup( "unadv" );
	funct( "unadv" );
	
	prep("Allocate and initialize buffers.  Set real uid to root.");
	start
		char *malloc();
		exit_on( ERROR );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
		setuid( ROOT_UID );
	finis

	abstract("A normal invocation of 'unadv'.");
	start
		abstract("Invoke 'unadv %s'.", RF_RESOURCE );
		start
			sv_cmd( "unadv", RF_RESOURCE, (char *)0 );
		finis
		chk_stderr( 0 );
		cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
		start
/*
 *			may not have been mounted again, 
 *			from a prior test, don't check ret val, 
 *			as may fail.
 */
			expecting( VOID );
			sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
		finis
		cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl );
		start
			sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE, 
			  rf_lcl, (char *)0 );
		finis
	finis
	abstract("Invoke 'adv' with a resource that is not advertised.");
	abstract("Invoke 'adv %s' twice, expecting the second", RF_RESOURCE);
	abstract("invocation to fail.");
	start
		prep("Invoke 'unadv %s', expecting success.", RF_RESOURCE );
		start
			sv_cmd( "unadv", RF_RESOURCE, (char *)0 );
		finis
		testing("Invoke 'unadv %s' again, expecting failure.",
		   RF_RESOURCE );
		start
			expecting( FAILURE );
			sv_cmd( "unadv", RF_RESOURCE, (char *)0 );
		finis
		chk_stderr( 1 );
		cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
		start
/*
 *			may not have been mounted again, 
 *			don't check ret val, as may fail.
 */
			expecting( VOID );
			sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
		finis
		cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl );
		start
			sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE, 
			  rf_lcl, (char *)0 );
		finis
	finis
	abstract("Invoke 'adv' while not executing as root.");
	start
		break_on( ERROR );
		prep("Set uid to %d.", UID );
		start
			setuid( UID );
		finis
		testing("Invoke 'unadv %s', expecting failure.", RF_RESOURCE );
		start
			expecting( FAILURE );
			sv_cmd( "unadv", RF_RESOURCE, (char *)0 );
		finis
		chk_stderr( 1 );
	finis
	done();
}
