/*
 *	adv.c
 *
 *	a test of adv(NS_CMD)
 *
 *	this file should be setuid root
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#include <sv_base.h>
#include <stdio.h>	
#include <fcntl.h>	
#include <varargs.h>
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>

#include <sv_rfs.h>

#ident	"@(#)adv.c	1.4"

/*
 *	test cases:
 *	
 *		su
 *		adv
 *		fumount RF_RESOURCE
 *		adv RF_RESOURCE RF_LOCAL
 *		fumount RF_RESOURCE
 *		adv RF_RESOURCE RF_LOCAL RF_DOMAIN.
 *		fumount RF_RESOURCE
 *		adv RF_RESOURCE RF_LOCAL RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv RF_RESOURCE RF_LOCAL RF_DOMAIN.RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv -r RF_RESOURCE RF_LOCAL
 *		fumount RF_RESOURCE
 *		adv -r RF_RESOURCE RF_LOCAL RF_DOMAIN.
 *		fumount RF_RESOURCE
 *		adv -r RF_RESOURCE RF_LOCAL RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv -r RF_RESOURCE RF_LOCAL RF_DOMAIN.RF_CLIENT
 *		fumount RF_RESOURCE
 *		mount -r SVVS_FS MOUNT_POINT
 *		adv -r RF_RD_RESOURCE MOUNT_POINT
 *		fumount RF_RD_RESOURCE
 *		unmount SVVS_FS
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		fumount RF_RESOURCE
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL RF_DOMAIN.
 *		fumount RF_RESOURCE
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL RF_DOMAIN.RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv -r -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL
 *		fumount RF_RESOURCE
 *		adv -r -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL RF_DOMAIN.
 *		fumount RF_RESOURCE
 *		adv -r -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv -r -d RF_DESCRIPTION RF_RESOURCE RF_LOCAL RF_DOMAIN.RF_CLIENT
 *		fumount RF_RESOURCE
 *		adv RF_RESOURCE RF_LOCAL
 *		adv -m RF_RESOURCE RF_DOMAIN.
 *		adv -m RF_RESOURCE RF_CLIENT
 *		adv -m RF_RESOURCE RF_DOMAIN.RF_CLIENT
 *		adv -m RF_RESOURCE -d RF_RD_DESCRIPTION
 *		adv -m RF_RESOURCE -d RF_DESCRIPTION RF_DOMAIN.
 *		adv -m RF_RESOURCE -d RF_RD_DESCRIPTION RF_CLIENT
 *		adv -m RF_RESOURCE -d RF_DESCRIPTION RF_DOMAIN.RF_CLIENT
 *		adv RF_RD_RESOURCE RF_LOCAL (ERROR - path is adv under diff name )
 *		adv RF_RESOURCE temppath (ERROR - resource !unique to server)
 *		fumount RF_RESOURCE
 *		mount -r SVVS_FS MOUNT_POINT
 *		adv RF_RESOURCE MOUNT_POINT (ERROR - path is readonly)
 *		unmount SVVS_FS
 *		adv RF_RESOURCE TMPFILE	(ERROR - path is not a directory)
 *		adv RF_RESOURCE SVVS_FS	(ERROR - path is not a directory)
 *		adv RF_RESOURCE. RF_LOCAL (ERROR - resource has '.' )
 *		adv RF_RESOURCE/ RF_LOCAL (ERROR - resource has '/' )
 *		umount -d RF_REMOTE_RESOURCE
 *		rfstop
 *		adv	( ERROR - rfs not running )
 *		rfstart
 *		adv RF_REMOTE_RESOURCE RF_LOCAL (ERROR - !unique w/in domain )
 *		mount RF_REMOTE_RESOURCE /svvs/svvs/NS/REMOTE
 *		adv RF_RESOURCE /svvs/svvs/NS/REMOTE (ERROR - mount pt for remote)
 *		adv RF_RESOURCE RF_LOCAL
 *		su svvs
 *		adv
 *		adv RF_RD_RESOURCE temppath ( ERROR - not root )
 *	
 *	caveats:
 *		if interrupted may leave rfs stopped, may leave spare fs mounted,
 *		may leave spare fs advertised, may leave RF_LOCAL fumount or adv
 *		with wrong resource name and/or description, may leave 
 *		RF_REMOTE_RESOURCE unmounted.
 *
 */

main() {
	char *domain;
	char *dom_client;
	char *rf_lcl;
	char *rf_rmt;
	char *rf_des;
	char *rf_rd_des;
	char *filename;

	setup( "adv" );
	funct( "adv" );
	
	prep("Initialize buffers and set real uid to root.");
	start
		char *malloc();
		exit_on( ERROR );
		domain = malloc( strlen(RF_DOMAIN) + 2 );
		dom_client = malloc( strlen(RF_DOMAIN) + strlen(RF_CLIENT) + 2 );
		rf_lcl = malloc( strlen(RF_LOCAL) + strlen(SVVSPATH) + 1 );

		rf_rmt = malloc( strlen(RF_REMOTE_MOUNT) + strlen(SVVSPATH) + 1 );
		rf_des = malloc( strlen(RF_DESCRIPTION) + 3 );
		rf_rd_des = malloc( strlen(RF_RD_DESCRIPTION) + 3 );
		filename = malloc( 512 );
		sprintf( domain, "%s.", RF_DOMAIN );
		sprintf( dom_client, "%s.%s", RF_DOMAIN, RF_CLIENT );
		sprintf( rf_lcl, "%s%s", SVVSPATH, RF_LOCAL );
		sprintf( rf_rmt, "%s%s", SVVSPATH, RF_REMOTE_MOUNT );
		sprintf( rf_des, "\"%s\"", RF_DESCRIPTION );
		sprintf( rf_rd_des, "\"%s\"", RF_RD_DESCRIPTION );
		setuid( ROOT_UID );
	finis

	abstract("Test cases of 'adv' without '-r', '-d', or '-m'.");
	start
		abstract("Invoke 'adv' with no arguments.");
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", (char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( rf_lcl );
			grp_stdout( RF_RESOURCE );
			grp_stdout( rf_des );
		finis

		abstract("Invoke 'adv %s %s'.", RF_RESOURCE, rf_lcl );
		start
			prep("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv %s %s %s.'.", RF_RESOURCE, rf_lcl,
		  RF_DOMAIN );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, domain, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, RF_DOMAIN, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv %s %s %s'.", RF_RESOURCE, rf_lcl, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, RF_CLIENT, 
				  (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, RF_CLIENT, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv %s %s %s.%s'.", RF_RESOURCE, rf_lcl,
		  RF_DOMAIN, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, dom_client,
				  (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, dom_client, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis
	finis

	abstract("Test cases of 'adv' with the '-r' option.");
	start
		abstract("Invoke 'adv -r %s %s'.", RF_RESOURCE, rf_lcl );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", RF_RESOURCE, rf_lcl, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -r %s %s %s.'.",
		  RF_RESOURCE, rf_lcl, RF_DOMAIN );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", RF_RESOURCE, rf_lcl, domain,
				  (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, RF_DOMAIN, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -r %s %s %s'.",
		  RF_RESOURCE, rf_lcl, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", RF_RESOURCE, rf_lcl, RF_CLIENT,
				  (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, RF_CLIENT, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -r %s %s %s.%s'.",
		  RF_RESOURCE, rf_lcl, RF_DOMAIN, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", RF_RESOURCE, rf_lcl, dom_client,
				  (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, dom_client, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Mount the spare file system read only, and advertise it");
		abstract("with 'adv -r %s %s'.",RF_RD_RESOURCE, MOUNT_POINT);
		start
			break_on( ERROR );
			prep("Invoke 'mount -r %s %s'.",SVVS_FS, MOUNT_POINT );
			start
				sv_cmd( "mount", "-r", SVVS_FS, MOUNT_POINT, (char *)0 );
			finis
			continue_on( ERROR );
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", RF_RD_RESOURCE, MOUNT_POINT,
				  (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( MOUNT_POINT, RF_RD_RESOURCE, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RD_RESOURCE );
			start
				sv_cmd( "fumount", RF_RD_RESOURCE, (char *)0 );
			finis
			cleanup("Invoke 'umount %s'.", SVVS_FS );
			start
				sv_cmd( "umount", SVVS_FS, (char *)0 );
			finis
		finis
	finis

	abstract("Test cases of 'adv' with the '-d' option.");
	start
		abstract("Invoke 'adv -d %s %s %s'.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -d %s %s %s %s.'.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl, RF_DOMAIN );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, domain, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, RF_DOMAIN, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -d %s %s %s %s'.",
		 RF_DESCRIPTION, RF_RESOURCE, rf_lcl, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, RF_CLIENT, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, RF_CLIENT, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis
	
		abstract("Invoke 'adv -d %s %s %s %s.%s'.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl, RF_DOMAIN, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, dom_client, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, dom_client, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis
	finis

	abstract("Test cases of 'adv' with the '-r' and '-d' options.");
	start
		abstract("Invoke 'adv -r -d %s %s %s'.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -r -d %s %s %s %s.'.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl, RF_DOMAIN );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", "-d", RF_DESCRIPTION, RF_RESOURCE, 
				  rf_lcl, domain, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, RF_DOMAIN, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -r -d %s %s %s %s'.",
		 RF_DESCRIPTION, RF_RESOURCE, rf_lcl, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, RF_CLIENT, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, RF_CLIENT, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis

		abstract("Invoke 'adv -r -d %s %s %s %s.%s'.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl, RF_DOMAIN, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-r", "-d", RF_DESCRIPTION, RF_RESOURCE,
				  rf_lcl, dom_client, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, dom_client, (char *)0 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis
	finis

	abstract("Test cases of 'adv' with the '-m' option.");
	start
		prep("Invoke 'adv %s %s'.",RF_RESOURCE, rf_lcl );
		start
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s %s.'.", RF_RESOURCE, RF_DOMAIN );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, domain, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, RF_DOMAIN, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s %s'.", RF_RESOURCE, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, RF_CLIENT, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, RF_CLIENT, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s %s.%s'.", RF_RESOURCE, RF_DOMAIN, 
		  RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, dom_client, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, dom_client, (char *)0 );
		finis
		cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
		start
			sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
		finis
	finis
	abstract("Test cases of 'adv' with the '-m' and '-d' options.");
	start
		prep("Invoke 'adv %s %s'.",RF_RESOURCE, rf_lcl );
		start
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s -d %s'.", RF_RESOURCE,
		  RF_RD_DESCRIPTION );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, "-d",
				   RF_RD_DESCRIPTION, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_rd_des, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s -d %s %s.'.",
		  RF_RESOURCE, RF_DESCRIPTION, RF_DOMAIN );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, "-d",
				  RF_DESCRIPTION, domain, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, RF_DOMAIN, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s -d %s %s'.",
		   RF_RESOURCE, RF_RD_DESCRIPTION, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, "-d",
				  RF_RD_DESCRIPTION, RF_CLIENT, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_rd_des, RF_CLIENT, (char *)0 );
		finis
		abstract("Invoke 'adv -m %s -d %s %s.%s'.",
		  RF_RESOURCE, RF_DESCRIPTION, RF_DOMAIN, RF_CLIENT );
		start
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv", "-m", RF_RESOURCE, "-d",
				  RF_DESCRIPTION, dom_client, (char *)0 );
			finis
			chk_stderr( 0 );
			chk_out( rf_lcl, RF_RESOURCE, rf_des, dom_client, (char *)0 );
		finis
		cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
		start
			sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
		finis
	finis
	abstract("Test cases of 'adv' which should fail due to errors.");
	start
		abstract("Invoke 'adv' with a pathname that is advertised");
		abstract("under a different resource name.");
		abstract("Invoke 'adv %s %s' followed by",RF_RESOURCE, rf_lcl );
		abstract("'adv %s %s'.", RF_RD_RESOURCE, rf_lcl );
		start
			prep("Invoke 'adv %s %s'.",RF_RESOURCE, rf_lcl );
			start
					sv_cmd( "adv", RF_RESOURCE, rf_lcl, (char *)0 );
			finis
			testing("Invoke 'adv %s %s'.", RF_RD_RESOURCE, rf_lcl );
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RD_RESOURCE, rf_lcl, (char *)0 );
			finis
			chk_stderr( 1 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis
		abstract("Invoke 'adv' with a resource name that is not");
		abstract("unique to the server.  Invoke ");
		abstract("'adv %s %s' followed by",RF_RESOURCE, rf_lcl );
		abstract("'adv %s %s'.", RF_RESOURCE, temp_path() );
		start
			prep("Invoke 'adv %s %s'.",RF_RESOURCE, rf_lcl );
			start
					sv_cmd( "adv", RF_RESOURCE, rf_lcl, (char *)0 );
			finis
			testing("Invoke 'adv %s %s'.", RF_RESOURCE, temp_path() );
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RESOURCE, temp_path(), (char *)0 );
			finis
			chk_stderr( 1 );
			cleanup("Invoke 'fumount %s'.", RF_RESOURCE );
			start
				sv_cmd( "fumount", RF_RESOURCE, (char *)0 );
			finis
		finis
		abstract("Invoke 'adv' for read/write on read only file system.");
		start
			break_on( ERROR );
			prep("Invoke 'mount -r %s %s'.",SVVS_FS, MOUNT_POINT );
			start
				sv_cmd( "mount", "-r", SVVS_FS, MOUNT_POINT, (char *)0 );
			finis
			continue_on( ERROR );
			prep("Invoke 'adv %s %s'.",RF_RESOURCE, MOUNT_POINT );
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RESOURCE, MOUNT_POINT, (char *)0 );
			finis
			chk_stderr( 1 );
			cleanup("Invoke 'umount %s'.", SVVS_FS );
			start
				sv_cmd( "umount", SVVS_FS, (char *)0 );
			finis
		finis
		abstract("Invoke 'adv' with a path name that is an");
		abstract("ordinary file.");
		start
			break_on( ERROR );
			prep("Create a temp file.");
			start
				close( temp_open( "adv.tmp", O_RDWR | O_CREAT, 0666 ));
			finis
			testing("Invoke 'adv %s %s%s'.",
			   RF_RESOURCE, temp_path(), "adv.tmp" );
			start
				sprintf( filename, "%s%s", temp_path(), "adv.tmp" );
				expecting( FAILURE );
				sv_cmd( "adv", RF_RESOURCE, filename, (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		abstract("Invoke 'adv' with a path name that is a");
		abstract("device.");
		start
			testing("Invoke 'adv %s %s'.", RF_RESOURCE, SVVS_FS );
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RESOURCE, SVVS_FS, (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		abstract("Invoke 'adv' with a resource name containing '.'.");
		start
			testing("Invoke 'adv foo. %s'.", rf_lcl );
			start
				expecting( FAILURE );
				sv_cmd( "adv", "foo.", rf_lcl, (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		abstract("Invoke 'adv' with a resource name containing '/'.");
		start
			testing("Invoke 'adv foo/ %s'.", rf_lcl );
			start
				expecting( FAILURE );
				sv_cmd( "adv", "foo/", rf_lcl, (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		abstract("Invoke 'adv' when rfs is not running.");
		start
			prep("Invoke 'umount -d %s'.", RF_REMOTE_RESOURCE );
			start
				sv_cmd( "umount", "-d", RF_REMOTE_RESOURCE, (char *)0 );
			finis
			prep("Invoke 'rfstop'.");
			start
				sv_cmd( "rfstop", (char *)0 );
			finis
			testing("Invoke 'adv %s %s'.", RF_RESOURCE, rf_lcl );
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RESOURCE, rf_lcl, (char *)0 );
			finis
			chk_stderr( 1 );
			cleanup("Invoke 'rfstart'.");
			start
				sv_cmd( "rfstart", (char *)0 );
			finis
			cleanup("Invoke 'mount -d %s %s'.",
			  RF_REMOTE_RESOURCE, rf_rmt );
			start
				sv_cmd( "mount", "-d", RF_REMOTE_RESOURCE,
				  rf_rmt, (char *)0 );
			finis
		finis
		abstract("Invoke 'adv' for a remote mount point.");
		start
			testing("Invoke 'adv %s %s'.", RF_RESOURCE, rf_rmt );
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RESOURCE, rf_rmt, (char *)0 );
			finis
			chk_stderr( 1 );
		finis
		cleanup("Invoke 'adv -d %s %s %s' to advertise the local resource.",
		  RF_DESCRIPTION, RF_RESOURCE, rf_lcl );
		start
			sv_cmd( "adv", "-d", RF_DESCRIPTION, RF_RESOURCE, rf_lcl, (char *)0 );
		finis
	finis
	abstract("Test cases of adv while running as non-root.");
	start
		abstract("Invoke 'adv' with no arguments while running as");
		abstract("non-root.");
		start
			break_on( ERROR );
			prep("Call setuid to set user id to %d.",UID);
			start
				setuid( UID );
			finis
			testing("Invoke 'adv'.");
			start
				sv_cmd( "adv",(char *)0 );
			finis
			chk_stdout( 1 );
			chk_stderr( 0 );
			grp_stdout( rf_lcl );
			grp_stdout( RF_RESOURCE );
			grp_stdout( rf_des );
		finis			
		abstract("Invoke 'adv' to advertise a resource while running as");
		abstract("non-root.  Verify that 'adv' fails.");
		start
			break_on( ERROR );
			prep("Call setuid to set user id to %d.",UID);
			start
				setuid( UID );
			finis
			testing("Invoke 'adv %s %s'.",RF_RD_RESOURCE, temp_path());
			start
				expecting( FAILURE );
				sv_cmd( "adv", RF_RD_RESOURCE, temp_path(), (char *)0 );
			finis
			chk_stderr( 1 );
		finis			
	finis
	done();
}
chk_out( va_alist )
va_dcl
{
	va_list	ap;
	char	*string;

	testing("Invoke 'adv' to verify the resource was advertised.");
	start
		break_on( ERROR );
		sv_cmd( "adv", (char *)0 );
		chk_stdout( 1 );
		chk_stderr( 0 );
		va_start( ap );
			while( string = va_arg( ap, char * ) )
				grp_stdout( string );
		va_end( ap );
	finis
}
