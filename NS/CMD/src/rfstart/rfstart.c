/*
 *	rfstart.c
 *
 *	a test of rfstart(NS_CMD)
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

#ident	"@(#)rfstart.c	1.3"

/*
 *	test cases:
 *	
 *		su
 *		fumount RF_RESOURCE
 *		umount -d RF_REMOTE_RESOURCE
 *		rfstop
 *		rfstart
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		mount -d RF_REMOTE_RESOURCE RF_REMOTE_MOUNT
 *		rfstart	(ERROR - rfs is already running)
 *		rfstop
 *		su svvs
 *		rfstart	(ERROR - not root)
 *		su
 *		rfstart
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		mount -d RF_REMOTE_RESOURCE RF_REMOTE_MOUNT
 *	
 *	caveats:
 *		if interrupted may leave rfs stopped, RF_RESOURCE unadv and/or
 *		fumounted, RF_REMOTE_RESOURCE unmounted.
 *	
 */

main() {
	char *rf_lcl;
	char *rf_rmt;

	setup( "rfstart" );
	funct( "rfstart" );

	prep("Allocate and initialize buffers.");
	start
		char *malloc();
		exit_on( ERROR );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );
		rf_rmt = malloc( strlen(RF_REMOTE_MOUNT) + strlen(SVVSPATH) + 1 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
		sprintf( rf_rmt, "%s%s", SVVSPATH, RF_REMOTE_MOUNT );
	finis

	abstract("A normal invocation of rfstart.  Set real uid to root.");
	abstract("Unmount and unadvertise resources. Stop rfs.  Invoke");
	abstract("'rfstart'.  Mount and advertise resources.");
	start
		exit_on( ERROR );
		prep("Set real uid to root.");
		start
			setuid( ROOT_UID );
		finis
		prep("Invoke 'fumount %s'.", RF_RESOURCE );
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
		continue_on( ERROR );
		testing("Invoke 'rfstart'.");
		start
			sv_cmd( "rfstart", (char *)0 );
		finis
		cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl );
		start
			sv_cmd( "adv", "-d",  RF_DESCRIPTION, 
			  RF_RESOURCE, rf_lcl, (char *)0 );
		finis
		cleanup("Invoke 'mount -d %s %s'.", RF_REMOTE_RESOURCE, rf_rmt);
		start
			sv_cmd( "mount", "-d", RF_REMOTE_RESOURCE, rf_rmt, (char *)0 );
		finis
	finis
	abstract("Invoke 'rfstart' when rfs is running.");
	start
		exit_on( ERROR );
		prep("Set real uid to root.");
		start
			setuid( ROOT_UID );
		finis
		break_on( ERROR );
		testing("Invoke 'rfstart'.");
		start
			expecting( FAILURE );
			sv_cmd( "rfstart", (char *)0 );			
		finis
		chk_stderr( 1 );
	finis
	abstract("Invoke rfstart while not root.  Set real uid to root.");
	abstract("Unmount and unadvertise resources. Stop rfs.  Set uid");
	abstract("to %d and invoke 'rfstart', expecting failure.  Set", UID);
	abstract("uid back to root and invoke 'rfstart'.  Mount and");
	abstract("advertise resources.");
	start
		exit_on( ERROR );
		prep("Set real uid to root.");
		start
			setuid( ROOT_UID );
		finis
		prep("Invoke 'fumount %s'.", RF_RESOURCE );
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
		prep("Fork a child process to verify 'rfstart' while ");
		prep("executing as non-root.");
		start
			int pid;
			int sts;

			break_on( ERROR );
			if( !(pid = fork()) ) {
				exit_on( ERROR );
				prep("Set uid to %d.", UID);
				start
					setuid( UID );
				finis
				testing("Invoke 'rfstart', expecting failure due to");
				testing("non-root.");
				start
					expecting( FAILURE );
					sv_cmd( "rfstart", (char *)0 );
				finis
				chk_stderr( 1 );
				done();
			}	
			expecting( pid );
			wait( &sts );
			if( sts )
				error("child process terminated abnormally.");
		finis
		cleanup("Invoke 'rfstart', expecting success.");
		start
			sv_cmd( "rfstart", (char *)0 );
		finis
		cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl );
		start
			sv_cmd( "adv", "-d",  RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl, (char *)0 );
		finis
		cleanup("Invoke 'mount -d %s %s'.", RF_REMOTE_RESOURCE, rf_rmt);
		start
			sv_cmd( "mount", "-d", RF_REMOTE_RESOURCE, rf_rmt, (char *)0 );
		finis
	finis
	done();
}
