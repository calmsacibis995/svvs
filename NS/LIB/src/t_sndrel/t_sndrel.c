/*
 *	t_sndrel.c
 *
 *	a test of t_sndrel(NS_LIB) and t_rcvrel(NS_LIB)
 *	
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)t_sndrel.c	1.5"

#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <signal.h>
#include <svvs.h>
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#define rendevous()		( v_sema(), p_sema() )
#define	debug(foo)	/*	printf foo ; fflush( stdout )		*/

char tmpfyle[128];
char *fnct;
int	(*fn)();
#define	SNDREL	(fn == t_sndrel)
#define	RCVREL	(fn == t_rcvrel)
int	t_sndrel();
int	t_rcvrel();
int	t_errno;
char	*terr_lookup();
char	*err_lookup();

#define	min( a, b )		(( a < b ) ? a : b )
#define	max( a, b )		(( a > b ) ? a : b )

#define	supported( z )	( info[z].servtype == T_COTS_ORD )

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
	{	NULL,	NULL,	0,	NULL,	0,	0,	-1,	},
};

#define NPROVIDERS	(sizeof( tp )/sizeof(struct endpoint))

struct	t_info		info[ NPROVIDERS ];			
struct	t_bind		*req[ NPROVIDERS ];
struct	t_bind		*ret[ NPROVIDERS ];
struct	t_call		*scall[ NPROVIDERS ];
struct	t_call		*rcall[ NPROVIDERS ];

char	*sbuf;		/* send buffer	*/
char	*rbuf;		/* rcv	buffer	*/
int		bsize;		/* buffer size	*/

int states[] = { 
	T_INREL,
	T_OUTREL 
};

#define	NSTATES	(sizeof( states )/sizeof( int ))

main( argc, argv ) 
int argc;
char *argv[];
{
	int	cpid;
	int	state_id;
	int	expd;
	int	async;
	int	x;
	int	r;

	for( fnct = argv[0]; *fnct; fnct++ );
	while( fnct > argv[0] && *(fnct-1) != '/' ) fnct--;

	setup(fnct);
	funct(fnct);

	prep("Initialize variables and structures.");
	start
		expecting( VOID );
		if( !strcmp( fnct, "t_sndrel" ) ) {
			fn = t_sndrel;
		}
		else if( !strcmp( fnct, "t_rcvrel" ) ) {
			fn = t_rcvrel;
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

		tp[2].provider = TPDGFILE0;
		tp[2].addr = TPDGADDR1;
		tp[2].alen = TPDGALEN1;
		tp[2].conn = TPDGCONN1;
		tp[2].clen = TPDGCLEN1;
	finis

	prep("Initialize semaphore mechanism.");
	start
		seminit( 1 );
	finis

	prep("Call t_open, t_alloc, and t_bind for the");
	prep("active transport provider.");
	start
		exit_on( ERROR );
		do_open( 1 );
		chk_unbnd( 1 );
		do_alloc( 1 );
		do_bind( 1 );
		chk_idle( 1 );
	finis

	prep("Call t_open, t_alloc, and t_bind for the");
	prep("passive transport provider.");
	start
		exit_on( ERROR );
		do_open( 0 );
		chk_unbnd( 0 );
		do_alloc( 0 );
		do_bind( 0 );
		chk_idle( 0 );
	finis

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

	for( state_id = 0; state_id <= NSTATES; state_id++ ) {
	for( expd = 0; expd <= T_EXPEDITED; expd += T_EXPEDITED ){ 
	for( async = 0; async <= 1; async++ ) {
		abstract("Establish communications between two endpoints.");
		switch( states[state_id] ) {
		case	T_INREL:
			abstract("Call t_sndrel and t_rcvrel from the");
			abstract("passive user and active user respectively.");
			abstract("Send from the passive user to the active user.");
			x = 0;
			r = 1;
			break;
		case	T_OUTREL:
			abstract("Call t_sndrel and t_rcvrel from the");
			abstract("active user and passive user respectively.");
			abstract("Send from the active user to the passive user.");
			x = 1;
			r = 0;
			break;
		}
		if( expd )
			abstract("Expedited data is transferred.");
		else
			abstract("Normal (not expedited) data is transferred.");

		if( async )
			abstract("Endpoints are used asynchronously.");
		else
			abstract("Endpoints are used synchronously.");
		start
			
			get_to_dataxfer();

			if( async ) {
				set_ndelay( 0 );
				set_ndelay( 1 );
			}
			else {
				clr_ndelay( 0 );
				clr_ndelay( 1 );
			}

			rendevous();

			do_rel( x, r );
	
			if( supported( x ) )
				chk_outrel( x );
			if( supported( r ) )
				chk_inrel( r );
	
			rendevous();
	
			do_sndrcv( r, x, expd );

			rendevous();
		
			do_rel( r, x );

			if( !supported( x ) ) {
				cleanup("Call t_snddis to return the sending user to");
				cleanup("T_IDLE.");
				start
					t_snddis( tp[x].fd, NULL );
				finis
			}
			chk_idle( x );
	
			rendevous();
	
			if( !supported( r ) ) {
				cleanup("Call t_rcvdis to return the receiving user to");
				cleanup("T_IDLE.");
				start
					t_rcvdis( tp[x].fd, NULL );
				finis
			}
			chk_idle( r );

			rendevous();
		finis
	}}}

	if (RCVREL) {
		abstract("The active user calls t_snddis to send a disconnect.");
		abstract("Call t_rcvrel from the passive user and verify that");
		abstract("t_rcvrel fails and sets t_errno to TLOOK.");
		start
			get_to_dataxfer();
	
			rendevous();
	
			prep("Call t_snddis.");
			start		
				t_snddis( tp[1].fd, NULL );
			finis
	
			rendevous();
	
			testing("Call t_rcvrel.");
			start
				expecting( TLOOK );
				t_rcvrel( tp[0].fd );
			finis
	
			cleanup("Call t_rcvdis to clear the disconnect indication.");
			start
				t_rcvdis( tp[0].fd, NULL );
			finis
		finis
	
		abstract("The passive user calls t_snddis to send a disconnect.");
		abstract("Call t_rcvrel from the active user and verify that");
		abstract("t_rcvrel fails and sets t_errno to TLOOK.");
		start
			get_to_dataxfer();
	
			rendevous();
	
			prep("Call t_snddis.");
			start		
				t_snddis( tp[0].fd, NULL );
			finis
	
			rendevous();
	
			testing("Call t_rcvrel.");
			start
				expecting( TLOOK );
				t_rcvrel( tp[1].fd );
			finis
	
			cleanup("Call t_rcvdis to clear the disconnect indication.");
			start
				t_rcvdis( tp[1].fd, NULL );
			finis
		finis
	
		abstract("Call t_rcvrel from both users when no orderly");
		abstract("release indication exists and verify that t_rcvrel ");
		abstract("fails and sets t_errno to TNOREL.");
		start
	
			get_to_dataxfer();
	
			rendevous();
	
			prep("Call t_rcvrel from the active user.");
			start		
				expecting( TNOREL );
				t_rcvrel( tp[1].fd );
			finis
	
			prep("Call t_rcvrel from the passive user.");
			start		
				expecting( TNOREL );
				t_rcvrel( tp[1].fd );
			finis
		finis
	
	}

	rendevous();

	if	(SNDREL) {

		get_to_dataxfer();
	
		rendevous();
	
		abstract("Call t_snd many times asynchronously with normal data to");
		abstract("attempt to generate a flow restriction.  Call t_snd");
		abstract("again with expedited data until a flow restriction"); 
		abstract("occurs.  Call t_sndrel asynchronously expecting");
		abstract("a TFLOW error.  Call t_sndrel again synchronously and");
		abstract("verify t_sndrel blocks when flow control prevents the");
		abstract("orderly release indication from being accepted. Set an");
		abstract("alarm.  Verify that when the SIGALRM occurs, t_sndrel");
		abstract("fails and sets t_errno to TSYSERR and errno to EINTR.");
		abstract("Call t_rcv to clear the receiver endpoint.  Data is");
		abstract("sent from the active user to the passive user.");
		start
			do_tflow( 1, 0 );
		finis
	
		rendevous();
	
		abstract("Call t_snd many times asynchronously with normal data to");
		abstract("attempt to generate a flow restriction.  Call t_snd");
		abstract("again with expedited data until a flow restriction"); 
		abstract("occurs.  Call t_sndrel asynchronously expecting");
		abstract("a TFLOW error.  Call t_sndrel again synchronously and");
		abstract("verify t_sndrel blocks when flow control prevents the");
		abstract("orderly release indication from being accepted. Set an");
		abstract("alarm.  Verify that when the SIGALRM occurs, t_sndrel");
		abstract("fails and sets t_errno to TSYSERR and errno to EINTR.");
		abstract("Call t_rcv to clear the receiver endpoint.  Data is");
		abstract("sent from the active user to the passive user.");
		start
			do_tflow( 0, 1 );
		finis
	
	}
	
	rendevous();

	cleanup("Call t_snddis from the active user and t_rcvdis from");
	cleanup("the passive user to return to the T_IDLE state.");
	start
		t_snddis( tp[1].fd, NULL );
		rendevous();
		t_rcvdis( tp[0].fd, NULL );
	finis

	cleanup("Unbind, free, and close the active transport provider.");
	start
		do_unbind( 1 );
		do_free( 1 );
		do_close( 1 );
	finis

	cleanup("Unbind, free, and close the passive transport provider.");
	start
		do_unbind( 0 );
		do_free( 0 );
		do_close( 0 );
	finis

	abstract("Open a temp file.  Call %s with the file", fnct);
	abstract("file descriptor of the temp file.  Verify that");
	abstract("%s fails and sets t_errno to TBADF.",fnct);
	start
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
			(*fn)( fd );
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
			tp[2].fd = t_open( TPDGFILE0, O_RDWR, &info[ 2 ]);
		finis
		chk_unbnd( 2 );
		do_alloc( 2 );
		do_bind( 2 );
		testing("Call %s.",fnct);
		start
			expecting( TNOTSUPPORT );
			(*fn)( tp[2].fd );
		finis
		do_free( 2 );
		do_close( 2 );
	finis
	done();
}
void do_sndrcv( x, r, flags ) 
int	x;
int	r;
int	flags;
{
	static char casenum = '1';

	testing("Call t_snd from the %s user.",x?"active":"passive");
	start
		if( supported( x ) ) {
			*sbuf = casenum;
			t_snd( tp[x].fd, sbuf, 1, flags );
		}
	finis

	rendevous();

	testing("Call t_rcv from the %s user.",r?"active":"passive");
	start
		int rflags;
		if( supported( r ) ) {
			t_rcv( tp[r].fd, rbuf, bsize, &rflags );
			if( *rbuf != casenum )
				error("Wrong data received by t_rcv.");
		}
	finis
	casenum++;
}
do_tflow( x, r )
int x, r;
{
	int	size;
	int	esize;

	size = info[x].tsdu;
	esize = info[x].etsdu;
	if( size <= 0 && esize <= 0 ) {
		message("tsdu and etsdu not supported for this provider.");
		return;
	}

	prep("Set signal catching for SIGALRM.");
	start
		void sigfoo();
		signal( SIGALRM, sigfoo );
	finis

	set_ndelay( x );

	prep("Call t_snd with normal data and a data size of %d",size);
	prep("10000 times or until a TFLOW error occurs.");
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
	prep("Call t_snd with expedited data and a data size of %d",esize);
	prep("10000 times or until a TFLOW error occurs.");
	start
		int	i;
		expecting( VOID );
		for( i = 0; i < 10000; i++ ) {
			if( t_snd( tp[x].fd, sbuf, esize, T_EXPEDITED ) < 0 ){
				expecting( TFLOW );
				t_snd( tp[x].fd, sbuf, esize, T_EXPEDITED );
				break;
			}
		}
		if( i == 10000 ) {
			warning("Could not generate TFLOW error in 10K attempts.");
			goto recover;
		}
	finis

	testing("Call t_sndrel asynchronously expecting a TFLOW error.");
	start
		expecting( TFLOW );
		t_sndrel( tp[x].fd );
	finis

	clr_ndelay( x );

	prep("Set a 10 second alarm clock.");
	start
		expecting( 0 );
		alarm( 10 );
	finis

	testing("Call t_sndrel, expecting to block due to a flow");
	testing("restriction.  Expect the pending SIGALRM to make");
	testing("t_sndrel fail.");
	start
#ifdef UNDEF
		expecting( TSYSERR );
		expecting( EINTR );
		t_sndrel( tp[x].fd );
#else
		expecting( FAILURE );
		t_sndrel( tp[x].fd );
		if( t_errno == TFLOW )
			warning( "%s\nt_sndrel set t_errno to TFLOW instead of TSYSERR",ADVISORY );
		else if ( t_errno != TSYSERR )
			error("%s%s%s%s%s%s",
			  "t_sndrel - set t_errno incorrectly\n",
			  "expected t_errno: ",
			  terr_lookup( TSYSERR ),
			  "\nactual t_errno: ",
			  terr_lookup(t_errno),
			  "\n" );
		else if ( errno != EINTR )
			error("%s%s%s%s%s%s",
			  "t_sndrel - set errno incorrectly\n",
			  "expected errno: ",
			  err_lookup( EINTR ),
			  "\nactual errno: ",
			  err_lookup(errno),
			  "\n" );
#endif
	finis

	cleanup("Clear the alarm clock.");
	start
		expecting( VOID );
		alarm(0);
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
		rcall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL, T_ALL );
		scall[id] = (struct t_call *) t_alloc( tp[id].fd, T_CALL, T_ALL );
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
get_to_dataxfer() {

	prep("Establish communications.");
	start
		rendevous();

		prep("Call t_connect asynchronously for the active user.");
		start
			set_ndelay( 1 );
			scall[1]->addr.len = tp[0].clen;
			memcpy( scall[1]->addr.buf, tp[0].conn, tp[0].clen );
			expecting( TNODATA );
			t_connect( tp[1].fd, scall[1], rcall[1] );
			clr_ndelay(1);
		finis

		prep("Call t_listen for passive user.");
		prep("Check the returned addr corresponds to active user.");
		start
			clr_ndelay( 0 );
			t_listen( tp[0].fd, rcall[0] );
			expecting( VOID );
			if( tp[1].clen != rcall[0]->addr.len ||
			  memcmp( rcall[0]->addr.buf, tp[1].conn, tp[1].clen ) )
				warning("Wrong addr returned by t_listen.");
		finis

		chk_incon( 0 );
		chk_outcon( 1 );

		rendevous();	/* need for outcon chk */

		prep("Call t_accept for passive user.");
		start
			t_accept( tp[0].fd, tp[0].fd, rcall[0] );
		finis

		prep("Call t_rcvconnect for the active user.");
		prep("Check that the active user addr is returned.");
		start
			t_rcvconnect( tp[1].fd, rcall[1] );
			expecting( VOID );
			if( tp[0].clen != rcall[1]->addr.len ||
			  memcmp( rcall[1]->addr.buf, tp[0].conn, tp[0].clen ) )
				warning("Wrong addr returned by t_rcvconnect.");
		finis
		chk_dataxfer( 0 );
		chk_dataxfer( 1 );
		rendevous();
	finis
}
do_rel( a, b ) 
int a;
int b;
{
	if( a ) {
		testing("Call t_sndrel from the active user and t_rcvrel");
		testing("from the passive user.");
	}
	else {
		testing("Call t_sndrel from the passive user and");
		testing("t_rcvrel from the active user.");
	}
	start
		if( supported( a ) )
			t_sndrel( tp[a].fd );
		else
			message("Orderly release not supported by '%s'.",
			  tp[a].provider);

		rendevous();

		if( supported( b ) )
			t_rcvrel( tp[b].fd );
		else
			message("Orderly release not supported by '%s'.",
			  tp[b].provider);

	finis
}
