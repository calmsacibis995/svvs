/*
 *	rfpasswd.c
 *
 *	a test of rfpasswd(NS_CMD)
 *
 *	this file should NOT be setuid root.
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#include <sv_base.h>
#include <stdio.h>	
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

#ident	"@(#)rfpasswd.c	1.2"

/*
 *	test cases:
 *	
 *		su svvs		(implied - should not be setuid root)
 *		rfpasswd	(ERROR - not root)
 *	
 */

main() {

	setup( "rfpasswd" );
	funct( "rfpasswd" );

	abstract("Invoke 'rfpasswd' while not root");
	start
		break_on( ERROR );
		prep("Verify that this test is not running as superuser.");
		start
			expecting( VOID );
			if( getuid() == ROOT_UID || geteuid() == ROOT_UID )
				error("Test is being run by root.");
		finis 
		testing("Invoke	'rfpasswd'.");
		start
			expecting( FAILURE );
			sv_cmd( "rfpasswd", (char *)0 );
		finis
		chk_stderr( 1 );
	finis

	done();
}
