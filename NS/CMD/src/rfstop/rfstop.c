/*
 *	rfstop.c
 *
 *	a test of rfstop(NS_CMD)
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

#ident	"@(#)rfstop.c	1.4"

/*
 *	test cases:
 *
 *		su
 *		fumount	RF_RESOURCE
 *		umount -d RF_REMOTE_RESOURCE
 *		rfstop
 *		rfstart
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		mount -d RF_REMOTE_RESOURCE RF_REMOTE_MOUNT
 *
 *		su
 *		fumount	RF_RESOURCE
 *		umount -d RF_REMOTE_RESOURCE
 *		rfstop
 *		rfstop	( ERROR - rfs not running )
 *		rfstart
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		mount -d RF_REMOTE_RESOURCE RF_REMOTE_MOUNT
 *
 *		su
 *		umount -d RF_REMOTE_RESOURCE
 *		rfstop	( ERROR - resources are still adv )
 *		mount -d RF_REMOTE_RESOURCE RF_REMOTE_MOUNT
 *		
 *		su
 *		fumount RF_RESOURCE
 *		rfstop	( ERROR - remote mounted resources in local file sys tree )
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		
 *		fumount RF_RESOURCE
 *		umount -d RF_REMOTE_RESOURCE
 *		su svvs
 *		rfstop	( ERROR - not root )
 *		su
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

	setup( "rfstop" );
	funct( "rfstop" );

	prep("Allocate and initialize buffers.");
	start
		char *malloc();
		exit_on( ERROR );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );
		rf_rmt = malloc( strlen(RF_REMOTE_MOUNT) + strlen(SVVSPATH) + 1 );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
		sprintf( rf_rmt, "%s%s", SVVSPATH, RF_REMOTE_MOUNT );
	finis

	abstract("A normal invocation of rfstop.  Set real uid to root.");
	abstract("Unmount and unadvertise resources. Invoke 'rfstop'.");
	abstract("Invoke 'rfstart'.  Mount and advertise resources.");
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
		break_on( ERROR );
		testing("Invoke 'rfstop'.");
		start
			sv_cmd( "rfstop", (char *)0 );
		finis
		continue_on( ERROR );
		cleanup("Invoke 'rfstart'.");
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
	abstract("Invoke 'rfstop' when rfs is not running.  Set uid to root.");
	abstract("unmount and unadvertise resources.  Invoke 'rfstop'");
	abstract("to stop rfs, and invoke 'rfstop' again, expecting failure.");
	abstract("Invoke 'rfstart'.  Mount and advertise resources.");
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
		break_on( ERROR );
		prep("Invoke 'rfstop', expecting success.");
		start
			sv_cmd( "rfstop", (char *)0 );
		finis
		testing("Invoke 'rfstop', expecting failure.");
		start
			expecting( FAILURE );
			sv_cmd( "rfstop", (char *)0 );
		finis
		chk_stderr( 1 );
		continue_on( ERROR );
		cleanup("Invoke 'rfstart'.");
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
	abstract("Invoke 'rfstop' when local resources are still advertised.");
	abstract("Set uid to root. Unmount remote resources.  Invoke");
	abstract("'rfstop', expecting failure.  Mount remote resources.");
	start
		exit_on( ERROR );
		prep("Set real uid to root.");
		start
			setuid( ROOT_UID );
		finis
		prep("Invoke 'umount -d %s'.", RF_REMOTE_RESOURCE );
		start
			sv_cmd( "umount", "-d", RF_REMOTE_RESOURCE, (char *)0 );
		finis
		break_on( ERROR );
		testing("Invoke 'rfstop', expecting failure.");
		start
			expecting( FAILURE );
			sv_cmd( "rfstop", (char *)0 );
		finis
		chk_stderr( 1 );
		continue_on( ERROR );
		cleanup("Invoke 'mount -d %s %s'.", RF_REMOTE_RESOURCE, rf_rmt);
		start
			sv_cmd( "mount", "-d", RF_REMOTE_RESOURCE, rf_rmt, (char *)0 );
		finis
	finis
	abstract("Invoke 'rfstop' when remote resources are mounted on this");
	abstract("system.  Set uid to root.  Fumount local resources.");
	abstract("Invoke 'rfstop', expecting failure.  Advertise local");
	abstract("resources.");
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
		testing("Invoke 'rfstop', expecting failure.");
		start
			expecting( FAILURE );
			sv_cmd( "rfstop", (char *)0 );
		finis
		chk_stderr( 1 );
		continue_on( ERROR );
		cleanup("Invoke 'adv -d %s %s %s'.", RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl );
		start
			sv_cmd( "adv", "-d",  RF_DESCRIPTION, 
		  RF_RESOURCE, rf_lcl, (char *)0 );
		finis
	finis
	
	abstract("Invoke 'rfstop' while not root.  Set real uid to root.");
	abstract("Unmount and unadvertise resources. Set uid to %d", UID);
	abstract("and invoke 'rfstop', expecting failure.  Set uid");
	abstract("back to root.  Mount and advertise resources.");
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
		prep("Fork a child process to verify 'rfstop' while ");
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
				testing("Invoke 'rfstop', expecting failure due to");
				testing("non-root.");
				start
					expecting( FAILURE );
					sv_cmd( "rfstop", (char *)0 );
				finis
				chk_stderr( 1 );
				done();
			}	
			expecting( pid );
			wait( &sts );
			if( sts )
				error("child process terminated abnormally.");
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
