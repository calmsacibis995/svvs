/*
 *	t_sync0.c
 *
 *	subtest for the SVVS test of t_sync(NS_LIB)
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)t_sync0.c	1.4"

#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <signal.h>
#include <svvs.h>
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

main() {
	int	i;
	int	tfd;
	int	fd;
	int state;
	int	kludge;
	char *st_name;
	char *sname();

	for( i = 0 ; i < OPEN_MAX ; i++ )
		no_close( i );

	setup("t_sync");
	prep("Read the file descriptor and state from the temp file.");
	start
		tfd = temp_open( "t_sync.tmp", O_RDONLY );
		expecting( sizeof( int  ) );
		read( tfd, &fd, sizeof( int ) );		
		expecting( sizeof( int  ) );
		read( tfd, &state, sizeof( int ) );		
		expecting( sizeof( int  ) );
		read( tfd, &kludge, sizeof( int ) );		
		close( tfd );
	finis
	st_name = sname( state );
	testing("Call t_sync, verify that the state returned is %s.",st_name);
	start
		if( state == T_INREL || state == T_OUTREL ) {
			int st;
			expecting( VOID );
			if ( (st = t_sync( fd )) != state ) {
				if( st == T_OUTREL && state == T_INREL ) {
					if( kludge )
						warning("%s\nt_sync returned T_OUTREL instead of T_INREL",ADVISORY);
				} else if( st == T_INREL && state == T_OUTREL ) {
					if( kludge )
						warning("%s\nt_sync returned T_INREL instead of T_OUTREL",ADVISORY);
				} else {
					expecting( state );
					t_sync( fd );
				}
			}
		}
		else {
			expecting( state );
			t_sync( fd );
		}
	finis
	done();
}
