/*
 *	t_snd.c
 *
 *	a test of t_snd(NS_LIB) and t_rcv(NS_LIB)
 *	
 *	tabstops should be set to 4 to view this file
 *
 *	Note:	this test assumes that EPROTO errors
 *		are not detected until the following 
 *		function call.
 */

#ident	"@(#)t_snd.c	1.5"

#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <signal.h>
#include <svvs.h>
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#define rendevous()	( v_sema(), p_sema() )

#define	debug(foo)	/*	printf foo ; fflush( stdout )	*/

char	*fnct;
char	tmpfyle[128];

#define	SND	(fn == t_snd)
#define	RCV	(fn == t_rcv)

int		(*fn)() = NULL;
int		strt;
int		t_snd();
int		t_rcv();

#define	min( a, b )		(( a < b ) ? a : b )
#define	max( a, b )		(( a > b ) ? a : b )
#define	tmore( x )		( x & T_MORE )
#define	expedited( z )	( z & T_EXPEDITED )

void	chk_unbnd();
void	chk_incon();
void	chk_outcon();
void	chk_idle();
void	chk_dataxfer();
void	chk_inrel();
void	chk_outrel();
void	do_open();
void	do_close();
void	do_alloc();
void	do_free();
void	do_bind();
void	do_unbind();
void	do_sndrcv();
void	set_ndelay();
void	clr_ndelay();
void	set_flags();
void	clr_flags();

char	*t_alloc();

struct endpoint {
	char	*provider;
	char	*addr;
	int		alen;
	char	*conn;
	int		clen;
	int		qlen;
	int		fd;
} tp[] = {
	{	NULL,	NULL,	0,	NULL,	0,	1,	-1,	},
	{	NULL,	NULL,	0,	NULL,	0,	0,	-1,	},
};

#define NPROVIDERS	(sizeof( tp )/sizeof(struct endpoint))

struct	t_info		info[ NPROVIDERS ];			
struct	t_bind		*req[ NPROVIDERS ];
struct	t_bind		*ret[ NPROVIDERS ];
struct	t_call		*sndcall[ NPROVIDERS ];
struct	t_call		*rcvcall[ NPROVIDERS ];
struct	t_call		*scall[ NPROVIDERS ];
struct	t_call		*rcall[ NPROVIDERS ];


char	*sbuf;		/* send buffer	*/
char	*rbuf;		/* rcv	buffer	*/
int		bsize;		/* buffer size	*/

int states[] = { 
	T_UNINIT, 
	T_UNBND,
	T_IDLE,
	T_OUTCON,
	T_DATAXFER,
	T_INREL,
	T_OUTREL 
};

int flags[] = {
	0,
	T_MORE,
	T_EXPEDITED,
	T_MORE | T_EXPEDITED,
};

main(argc,argv) 
int	argc;
char	*argv[];
{
	int	state_id;
	int	flag_id;
	int	async;


	for( fnct = argv[0]; *fnct; fnct++ );
	while( fnct > argv[0] && *(fnct-1) != '/' ) fnct--;

	setup(fnct);
	funct(fnct);

	prep("Initialize variables and structures.");
	start
		expecting( VOID );
		if( !strcmp( fnct, "t_snd" ) ) {
			strt = 0;
			fn = t_snd;
		}
		else if( !strcmp( fnct, "t_rcv" ) ) {
			strt = 1;
			fn = t_rcv;
		}
		else
			severr("%s%s%s%s",
			  "Unknown name '",fnct,"'\n",
			  "expected:	t_snd or t_rcv.");
		strcpy(tmpfyle,fnct);
		strcat(tmpfyle,".tmp");

		tp[0].provider = TPVCFILE0;
		tp[0].addr = TPVCADDR0;
		tp[0].alen = TPVCALEN0;
		tp[0].conn = TPVCCONN0;
		tp[0].clen = TPVCCLEN0;

		tp[1].provider = TPVCFILE1;	
		tp[1].addr = TPVCADDR1;
		tp[1].alen = TPVCALEN1;
		tp[1].conn = TPVCCONN1;
		tp[1].clen = TPVCCLEN1;
	finis

	prep("Initialize semaphore mechanism.");
	start
		seminit( 1 );
	finis

	init_endpoint( 0 );
	init_endpoint( 1 );

	prep("Allocate some buffer space for the send and receive buffers.");
	start
		int	size0, size1;
		char	*malloc();

		abort_on( ERROR );
		size0 = max( info[0].tsdu, info[0].etsdu );
		size1 = max( info[1].tsdu, info[1].etsdu );
		bsize = max( size0, size1 );
		if( !bsize ) 
			bsize = 4096;
		sbuf = malloc( bsize + 1 );
		rbuf = malloc( bsize + 1 );
	finis

	rendevous();

	for( state_id = 4; state_id <= 6; state_id++ ) {

		if( get_to_state( states[state_id] ) ) {

			rendevous();

			for( flag_id = 0; flag_id <= 3; flag_id++ ) {
			for( async = 0 ; async <= 1 ; async++ ) {
				abstract("Exercise the communications between the");
				abstract("two endpoints.");
				switch( states[ state_id ]  ) {
					case	T_DATAXFER:
						abstract("Both the active and passive endpoints");
						abstract("are in the T_DATAXFER state.");
						break;
					case	T_INREL:
						abstract("The active and passive endpoints are");
						abstract("in the T_INREL and T_OUTREL states");
						abstract("respectively.");
						break;
					case	T_OUTREL:
						abstract("The active and passive endpoints are");
						abstract("in the T_OUTREL and T_INREL states");
						abstract("respectively.");
						break;
				}
				if (SND) {
					if( tmore( flags[ flag_id ] ) ) {
						abstract("T_MORE is set in the flags passed");
					}
					else {
						abstract("T_MORE is not set in the flags passed");
					}
					abstract("to t_snd.");
				}
				if (RCV) {
					if( tmore( flags[ flag_id ] ) ) {
						abstract("The receiver buffer size is reduced to");
						abstract("set T_MORE in the returned flags.");
					}			
				}
				if( expedited( flags[ flag_id ] ) )
						abstract("Expedited data is transferred.");
				else
					abstract("Normal (not expedited) data is transferred.");
	
				if( async )
					abstract("Endpoints are used asynchronously.");
				else
					abstract("Endpoints are used synchronously.");
	
				abstract("Messages of all boundary sizes (any size which");
				abstract("is within +/- 1 of a power of two) between 0 ");
				if( expedited( flags[ flag_id ] ) )
					abstract("and etsdu size are used.");
				else
					abstract("and tsdu size are used.");
		
				start
					do_sndrcv( async, flags[flag_id], states[state_id] );
				finis

				rendevous();
			}}
		}
		rendevous();
		cleanup("Call t_snddis from the active user and t_rcvdis from");
		cleanup("the passive user to return both to the T_IDLE state.");
		start
			t_snddis( tp[1].fd, NULL );
			rendevous();
			t_rcvdis( tp[0].fd, NULL );
		finis
	}
	rendevous();

	if	(RCV) {

		get_to_state( T_DATAXFER );

		rendevous();

		abstract("Check that t_rcv in asynchronous mode fails and sets");
		abstract("t_errno to TNODATA when no data units are available.");
		start
			set_ndelay( 0 );
			testing("Call t_rcv.");
			start
				int rflags;
				expecting( TNODATA );
				t_rcv( tp[0].fd, rbuf, bsize, &rflags );
			finis
			clr_ndelay( 0 );
		finis

		rendevous();
	
		abstract("Check that t_rcv in synchronous mode blocks");
		abstract("when no data units are available.  Set an alarm");
		abstract("and verify that when the SIGALRM occurs t_rcv");
		abstract("fails and sets t_errno to TSYSERR and errno to EINTR.");
		start
			clr_ndelay( 0 );
			prep("Set signal catching for SIGALRM.");
			start
				void sigfoo();
				signal( SIGALRM, sigfoo );
			finis
			prep("Set a 10 second alarm.");
			start
				expecting( 0 );
				alarm( 10 );
			finis
			testing("Call t_rcv.");
			start
				int rflags;
				expecting( TSYSERR );
				expecting( EINTR );
				t_rcv( tp[0].fd, rbuf, bsize, &rflags );
			finis
		finis
	
		rendevous();
	
		abstract("The transmitter calls t_snddis to send a disconnect.");
		abstract("Call t_rcv and verify that t_rcv fails and sets");
		abstract("t_errno to TLOOK.");
		start
			prep("Call t_snddis.");
			start		
				t_snddis( tp[1].fd, NULL );
			finis
			rendevous();
			testing("Call t_rcv.");
			start
				int rflags;
				expecting( TLOOK );
				t_rcv( tp[0].fd, rbuf, bsize, &rflags );
			finis
			cleanup("Call t_rcvdis to clear the disconnect indication.");
			start
				t_rcvdis( tp[0].fd, NULL );
			finis
		finis
	}

	rendevous();

	if	(SND) {

		get_to_state( T_DATAXFER );
	
		rendevous();
	
		abstract("Call t_snd many times asynchronously to");
		abstract("attempt to generate a TFLOW error.");
		abstract("After generating a TFLOW error, call t_snd again");
		abstract("synchronously to verify t_snd in synchronous mode");
		abstract("blocks when flow control prevents the data from being");
		abstract("accepted. Set an alarm and verify that when SIGALRM");
		abstract("occurs, t_snd fails and sets t_errno to TSYSERR and");
		abstract("errno to EINTR.  Then call t_rcv to clear the");
		abstract("receiver endpoint.  Data is sent from the active user");
		abstract("to the passive user.");
		start
			do_tflow( 1, 0 );
		finis

		rendevous();

		abstract("Call t_snd many times asynchronously to");
		abstract("attempt to generate a TFLOW error.");
		abstract("After generating a TFLOW error, call t_snd again");
		abstract("synchronously to verify t_snd in synchronous mode");
		abstract("blocks when flow control prevents the data from being.");
		abstract("accepted. Set an alarm and verify that when SIGALRM");
		abstract("occurs, t_snd fails and sets t_errno to TSYSERR and");
		abstract("errno to EINTR.  Then call t_rcv to clear the");
		abstract("receiver endpoint.  Data is sent from the passive user");
		abstract("to the active user.");
		start
			do_tflow( 0, 1 );
		finis

		rendevous();

		cleanup("Call t_snddis from the active user and t_rcvdis from");
		cleanup("the passive user to return to the T_IDLE state.");
		start
			t_snddis( tp[1].fd, NULL );
			rendevous();
			t_rcvdis( tp[0].fd, NULL );
		finis

		rendevous();

		deinit_endpoint( 0 );
		deinit_endpoint( 1 );

		rendevous();

		abstract("Call t_snd from the active user with a data size");
		abstract("larger than [e]tsdu.  Call t_snd again with a data");
		abstract("size of 1.  Verify the second t_snd fails and sets");
		abstract("t_errno to TSYSERR and errno to EPROTO.");
		start
			abstract("Call t_snd from the active user with normal data.");
			start
				if( !info[1].tsdu )
					message("Concept of tsdu not supported by '%s'.",
					  tp[1].provider);
				else if( info[1].tsdu == -1 )
					message("Finite tsdu not supported by '%s'.",
					  tp[1].provider);
				else if( info[1].tsdu < -1 )
					message("tsdu not supported by '%s'.",
					  tp[1].provider);
				else {
					init_endpoint( 0 );
					init_endpoint( 1 );
					get_to_state( T_IDLE );
					prep("Call t_snd with an invalid size expecting");
					prep("success.");
					start
						t_snd( tp[1].fd, sbuf, info[1].tsdu+1, 0 );
					finis
					testing("Call t_snd with a size of 1.  Failure");
					testing("is expected due to the previous call.");
					start
						expecting( TSYSERR );
						expecting( EPROTO );
						t_snd( tp[1].fd, sbuf, 1, 0 );
					finis
					cleanup("Call t_close to close the active and");
					cleanup("passive endpoints.  No return value check");
					cleanup("is made, as there is an EPROTO condition.");
					start
						expecting( VOID );
						t_close( tp[0].fd );	
						t_close( tp[1].fd );
					finis		
				}
			finis
			testing("Call t_snd from the active user with expedited");
			testing("data");
			start
				if( !info[1].etsdu )
					message("Concept of etsdu not supported by '%s'.",
					  tp[1].provider);
				else if( info[1].etsdu == -1 )
					message("Finite etsdu not supported by '%s'.",
					  tp[1].provider);
				else if( info[1].etsdu < -1 )
					message("etsdu not supported by '%s'.",
					  tp[1].provider);
				else {
					init_endpoint( 0 );
					init_endpoint( 1 );
					get_to_state( T_IDLE );
					prep("Call t_snd with an invalid size expecting");
					prep("success.");
					start
						t_snd(tp[1].fd,sbuf, info[1].etsdu+1, T_EXPEDITED);
					finis
					testing("Call t_snd with a size of 1.  Failure");
					testing("is expected due to the previous call.");
					start
						expecting( TSYSERR );
						expecting( EPROTO );
						t_snd( tp[1].fd, sbuf, 1, T_EXPEDITED );
					finis
					cleanup("Call t_close to close the active and");
					cleanup("passive endpoints.  No return value check");
					cleanup("is made, as there is an EPROTO condition.");
					start
						expecting( VOID );
						t_close( tp[0].fd );	
						t_close( tp[1].fd );
					finis		
				}
			finis
			testing("Call t_snd from the passive user with normal data.");
			start
				if( !info[0].tsdu )
					message("Concept of tsdu not supported by '%s'.",
					  tp[0].provider);
				else if( info[0].tsdu == -1 )
					message("Finite tsdu not supported by '%s'.",
					  tp[0].provider);
				else if( info[0].tsdu < -1 )
					message("tsdu not supported by '%s'.",
					  tp[0].provider);
				else {
					init_endpoint( 0 );
					init_endpoint( 1 );
					get_to_state( T_IDLE );
					prep("Call t_snd with an invalid size expecting");
					prep("success.");
					start
						t_snd( tp[0].fd, sbuf, info[0].tsdu+1, 0 );
					finis
					testing("Call t_snd with a size of 1.  Failure");
					testing("is expected due to the previous call.");
					start
						expecting( TSYSERR );
						expecting( EPROTO );
						t_snd( tp[0].fd, sbuf, 1, 0 );
					finis
					cleanup("Call t_close to close the active and");
					cleanup("passive endpoints.  No return value check");
					cleanup("is made, as there is an EPROTO condition.");
					start
						expecting( VOID );
						t_close( tp[0].fd );	
						t_close( tp[1].fd );
					finis		
				}
			finis
			testing("Call t_snd from the passive user with expedited");
			testing("data.");
			start
				if( !info[0].etsdu )
					message("Concept of etsdu not supported by '%s'.",
					  tp[0].provider);
				else if( info[0].etsdu == -1 )
					message("Finite etsdu not supported by '%s'.",
					  tp[0].provider);
				else if( info[0].etsdu < -1 )
					message("etsdu not supported by '%s'.",
					  tp[0].provider);
				else {
					init_endpoint( 0 );
					init_endpoint( 1 );
					get_to_state( T_IDLE );
					prep("Call t_snd with an invalid size expecting");
					prep("success.");
					start
						t_snd(tp[0].fd,sbuf, info[0].etsdu+1, T_EXPEDITED);
					finis
					testing("Call t_snd with a size of 1.  Failure");
					testing("is expected due to the previous call.");
					start
						expecting( TSYSERR );
						expecting( EPROTO );
						t_snd( tp[0].fd, sbuf, 1, T_EXPEDITED );
					finis
					cleanup("Call t_close to close the active and");
					cleanup("passive endpoints.  No return value check");
					cleanup("is made, as there is an EPROTO condition.");
					start
						expecting( VOID );
						t_close( tp[0].fd );	
						t_close( tp[1].fd );
					finis		
				}
			finis
		finis

		rendevous();

		abstract("Call t_snd from invalid states.   Verify that t_snd");
		abstract("fails and sets t_errno to TSYSERR and errno to EPROTO.");
		start
			abstract("Bring the transmitter and receiver to the T_OUTCON");
			abstract("and T_INCON states.  Call t_snd twice from the");
			abstract("T_OUTCON state.  The second call should fail.");
			start
				init_endpoint( 0 );
				init_endpoint( 1 );
				get_to_state( T_OUTCON );
				prep("Call t_snd, expecting success.");
				start
					t_snd( tp[1].fd, sbuf, 1, 0 );
				finis
				testing("Call t_snd, expecting failure.");
				start
					expecting( TSYSERR );
					expecting( EPROTO );
					t_snd( tp[1].fd, sbuf, 1, 0 );
				finis
				cleanup("Call t_close to close the active and");
				cleanup("passive endpoints.  No return value check");
				cleanup("is made, as there is an EPROTO condition.");
				start
					expecting( VOID );
					t_close( tp[0].fd );	
					t_close( tp[1].fd );
				finis		
			finis
			abstract("Bring the transmitter and receiver to the T_OUTCON");
			abstract("and T_INCON states.  Call t_snd twice from the");
			abstract("T_INCON state.  The second call should fail.");
			start
				init_endpoint( 0 );
				init_endpoint( 1 );
				get_to_state( T_OUTCON );
				prep("Call t_snd, expecting success.");
				start
					t_snd( tp[0].fd, sbuf, 1, 0 );
				finis
				testing("Call t_snd, expecting failure.");
				start
					expecting( TSYSERR );
					expecting( EPROTO );
					t_snd( tp[0].fd, sbuf, 1, 0 );
				finis
				rendevous();
				cleanup("Call t_close to close the active and");
				cleanup("passive endpoints.  No return value check");
				cleanup("is made, as there is an EPROTO condition.");
				start
					expecting( VOID );
					t_close( tp[0].fd );	
					t_close( tp[1].fd );
				finis		
			finis

			abstract("Bring the transmitter and receiver to the T_INREL");
			abstract("and T_OUTREL states.  Call t_snd twice from the");
			abstract("T_OUTREL state.  The second call should fail.");
			start
				init_endpoint( 0 );
				init_endpoint( 1 );
				if( get_to_state( T_INREL ) ) {
					prep("Call t_snd, expecting success.");
					start
						t_snd( tp[0].fd, sbuf, 1, 0 );
					finis
					testing("Call t_snd from the T_OUTREL state of the");
					testing("passive user.");
					start
						expecting( TSYSERR );
						expecting( EPROTO );
						t_snd( tp[0].fd, sbuf, 1, 0 );
					finis
				}
				rendevous();
				cleanup("Call t_close to close the active and");
				cleanup("passive endpoints.  No return value check");
				cleanup("is made, as there is an EPROTO condition.");
				start
					expecting( VOID );
					t_close( tp[0].fd );	
					t_close( tp[1].fd );
				finis		
			finis

			abstract("Bring the transmitter and receiver to the T_OUTREL");
			abstract("and T_INREL states.  Call t_snd twice from the");
			abstract("T_OUTREL state.  The second call should fail.");
			start
				init_endpoint( 0 );
				init_endpoint( 1 );
				if( get_to_state( T_OUTREL ) ) {
					prep("Call t_snd, expecting success.");
					start
						t_snd( tp[1].fd, sbuf, 1, 0 );
					finis
					testing("Call t_snd from the T_OUTREL state of the");
					testing("active user.");
					start
						expecting( TSYSERR );
						expecting( EPROTO );
						t_snd( tp[1].fd, sbuf, 1, 0 );
					finis
				}
				rendevous();
				cleanup("Call t_close to close the active and");
				cleanup("passive endpoints.  No return value check");
				cleanup("is made, as there is an EPROTO condition.");
				start
					expecting( VOID );
					t_close( tp[0].fd );	
					t_close( tp[1].fd );
				finis		
			finis

			abstract("Call t_snd from the T_UNBND state.");
			start
				do_open( 1 );
				do_alloc( 1 );
				chk_unbnd( 1 );
				prep("Call t_snd, expecting success.");
				start
					t_snd( tp[1].fd, sbuf, 1, 0 );
				finis
				testing("Call t_snd, expecting failure.");
				start
					expecting( TSYSERR );
					expecting( EPROTO );
					t_snd( tp[1].fd, sbuf, 1, 0 );
				finis
				cleanup("Call t_close to close the transmitter provider.");
				cleanup("No check of return value is done due to the");
				cleanup("existing EPROTO condition.");
				start
					t_close( tp[1].fd );
				finis
			finis
		finis
	}
	rendevous();

	if( !SND ) {
		deinit_endpoint( 1 );
		deinit_endpoint( 0 );
	}

	abstract("Open a temp file.  Call %s with the file", fnct);
	abstract("file descriptor of the temp file.  Verify that");
	abstract("%s fails and sets t_errno to TBADF.",fnct);
	start
		int rflags;
		int fd;

		break_on( ERROR );

		prep("Open a temp file: '%s'", tmpfyle);
		start
			fd = temp_open( tmpfyle, O_RDWR | O_CREAT, 0664 );
		finis
		testing("Call %s with the file descriptor for the temp", fnct);
		testing("file.  Verify %s fails with t_errno set to", fnct);
		testing("TBADF.");
		start
			expecting( TBADF );
			if (RCV)
				t_rcv( fd, rbuf, bsize, &rflags );
			if (SND)
				t_snd( fd, sbuf, 1, 0 );
		finis
		cleanup("Close the temp file.");
		start
			close(fd);
		finis
	finis

	abstract("Call %s with a connectionless mode provider.",fnct);
	start
		prep("Call t_open to open the transport provider: '%s'.",
		 TPDGFILE0 );
		start
			tp[0].fd = t_open( TPDGFILE0, O_RDWR, &info[ 0 ]);
		finis
		chk_unbnd( 0 );
		do_alloc( 0 );
		do_bind( 0 );
		testing("Call %s.",fnct);
		start
			int rflags;
			expecting( TNOTSUPPORT );
			if (RCV)
				t_rcv( tp[0].fd, rbuf, bsize, &rflags );
			if (SND)
				t_snd( tp[0].fd, sbuf, 1, 0 );
		finis
		do_free( 0 );
		do_close( 0 );
	finis
	done();
}
void do_sndrcv( async, flags, state ) 
int async;
int	flags;
int state;
{
	if( async ) {
		set_ndelay( 0 );
		set_ndelay( 1 );
	}
	else {
		clr_ndelay( 0 );
		clr_ndelay( 1 );
	}
	rendevous();

	if (SND) {
		if( !tmore( flags ) ) {
			if( state != T_OUTREL ) {
				abstract("Call t_snd with a data size of 0.  Verify that");
				abstract("no data is sent with a subsequent call to");
				abstract("t_rcv.  Data is sent from the active user to");
				abstract("the passive user.");
				start
					do_sr0( 1, 0, async, flags );
				finis
			}
			if( state != T_INREL ) {
				abstract("Call t_snd with a data size of 0.  Verify that");
				abstract("no data is sent with a subsequent call to");
				abstract("t_rcv.  Data is sent from the passive user to");
				abstract("the active user.");
				start
					do_sr0( 0, 1, async, flags );
				finis
			}
		}
	}
	if( state != T_OUTREL ) {
		abstract("Exercise sending from the active to passive user.");
		start
			do_sr( 1, 0, async, flags );
		finis
	}

	rendevous();

	if( state != T_INREL ) {
		abstract("Exercise sending from the passive to active user.");
		start
			do_sr( 0, 1, async, flags );
		finis
	}
}
do_sr( x, r, async, flags )
int x, r, async, flags;
{
	int xcases;
	int	rcases;
	int	i,j;
	int sum, cnt, rflags;

	prep("Call t_snd to send the number of outgoing test cases.");
	start
/*
 *		put xcases in ascii in case of different reps for 2 machines 
 */
		xcases = expedited( flags ) ? info[x].etsdu : info[x].tsdu;
		if( xcases <= 0 ) {
			switch( xcases ) {
			case 0:
				if( tmore( flags )  ) {
					message(
					  "concept of tsdu not supported by this provider.");
					xcases = 0;
				}
				else
					xcases = 4096;					
				break;
			case -1:
				xcases = 4096;
				break;
			case -2:
				message("[e]tsdu not supported by this provider.");
				xcases = 0;
				break;
			}
		}
		if( xcases ) {
			sprintf(sbuf,"%d",xcases);
			t_snd( tp[x].fd, sbuf, strlen(sbuf)+1, expedited( flags ) );
		}
		debug(("outgoing test cases: %s",sbuf));
	finis
	rendevous();
	prep("Call t_rcv to receive the number of incoming test cases.");
	start
		int	ret;
		set_ndelay( r );
		expecting( VOID );
		if( t_rcv( tp[r].fd, rbuf, bsize, &rflags ) <= 0)
			rcases = 0;
		else
			sscanf(rbuf,"%d",&rcases);
		if( !async ) 
			clr_ndelay( r );
		debug(("incoming test cases: %d",rcases));
	finis

	rendevous();

	testing("Call t_snd and t_rcv for the number of outgoing");
	testing("and incoming test cases respectively.");
	start
		int rbsize = bsize;
		if (RCV) {
			if ( tmore( flags ) )
				if( expedited( flags ) ) {
					if( !info[r].etsdu )
						message("Concept of etsdu not supported for '%s'.",
						  tp[r].provider);
					else if( info[r].etsdu == -2 )
						message("etsdu not supported for '%s'.",
						  tp[r].provider);
					else
						rbsize = rcases/4;
				} 
				else {
					if( !info[r].tsdu )
						message("Concept of tsdu not supported for '%s.",
						  tp[r].provider);
					else if( info[r].tsdu == -2 )
						message("tsdu not supported for receiver.");
					else
						rbsize = rcases/4;
				}
		}
		for( i = 1; i <= max( xcases, rcases ); i = bound( i ) ) {
			if( async )
				rendevous();
			debug(("%4d ",i));
			if( i <= xcases ) {
				debug(("S",i));
				fillbuf( sbuf, i );
				if( RCV || (SND && !tmore( flags )) ) {
					if( !async ) {
						expecting( i );
						t_snd( tp[x].fd, sbuf, i, expedited( flags ) );
					}
					else {
/*
 *						async may not send all on first request
 */
						int sum = 0;
						int	ret;
						do {
							ret = t_snd(tp[x].fd, sbuf+sum, 
							  i-sum, expedited( flags ));
							if( ret < 0 ) break;
							sum += ret;
						} while( sum < i );
					}
				}
				else {
					sum = 0;
					if( !async ) {
						for( j = i/10+1; sum+j < i; j++) {
							debug(("[%d:%d]",j,sum));
							expecting( j );
							t_snd( tp[x].fd, &sbuf[sum], j, flags );
							debug(("\n"));
							sum += j;
						}
						expecting( i - sum );
						t_snd( tp[x].fd, &sbuf[sum], (i-sum), 
						  expedited( flags ) );
					}
					else {
						int	sum2, ret;
						for( j = i/10+1; sum+j < i; j++) {
							sum2 = 0;
							do {
								ret = t_snd(tp[x].fd, &sbuf[sum]+sum2, 
								  j-sum2, flags );
								if( ret < 0 ) break;
								sum2 += ret;
							} while( sum2 < j );
							sum += j;
						}
						sum2 = 0;
						do {
							ret = t_snd(tp[x].fd, &sbuf[sum]+sum2, 
							  i-sum-sum2, expedited( flags ));
							if( ret < 0 ) break;
							sum2 += ret;
						} while( sum+sum2 < i );
					}
				}
				debug(("S",i));
			}
			if( async )
				rendevous();
			if( i <= rcases ) {
				debug(("R",i));
				sum = 0;
				if ( expedited( flags ) ? info[r].etsdu : info[r].tsdu ) {
					do {
						cnt = t_rcv( tp[r].fd, rbuf, rbsize, &rflags );
						chkbuff( rbuf, cnt, sum, i );
						if( (i-sum) > rbsize && !tmore( rflags  )  ) {
							error("T_MORE not set when expected.");
							break;
						}
						if( !expedited( rflags )) {
							static int xerr1 = 0;
							if( !sum && expedited( flags ) && !xerr1 ) {
								error(
		 						  "T_EXPEDITED not set when expected.");
								xerr1++;
							}
						} 
#ifdef UNDEF
/*
 *						check of expedited cleared on tmore removed
 */
						else {
							static int xerr0 = 0;
							if ( !xerr0 && (sum || !expedited( flags ) )) {
								error(
								  "T_EXPEDITED set when not expected.");
								xerr0++;
							}
						}
#endif
						sum += cnt;
					} while( (i > sum) && tmore( rflags ) );
					if ( tmore( rflags ) )
						error("T_MORE set after all data received.");
				}
				else {
					do {
						static	int	xerr2 = 0;
						cnt = t_rcv( tp[r].fd, rbuf, i-sum, &rflags );
						if( !xerr2 && expedited(flags) && 
						  !expedited(rflags) )
							error("T_EXPEDITED not set when expected.");
						chkbuff( rbuf, cnt, sum, i );
						sum += cnt;
					} while( sum < i );
				}
				debug(("R",i));
			}
			debug((" "));
		}
	finis
}
do_sr0( x, r, async, flags )
int x, r, async, flags;
{
	testing("Call t_snd.");
	start
		char z = 'Z';
		t_snd( tp[x].fd, &z, 0, expedited( flags ) );
	finis

	rendevous();

	set_ndelay( r );

	testing("Call t_rcv with NDELAY set to verify that no");
	testing("data was sent.");
	start
		int rflags;
		char	z;
		expecting( TNODATA );		
		t_rcv( tp[r].fd, &z, 1, &rflags );
	finis
	if( !async )
		clr_ndelay( r );
	rendevous();
}
do_tflow( x, r )
int x, r;
{
	int	size;

	size = info[x].tsdu;
	if( !size ) {
		message("concept of tsdu not supported for this provider.");
		return;
	}
	if( size == -1  ) {
		message("finite tsdu not supported for this provider.");
		return;
	}
	if( size < -1  ) {
		message("tsdu not supported for this provider.");
		return;
	}
	prep("Set signal catching for SIGALRM.");
	start
		void sigfoo();
		signal( SIGALRM, sigfoo );
	finis
	set_ndelay( x );
	testing("Call t_snd with a data size of %d",size);
	testing("10000 times or until a TFLOW error occurs.");
	start
		int	i;
		expecting( VOID );
		for( i = 0; i < 10000; i++ ) {
			if( t_snd( tp[x].fd, sbuf, size, 0 ) < 0 ){
				expecting( TFLOW );
				t_snd( tp[x].fd, sbuf, size, 0 );
				break;
			}
		}
		if( i == 10000 ) {
			warning("Could not generate TFLOW error in 10K attempts.");
			goto recover;
		}
	finis
	clr_ndelay( x );
	prep("Set a 10 second alarm clock.");
	start
		expecting( 0 );
		alarm( 10 );
	finis
	testing("Call t_snd, expecting to block due to a flow");
	testing("restriction.  Expect the pending SIGALRM to make");
	testing("t_snd fail.");
	start
		expecting( TSYSERR );
		expecting( EINTR );
		t_snd( tp[x].fd, sbuf, size, 0 );
	finis
recover:
	rendevous();
	set_ndelay( r );
	cleanup("Call t_rcv asynchronously until TNODATA occurs.");
	start
		int rflags;

		expecting( VOID );
		while( t_rcv( tp[r].fd, rbuf, bsize, &rflags ) > 0);
		expecting( TNODATA );
		t_rcv( tp[r].fd, rbuf, bsize, &rflags );
	finis
}

void do_open( id ) 
int id;
{
	prep("Call t_open to open the transport provider: '%s'.",
	 tp[id]. provider );
	start
		tp[id].fd = t_open( tp[id].provider, O_RDWR, &info[id]);
	finis
	chk_unbnd( id );
}
void do_alloc( id ) 
int id;
{
	prep("Call t_alloc to allocate some structures.");
	start
		ret[id] = (struct t_bind *) t_alloc( tp[id].fd, T_BIND, T_ALL );
		req[id] = (struct t_bind *) t_alloc( tp[id].fd, T_BIND, T_ALL );
		sndcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL,
		  T_ALL );
		rcvcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL,
		  T_ALL );
		rcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL,
		  T_ALL );
		scall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL, 
		  T_ALL );
	finis
}
void do_bind( id ) 
int id;
{
	prep("Copy the address to be bound into the addr buffer, and");
	prep("call t_bind.");
	start
		req[id]->addr.len = tp[id].alen;
		memcpy( req[id]->addr.buf, tp[id].addr, tp[id].alen );
		req[id]->qlen = tp[id].qlen;
		t_bind( tp[id].fd, req[id], ret[id]);
	finis
	prep("Check that the requested address was the one bound.");
	start
		expecting( VOID );
		if( tp[id].alen != ret[id]->addr.len ||
		  memcmp( ret[id]->addr.buf, tp[id].addr, tp[id].alen ) )
			warning("Address bound was not address requested.");
	finis
	chk_idle( id );
}
void do_unbind( id ) 
int id;
{
	cleanup("Call t_unbind.");
	start
		t_unbind( tp[id].fd );
	finis
}
void do_free( id ) 
int id;
{
	cleanup("Call t_free to free the allocated structures.");
	start
		t_free( ret[id], T_BIND );
		t_free( req[id], T_BIND );
		t_free( sndcall[id], T_CALL );
		t_free( rcvcall[id], T_CALL );
		t_free( scall[id], T_CALL );
		t_free( rcall[id], T_CALL );
	finis
}
void do_close( id ) 
int id;
{
	cleanup("Call t_close to close the transport provider.");
	start
		t_close( tp[id].fd );
	finis
}
void set_ndelay( id ) 
int id;
{
	int	oflag;

	prep("Call fcntl to get the file status flags.  If the");
	prep("O_NDELAY file status flag is not set, call fcntl");
	prep("again to set it.");
	start
		int oflag;
		expecting( VOID );
		oflag = fcntl( tp[id].fd, F_GETFL, NULL );
		if( !(oflag & O_NDELAY ) ) {
			expecting( SUCCESS );
			fcntl( tp[id].fd, F_SETFL, oflag | O_NDELAY );
		}
	finis
}
void clr_ndelay( id ) 
int id;
{
	int	oflag;

	prep("Call fcntl to get the file status flags.  If the");
	prep("O_NDELAY file status flag is set, call fcntl");
	prep("again to clear it.");
	start
		int oflag;

		expecting( VOID );
		oflag = fcntl( tp[id].fd, F_GETFL, NULL );
		if( (oflag & O_NDELAY ) ) {
			expecting( SUCCESS );
			fcntl( tp[id].fd, F_SETFL, oflag & ~O_NDELAY );
		}
	finis
}
void chk_unbnd( id ) 
int id;
{
	chk_state( tp[id].fd, T_UNBND );
}
void chk_idle( id ) 
int id;
{
	chk_state( tp[id].fd ,T_IDLE );
}
void chk_incon( id ) 
int id;
{
	chk_state( tp[id].fd, T_INCON );
}
void chk_outcon( id ) 
int id;
{
	chk_state( tp[id].fd, T_OUTCON );
}
void chk_dataxfer( id ) 
int id;
{
	chk_state( tp[id].fd, T_DATAXFER );
}
void chk_inrel( id ) 
int id;
{
	chk_state( tp[id].fd, T_INREL );
}
void chk_outrel( id ) 
int id;
{
	chk_state( tp[id].fd, T_OUTREL );
}
chk_state( fd, state ) 
int fd;
int state;
{
	char *st_name;
	char *sname();

	st_name = sname( state );
	prep("Call t_getstate, verify the current state is %s.",st_name);
	start
		continue_on( ERROR );
		expecting( state );
		t_getstate( fd );
	finis
}
void sigfoo( sig )
int sig;
{
	if ( sig != SIGALRM )
		severr("Caught stray signal in signal handler.");
}
fillbuf( buf, i )
char *buf;
int	i;
{
	int j;
	int	k;

	k = i;
	for( j = 0; j < i ; j++, k-- )
		*buf++ = (char) k;
}
chkbuff( buf, cnt, sum, i )
char *buf;
int	cnt, sum, i;
{
	int j,k;

	k = i - sum;
	for( j = 0; j < cnt; j++, k--)
		if( *buf++ != (char) k ) {
			error("Wrong data received in transfer of %d bytes.",i);
			return;
		}
}
bound( i )
int i;
{
	int j = 1;
/*
 *	1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, ...
 */
	while( i > j ) j *= 2;
	if( i == j || (i+1) == j ) return( i+1 );
	return( j-1 );
}
get_to_state( state ) 
int state;
{
	prep("Establish communications.");
	start
		rendevous();
		prep("Call t_connect asynchronously for the active user.");
		start
			set_ndelay( 1 );
			sndcall[1]->addr.len = tp[0].clen;
			memcpy( sndcall[1]->addr.buf, tp[0].conn, tp[0].clen );
			expecting( TNODATA );
			t_connect( tp[1].fd, sndcall[1], rcvcall[1] );
			clr_ndelay( 1 );
		finis
		prep("Call t_listen synchronously for passive user.");
		start
			clr_ndelay( 0 );
			t_listen( tp[0].fd, rcall[1] );
		finis
		prep("Check the returned addr corresponds to active user.");
		start
			expecting( VOID );
			if( tp[1].clen != rcall[1]->addr.len ||
			  memcmp( rcall[1]->addr.buf, tp[1].conn, tp[1].clen ) )
				warning("Wrong addr returned by t_listen.");
		finis
		chk_incon( 0 );
		chk_outcon( 1 );
		rendevous();
		if( state != T_OUTCON ) {
			prep("Call t_accept for passive user.");
			start
				t_accept( tp[0].fd, tp[0].fd, rcall[1] );
			finis
			prep("Call t_rcvconnect for the active user.");
			start
				t_rcvconnect( tp[1].fd, rcvcall[1] );
			finis
			prep("Check that the active user addr is returned.");
			start
				expecting( VOID );
				if( tp[0].clen != rcvcall[1]->addr.len ||
				  memcmp( rcvcall[1]->addr.buf, tp[0].conn, tp[0].clen ) )
					warning("Wrong addr returned by t_rcvconnect.");
			finis

			chk_dataxfer( 1 );

			rendevous();
		}
	finis
	switch( state ) {

	case T_OUTCON:
	case T_DATAXFER:
		break;

	case T_INREL:
		prep("Call t_sndrel from the passive user and t_rcvrel");
		prep("from the active user.");
		start
			if( info[1].servtype == T_COTS_ORD ) {
				t_sndrel( tp[0].fd );
				chk_outrel( 0 );
				rendevous();
				t_rcvrel( tp[1].fd );
				chk_inrel( 1 );
			}
			else
				message(
				  "Orderly release not supported by this provider.");
		finis
		if( info[1].servtype != T_COTS_ORD )
			return( 0 );
		break;

	case T_OUTREL:
		prep("Call t_sndrel from the active user and t_rcvrel");
		prep("from the passive user.");
		start
			if( info[0].servtype == T_COTS_ORD ) {
				t_sndrel( tp[1].fd );
				chk_outrel( 1 );
				rendevous();
				t_rcvrel( tp[0].fd );
				chk_inrel( 0 );
			}
			else
				message(
				  "Orderly release not supported by this provider.");
		finis
		if( info[0].servtype != T_COTS_ORD )
			return( 0 );
		break;
	}
	rendevous();
	return( 1 );
}
init_endpoint( id )
int id;
{
	prep("Call t_open, t_alloc, and t_bind for the");
	prep("%s transport provider.", id?"active":"passive");
	start
		exit_on( ERROR );
		do_open( id );
		chk_unbnd( id );
		do_alloc( id );
		do_bind( id );
		chk_idle( id );
	finis
}
deinit_endpoint( id )
int id;
{
	cleanup("Unbind, free, and close the active transport provider.",
	  id?"active":"passive");
	start
		do_unbind( id );
		do_free( id );
		do_close( id );
	finis
}
