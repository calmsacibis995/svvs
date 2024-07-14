/*
 *	idload.c
 *
 *	a test of idload(NS_CMD)
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

#ident	"@(#)idload.c	1.6"

/*
 *	test cases:
 *
 *		su root	( implied - should be setuid root )
 *		idload -n
 *		idload -n RF_IDLOAD_DIR
 *		idload -n -u RF_U_RULES
 *		idload -n -u RF_U_RULES RF_IDLOAD_DIR
 *		idload -n -g RF_G_RULES
 *		idload -n -g RF_G_RULES RF_IDLOAD_DIR
 *		idload -n -u RF_U_RULES -g RF_G_RULES
 *		idload -n -u RF_U_RULES -g RF_G_RULES RF_IDLOAD_DIR
 *		idload -u RF_U_RULES -g RF_G_RULES
 *		idload -u RF_U_RULES -g RF_G_RULES RF_IDLOAD_DIR
 *		su svvs
 *		idload -n (ERROR - not root)
 *
 *	caveats:
 *
 *		Invalid entries for RF_[UG]_RULES will alter [ug]id mapping.
 *	
 */

char sysbuf[2048];

main() {

	setup( "idload" );
	funct( "idload" );

	prep("Set real uid to root.");
	start
		exit_on( ERROR );
		setuid( ROOT_UID );
	finis

	prep("Call access to verify that %s and %s exist.",
	  RF_U_RULES, RF_G_RULES );
	start
		exit_on( ERROR );
		access( RF_U_RULES, 0 );
		access( RF_G_RULES, 0 );
	finis

	abstract("Invoke 'idload -n'.");
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n %s'", RF_IDLOAD_DIR);
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", RF_IDLOAD_DIR, (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n -u %s'", RF_U_RULES );
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", "-u", RF_U_RULES, (char *)0 );
		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n -u %s %s'", RF_U_RULES, RF_IDLOAD_DIR);
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", "-u", RF_U_RULES, RF_IDLOAD_DIR,
			  (char *)0 );

		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n -g %s'", RF_G_RULES );
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", "-g", RF_G_RULES, (char *)0 );

		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n -g %s %s'", RF_G_RULES, RF_IDLOAD_DIR);
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", "-g", RF_G_RULES, RF_IDLOAD_DIR, 
			  (char *)0 );

		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n -u %s -g %s'", RF_U_RULES, RF_G_RULES);
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", "-u", RF_U_RULES, "-g", RF_G_RULES,
			  (char *)0 );

		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n -u %s -g %s %s'", RF_U_RULES, RF_G_RULES, 
	  RF_IDLOAD_DIR);
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-n", "-u", RF_U_RULES, "-g",
			  RF_G_RULES, RF_IDLOAD_DIR, (char *)0 );

		finis
		chk_stdout( 1 );
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -u %s -g %s'", RF_U_RULES, RF_G_RULES );
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-u", RF_U_RULES, "-g", RF_G_RULES,
			  (char *)0 );

		finis
/*
 *		no chk_stdout, as may not print anything on success
 */
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -u %s -g %s %s'", RF_U_RULES, RF_G_RULES,
	  RF_IDLOAD_DIR);
	start
		break_on( ERROR );
		testing("Invoke 'idload'.");
		start
			sv_cmd( "idload", "-u", RF_U_RULES, "-g", RF_G_RULES,
			  RF_IDLOAD_DIR, (char *)0 );

		finis
/*
 *		no chk_stdout, as may not print anything on success
 */
		chk_stderr( 0 );
	finis

	abstract("Invoke 'idload -n' while not root");
	start
		break_on( ERROR );
		prep("Call setuid to set user id to %d.",UID);
		start
			setuid( UID );
		finis
		testing("Invoke 'idload'.");
		start
			expecting( VOID );
			if( !sv_cmd( "idload", "-n", (char *)0 ) )
				warning( "%s\nidload succeeded unexpectedly",ADVISORY );
		finis
		chk_stderr( 3 );
	finis

	abstract("Invoke 'idload' while not root");
	start
		break_on( ERROR );
		prep("Call setuid to set user id to %d.",UID);
		start	
			setuid( UID );
		finis
		testing("Invoke 'idload'.");
		start
			expecting( FAILURE );
			sv_cmd( "idload", (char *)0 );
		finis
		chk_stderr( 1 );
	finis

	done();
}
