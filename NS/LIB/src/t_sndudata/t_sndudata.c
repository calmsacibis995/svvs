/*
 *	t_sndudata.c
 *
 *	a test of t_sndudata(NS_LIB), t_rcvudata(NS_LIB) and
 *		t_rcvuderr(NS_LIB)
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)t_sndudata.c	1.8"

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
#define	debug(foo)		/*	printf foo ; fflush( stdout )	*/

#define	SNDUDATA	(fn == t_sndudata)
#define	RCVUDATA	(fn == t_rcvudata)
#define	RCVUDERR	(fn == t_rcvuderr)

char 	tmpfyle[128];
int	strt;
int	(*fn)();
char	*fnct;

#define	min( a, b )		(( a < b ) ? a : b )
#define	max( a, b )		(( a > b ) ? a : b )

int	t_sndudata();
int	t_rcvudata();
int	t_rcvuderr();

void	chk_unbnd();
void	chk_idle();

void	do_open();
void	do_close();
void	do_alloc();
void	do_free();
void	do_bind();
void	do_unbind();
void	do_sndrcv();
void	do_optmgmt();
void	set_ndelay();
void	clr_ndelay();
void	chk_udata();
void	set_flags();
void	clr_flags();
void	do_test();
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
	{	NULL,	NULL,	0,	NULL,	0,	0,	-1	},
	{	NULL,	NULL,	0,	NULL,	0,	1,	-1	},
};

#define NPROVIDERS	(sizeof( tp )/sizeof(struct endpoint))

struct	t_info		info[ NPROVIDERS ];			
struct	t_bind		*req[ NPROVIDERS ];
struct	t_bind		*ret[ NPROVIDERS ];
struct	t_unitdata	*udt[ NPROVIDERS ];
struct	t_optmgmt	*ops[ NPROVIDERS ];
struct	t_uderr		*udr[ NPROVIDERS ];

int got_sig;
extern int t_errno;

main(argc,argv) 
int	argc;
char	*argv[];
{
	for( fnct = argv[0]; *fnct; fnct++ );
	while( fnct > argv[0] && *(fnct-1) != '/' ) fnct--;

	setup(fnct);
	funct(fnct);

	prep("Initialize variables and structures.");
	start
		expecting( VOID );
		if( !strcmp( fnct, "t_sndudata" ) ) {
			strt = 0;
			fn = t_sndudata;
		}
		else if( !strcmp( fnct, "t_rcvudata" ) ) {
			strt = 1;
			fn = t_rcvudata;
		}
#ifdef RCVUDERR_FIXED
		else if( !strcmp( fnct, "t_rcvuderr" ) ) {
			fn = t_rcvuderr;
		}
#endif
		else
			severr("%s%s%s%s",
			  "Unknown name '",fnct,"'\n",
			  "expected:	t_snd or t_rcv.");

		strcpy(tmpfyle,fnct);
		strcat(tmpfyle,".tmp");

		tp[0].provider = TPDGFILE0;
		tp[0].addr = TPDGADDR0;
		tp[0].alen = TPDGALEN0;
		tp[0].conn = TPDGCONN0;
		tp[0].clen = TPDGCLEN0;
	
		tp[1].provider = TPDGFILE1;
		tp[1].addr = TPDGADDR1;
		tp[1].alen = TPDGALEN1;
		tp[1].conn = TPDGCONN1;
		tp[1].clen = TPDGCLEN1;
	finis

	prep("Initialize semaphore mechanism.");
	start
		seminit( 1 );
	finis

	prep("Call t_open, t_alloc, t_bind, and t_optmgmt for the");
	prep("transmitter transport provider.");
	start
		exit_on( ERROR );
		do_open( 1 );
		do_alloc( 1 );
		do_bind( 1 );
		do_optmgmt( 1 );
		chk_idle( 1 );
	finis

	prep("Initialize the addr buffer for the transmitter.");
	start
		udt[1]->addr.len = tp[0].clen;
		memcpy( udt[1]->addr.buf, tp[0].conn, tp[0].clen );
	finis

	prep("Call t_open, t_alloc, t_bind, and t_optmgmt for the");
	prep("receiver transport provider.");
	start
		exit_on( ERROR );
		do_open( 0 );
		do_alloc( 0 );
		do_bind( 0 );
		do_optmgmt( 0 );
		chk_idle( 0 );
	finis

	rendevous();

	if (SNDUDATA || RCVUDATA) {
		abstract("Exercise the communications between two endpoints.");
		abstract("Endpoints are used synchronously with no protocol");
		abstract("options requested by t_sndudata.  Send and receive a");
		abstract("series of data items of lengths %d through tsdu size.",
		  strt );

		start
			clr_ndelay( 0 );
			clr_ndelay( 1 );
			set_opt( 1, NULL, 0 );
			do_sndrcv( 0 );
		finis
	
		rendevous();
	
	 	abstract("Exercise the communications between two endpoints.");
		abstract("Endpoints are used synchronously with the protocol");
		abstract("options passed to t_sndudata set to values retrieved");
		abstract("by t_optmgmt.  Send and receive a series of data items");
		abstract("of length %d through tsdu size.",strt);
		start
			clr_ndelay( 0 );
			clr_ndelay( 1 );
			set_opt( 1, ops[1]->opt.buf, ops[1]->opt.len );
			do_sndrcv( 0 );
		finis
	
		rendevous();
	
		abstract("Exercise the communications between two endpoints.");
		abstract("Endpoints are used asynchronously with no protocol");
		abstract("options requested by t_sndudata.  Send and receive a");
		abstract("series of data items of length %d through tsdu size.",
		  strt);
		start
			set_ndelay( 0 );
			set_ndelay( 1 );
			set_opt( 1, NULL, 0 );
			do_sndrcv( 1 );
		finis
	
		rendevous();
	
		abstract("Exercise the communications between two endpoints.");
		abstract("Endpoints are used asynchronously with the options");
		abstract("passed to t_sndudata set to values retrieved by");
		abstract("t_optmgmt. Send and receive a series of messages of");
		abstract("length %d through tsdu size.",strt);
		start
			set_ndelay( 0 );
			set_ndelay( 1 );
			set_opt( 1, ops[1]->opt.buf, ops[1]->opt.len );
			do_sndrcv( 1 );
		finis
	}
	rendevous();

	if (RCVUDATA) {
		abstract("Exercise the communications between two endpoints.");
		abstract("Endpoints are used asynchronously with no protocol");
		abstract("options requested by t_sndudata.  The receiver data");
		abstract("buffer is reduced to one fourth of its default size.");
		abstract("Check that t_rcvudata with a small buffer generates");
		abstract("a T_MORE indication.  Verify that the length for addr");
		abstract("and opt are zero on subsequent calls to t_rcvudata.");
		start
			int	oldsize;

			set_ndelay( 0 );
			set_ndelay( 1 );
			set_opt( 1, NULL, 0 );
			prep("Set the buffer size for incoming data to %d.",
				udt[0]->udata.maxlen/4);
			start
				oldsize = udt[0]->udata.maxlen;
				udt[0]->udata.maxlen = udt[0]->udata.maxlen/4;
			finis
			do_sndrcv( 1 );
			cleanup("Reset the buffer size to %d.",oldsize);
			start
				udt[0]->udata.maxlen = oldsize;
			finis
		finis

		rendevous();

		abstract("Check that t_rcvudata in asynchronous mode fails");
		abstract("with t_errno set to TNODATA when no data units are");
		abstract("available.");
		start
			set_ndelay( 0 );
			testing("Call t_rcvudata.");
			start
				int flags;
				expecting( TNODATA );
				t_rcvudata( tp[0].fd, udt[0], &flags );
			finis
			clr_ndelay( 0 );
		finis

		rendevous();

		abstract("Check that t_rcvudata in synchronous mode blocks");
		abstract("when no data units are available.  Set an alarm");
		abstract("and verify that when the SIGALRM occurs t_rcvudata");
		abstract("fails and sets t_errno to TSYSERR and errno to EINTR.");
		start
			clr_ndelay( 0 );
			prep("Set signal catching for SIGALRM.");
			start
				void sigfoo();
				got_sig = 0;
				signal( SIGALRM, sigfoo );
			finis
			prep("Set a 10 second alarm.");
			start
				expecting( 0 );
				alarm( 10 );
			finis
			testing("Call t_rcvudata.");
			start
				int flags;
				expecting( TSYSERR );
				expecting( EINTR );
				t_rcvudata( tp[0].fd, udt[0], &flags );
			finis
		finis
	
		rendevous();

		abstract("Check that t_rcvudata with a zero length addr buffer");
		abstract("fails and sets t_errno to TBUFOVFLW.");
		start
			prep("Call t_sndudata.");
			start
				udt[1]->udata.len=1;
				t_sndudata( tp[1].fd, udt[1] );
			finis
			rendevous();
			testing("Call t_rcvudata with an addr buffer of length 0.");
			start
				int flags;
				int oldlen = udt[0]->addr.maxlen;
				udt[0]->addr.maxlen = 0;
				expecting( TBUFOVFLW );
				t_rcvudata( tp[0].fd, udt[0], &flags );
				udt[0]->addr.maxlen = oldlen;
			finis
		finis
	
		rendevous();
	
		abstract("Check that t_rcvudata with a zero length opt buffer");
		abstract("fails and sets t_errno to TBUFOVFLW.");
		start
			set_opt( 1, ops[1]->opt.buf, ops[1]->opt.len );
			prep("Call t_sndudata.");
			start
				udt[1]->udata.len=1;
				t_sndudata( tp[1].fd, udt[1] );
			finis
			rendevous();
			testing("Call t_rcvudata with an opt buffer of length 0.");
			start
				int flags;
				int oldlen = udt[0]->opt.maxlen;
				udt[0]->opt.maxlen = 0;
				expecting( TBUFOVFLW );
				t_rcvudata( tp[0].fd, udt[0], &flags );
				udt[0]->opt.maxlen = oldlen;
			finis
		finis

		rendevous();
#ifdef RCVUDERR_FIXED
		abstract("Unbind the transmitter endpoint.  The receiver then");
		abstract("attempts to send to the transmitter, which should");
		abstract("generate an error.  The transmitter then binds and");
		abstract("sends to the receiver.  Verify that t_rcvudata fails");
		abstract("and sets t_errno to TLOOK.");
		start
			set_ndelay( 0 );
			prep("Unbind the transmitter.");
			start		
				t_unbind( tp[1].fd );
			finis
			rendevous();
			prep("Initialize the addr buffer.");
			start
				udt[0]->addr.len = tp[1].clen;
				memcpy( udt[0]->addr.buf, tp[1].conn, tp[1].clen );
			finis
			set_opt( 0, NULL, 0 );
			prep("Call t_sndudata from the receiver.");
			start
				udt[0]->udata.len=1;
				t_sndudata( tp[0].fd, udt[0] );
			finis
			rendevous();
			do_bind( 1 );
			rendevous();
			prep("Call t_sndudata from the transmitter.");
			start
				udt[1]->udata.len=1;
				t_sndudata( tp[1].fd, udt[1] );
			finis
			rendevous();
			testing("Call t_rcvudata.");
			start
				int flags;
				expecting( TLOOK );
				t_rcvudata( tp[0].fd, udt[0], &flags );
			finis
			cleanup("Call t_rcvuderr to clear the error indication.");
			start
				t_rcvuderr( tp[0].fd, NULL );
			finis
			cleanup("Call t_rcvudata to receive the sent data.  This");
			cleanup("invocation of t_rcvudata is expected to succeed.");
			start
				int flags;
	
				t_rcvudata( tp[0].fd, udt[0], &flags );
			finis
		finis
#endif
	}

	rendevous();

	if (SNDUDATA) {
		abstract("Call t_sndudata many times asynchronously to");
		abstract("attempt to generate a TFLOW error.");
		abstract("After generating a TFLOW error, call t_sndudata again");
		abstract("synchronously to verify t_sndudata in synchronous mode");
		abstract("blocks when flow control prevents the data from being.");
		abstract("accepted. Set an alarm and verify that when SIGALRM");
		abstract("occurs, t_sndudata fails with t_errno set to TSYSERR");
		abstract("and errno to EINTR.  Then call t_rcvudata to clear the");
		abstract("receiver endpoint.");
		start
			prep("Set signal catching for SIGALRM.");
			start
				void sigfoo();
				got_sig = 0;
				signal( SIGALRM, sigfoo );
			finis
			set_ndelay( 1 );
			testing("Call t_sndudata with a data size of %d",
			  udt[1]->udata.maxlen);
			testing("10000 times or until a TFLOW error occurs.");
			start
				int	i;
				expecting( VOID );
				udt[1]->udata.len = udt[1]->udata.maxlen;
				for( i = 0; i < 10000; i++ ) {
					if( t_sndudata( tp[1].fd, udt[1] ) ) {
						expecting( TFLOW );
						t_sndudata( tp[1].fd, udt[1] );
						break;
					}
				}
				if( i == 10000 ) {
					warning("Could not generate TFLOW error in 10K attempts.");
					goto recover;
				}
			finis
			clr_ndelay( 1 );
			prep("Set a 10 second alarm clock.");
			start
				expecting( 0 );
				alarm( 10 );
			finis
			testing("Call t_sndudata, expecting to block due to a flow");
			testing("restriction.  Expect the pending SIGALRM to make");
			testing("t_sndudata fail.");
			start
				expecting( TSYSERR );
				expecting( EINTR );
				t_sndudata( tp[1].fd, udt[1]);
			finis
recover:
			rendevous();		
			cleanup("Call t_rcvudata until TNODATA occurs.");
			start
				int flags;
	
				expecting( VOID );
				while( !t_rcvudata( tp[0].fd, udt[0], &flags ));
				expecting( TNODATA );
				t_rcvudata( tp[0].fd, udt[0], &flags);
			finis
		finis

		abstract("Call t_sndudata with a data size larger than tsdu.");
		abstract("Verify that t_sndudata fails and sets t_errno to");
		abstract("TSYSERR and errno to EPROTO.");
		start
			int	oldsize;
			prep("Set the data size to tsdu (%d) + 1.",info[1].tsdu);
			start
				oldsize = udt[1]->udata.len;
				udt[1]->udata.len=info[1].tsdu+1;
			finis
			testing("Call t_sndudata.");
			start
				expecting( VOID );
				if( t_sndudata( tp[1].fd, udt[1] ) == -1 &&
				  t_errno == TSYSERR && errno == ERANGE )
					warning( "%s\nt_sndudata set errno to ERANGE instead of EPROTO",ADVISORY );
				else {
					expecting( TSYSERR );
					expecting( EPROTO );
					t_sndudata( tp[1].fd, udt[1] );
				}
			finis
			cleanup("Reset the data size to %d.",oldsize);
			start
				udt[1]->udata.len=oldsize;
			finis
			cleanup("Call t_close to close both endpoints.  No check of");
			cleanup("the return value is done, as there is an EPROTO");
			cleanup("condition.");
			start
				expecting( VOID );
				t_close( tp[0].fd );	
				t_close( tp[1].fd );
			finis
			cleanup("Call t_open, t_alloc, t_bind, and t_optmgmt for the");
			cleanup("transmitter transport provider.");
			start
				exit_on( ERROR );
				do_open( 1 );
				do_alloc( 1 );
				do_bind( 1 );
				do_optmgmt( 1 );
				chk_idle( 1 );
			finis
			cleanup("Initialize the addr buffer for the transmitter.");
			start
				udt[1]->addr.len = tp[0].clen;
				memcpy( udt[1]->addr.buf, tp[0].conn, tp[0].clen );
			finis
			cleanup("Call t_open, t_alloc, t_bind, and t_optmgmt for the");
			cleanup("receiver transport provider.");
			start
				exit_on( ERROR );
				do_open( 0 );
				do_alloc( 0 );
				do_bind( 0 );
				do_optmgmt( 0 );
				chk_idle( 0 );
			finis
		finis

		rendevous();

		abstract("Unbind the transmitter provider.  Check that the");
		abstract("state is T_UNBND. Call t_sndudata from an invalid");
		abstract("state until failure.  Verify the failure sets");
		abstract("t_errno to TSYSERR and errno to EPROTO.");
		start
			chk_idle( 1 );
			prep("Call t_unbind.");
			start
				t_unbind( tp[1].fd );
			finis
			chk_unbnd( 1 );
			testing("Call t_sndudata 10000 times or until failure.");
			testing("Verify the failure sets t_errno to TSYSERR and");
			testing("errno to EPROTO.  An alarm is set before each");
			testing("call to t_sndudata, in case a tflow condition");
			testing("should occur.");
			start
				break_on( ERROR );
				prep("Set signal catching for SIGALRM.");
				start
					void sigfoo();
					got_sig = 0;
					signal( SIGALRM, sigfoo );
				finis
				testing("Call t_sndudata many times.");
				start
					int i;
					expecting( VOID );

					for( i = 0; i < 10000 ; i++ ) {		
						alarm( 10 );	/* in case of tflow */
						if( t_sndudata( tp[1].fd, udt[1] ) )
							break;
					}
					alarm( 0 );
					if( got_sig )
						error("t_sndudata blocked on tflow before error.");
					break_on( WARNING );
					if( i == 10000 )
						warning("%s%s",
						  "Could not generate EPROTO error ",
						  "in 10K attempts.");
					expecting( TSYSERR );
					expecting( EPROTO );
					t_sndudata( tp[1].fd, udt[1] );
				finis
			finis
			cleanup("Call t_close to close both endpoints.  No check");
			cleanup("of the return value is done, as there is an");
			cleanup("EPROTO condition.");
			start
				expecting( VOID );
				t_close( tp[0].fd );	
				t_close( tp[1].fd );
			finis
			cleanup("Call t_open, t_alloc, t_bind, and t_optmgmt for");
			cleanup("the transmitter transport provider.");
			start
				exit_on( ERROR );
				do_open( 1 );
				do_alloc( 1 );
				do_bind( 1 );
				do_optmgmt( 1 );
				chk_idle( 1 );
			finis
			cleanup("Initialize the addr buffer for the transmitter.");
			start
				udt[1]->addr.len = tp[0].clen;
				memcpy( udt[1]->addr.buf, tp[0].conn, tp[0].clen );
			finis
			cleanup("Call t_open, t_alloc, t_bind, and t_optmgmt for");
			cleanup("the receiver transport provider.");
			start
				exit_on( ERROR );
				do_open( 0 );
				do_alloc( 0 );
				do_bind( 0 );
				do_optmgmt( 0 );
				chk_idle( 0 );
			finis
		finis
	}

	if (RCVUDERR) {
		abstract("Generate a unit data error.  Call t_look to verify");
		abstract("that an error indication exists.  Call t_rcvuderr with");
		abstract("a valid struct uderr to clear the error indication.");
		abstract("Check that the received addr and opt are correct.");
		start
			break_on( ERROR );
			do_error();
			continue_on( ERROR );
			testing("Call t_rcvuderr to clear the error indication.");
			start
				t_rcvuderr( tp[1].fd, udr[1] );
			finis
			testing("Verify that the returned addr buffer is correct.");
			start
				break_on( ERROR );
				if( udr[1]->addr.len != TPBADLEN ) 
					error("%s%s%d%s%d",
					  "Incorrect length of addr returned\n",
					  "Expected:	", TPBADLEN,
					  "\nReturned:	", udr[1]->addr.len);
				expecting( VOID );
				if( memcmp( udr[1]->addr.buf, TPBADADDR, 
				  TPBADLEN ))
					error("Incorrect address returned.");
			finis

			testing("Verify that the returned opt buffer is correct.");
			start
				break_on( ERROR );

				if( udr[1]->opt.len != ops[1]->opt.len )
					error("%s%s%d%s%d",
					  "Incorrect length of opt returned\n",
					  "Expected:	", ops[1]->opt.len,
					  "\nReturned:	", udr[1]->opt.len);

				expecting( VOID );
				if( memcmp( udr[1]->opt.buf, ops[1]->opt.buf, 
				  ops[1]->opt.len ))
					error("Incorrect options returned.");

			finis
		finis

		abstract("Generate a unit data error.  Call t_look to verify");
		abstract("that an error indication exists.  Call t_rcvuderr with");
		abstract("a NULL uderr to clear the error indication.");
		start
			break_on( ERROR );
			do_error();
			continue_on( ERROR );
			testing("Call t_rcvuderr to clear the error indication.");
			start
				t_rcvuderr( tp[1].fd, NULL );
			finis
		finis

		abstract("Generate a unit data error.  Call t_look to verify");
		abstract("that an error indication exists.  Call t_rcvuderr with");
		abstract("a zero length addr buffer.  Verify that t_rcvudata");
		abstract("fails with t_errno set to TBUFOVFLW.");
		start
			break_on( ERROR );
			do_error();
			continue_on( ERROR );
			testing("Call t_rcvuderr with a zero length addr buffer.");
			start
				int	oldsize;
				oldsize = udr[1]->addr.maxlen;
				udr[1]->addr.maxlen =  0;
				expecting( TBUFOVFLW );
				t_rcvuderr( tp[1].fd, udr[1] );
				udr[1]->addr.maxlen =  oldsize;
			finis
			cleanup("Call t_rcvuderr to clear the error indication.");
			start
				t_rcvuderr( tp[1].fd, udr[1] );
			finis
		finis

		abstract("Generate a unit data error.  Call t_look to verify");
		abstract("that an error indication exists.  Call t_rcvuderr with");
		abstract("a zero length opt buffer.  Verify that t_rcvudata");
		abstract("fails with t_errno set to TBUFOVFLW.");
		start
			break_on( ERROR );
			do_error();
			continue_on( ERROR );
			testing("Call t_rcvuderr with a zero length opt buffer.");
			start
				int	oldsize;
				oldsize = udr[1]->opt.maxlen;
				udr[1]->opt.maxlen =  0;
				expecting( TBUFOVFLW );
				t_rcvuderr( tp[1].fd, udr[1] );
				udr[1]->opt.maxlen =  oldsize;
			finis
			testing("Call t_rcvuderr to clear the error indication.");
			start
				t_rcvuderr( tp[1].fd, udr[1] );
			finis
		finis

		abstract("Call t_rcvuderr when no unit data error indication");
		abstract("currently exists on the specifed transport endpoint.");
		abstract("Call t_look to check that no error indication exists.");
		abstract("Verify that t_rcvuderr fails with t_errno set to.");
		abstract("TNOUDERR.");
		start
			break_on( ERROR );
			prep("Call t_look to verify no unit data indication exists.");
			start
				expecting( 0 );
				t_look( tp[1].fd );
			finis
			prep("Call t_rcvuderr.");
			start
				expecting( TNOUDERR );
				t_rcvuderr( tp[1].fd, NULL );
			finis
		finis
	}
	cleanup("Unbind, free, and close the transmit transport provider.");
	start
		do_unbind( 1 );
		do_free( 1 );
		do_close( 1 );
	finis

	cleanup("Unbind, free, and close the receiver transport provider.");
	start
		do_unbind( 0 );
		do_free( 0 );
		do_close( 0 );
	finis

	abstract("Open a temp file.  Call %s with the file", fnct);
	abstract("file descriptor of the temp file.  Verify that");
	abstract("%s fails and sets t_errno to TBADF.",fnct);
	start
		int flags;
		struct t_unitdata udt;
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
			if (RCVUDATA) 
				t_rcvudata( fd, &udt, &flags );
			else if (SNDUDATA)
				t_sndudata( fd, &udt );
			else if (RCVUDERR)
				t_rcvuderr( fd, NULL );
		finis
		cleanup("Close the temp file.");
		start
			close(fd);
		finis
	finis

	abstract("Call %s with a connection mode provider.",fnct);
	start
		prep("Call t_open to open the transport provider: '%s'.",
		 TPVCFILE0 );
		start
			tp[0].fd = t_open( TPVCFILE0, O_RDWR, &info[ 0 ]);
		finis
		chk_unbnd( 0 );
		do_alloc( 0 );
		do_bind( 0 );
		testing("Call %s.",fnct);
		start
			int flags;

			expecting( TNOTSUPPORT );
			if (RCVUDATA) 
				t_rcvudata( tp[0].fd, udt[0], &flags );
			else if (SNDUDATA) 
				t_sndudata( tp[0].fd, udt[0] );
			else if (RCVUDERR) 
				t_rcvuderr( tp[0].fd, NULL );
		finis
		do_free( 0 );
		do_close( 0 );
	finis
	done();
}
void do_sndrcv( async ) 
int async;
{
	int i,j,k;
	int ret_val;
	int	oldsize;
	int	flags;
	int ncases;
	int casenum;

	rendevous();

	if (SNDUDATA) {
		abstract("Call t_sndudata with a data size of 0.  Verify that no");
		abstract("data is sent with a subsequent call to t_rcvudata.");
		start
			testing("Call t_sndudata.");
			start
				udt[1]->udata.len = 0;
				t_sndudata( tp[1].fd, udt[1] );
			finis
	
			rendevous();

			set_ndelay( 0 );
	
			testing("Call t_rcvudata with NDELAY set to verify that no");
			testing("data was sent.");
			start
				expecting( TNODATA );		
				t_rcvudata( tp[0].fd, udt[0], &flags );
			finis
			if( !async )
				clr_ndelay( 0 );
		finis
	}

	prep("Initialize the data buffer with the number of test cases.");
	start
/*
 *		put ncases in ascii in case of different reps for 2 machines 
 */
		sprintf(udt[1]->udata.buf,"%d",info[1].tsdu);
		udt[1]->udata.len = strlen(udt[1]->udata.buf)+1;
	finis
	rendevous();
	prep("Call t_sndudata to send the number of outgoing test cases.");
	start
		t_sndudata( tp[1].fd, udt[1] );
		debug(("outgoing test cases: %s",udt[1]->udata.buf));
	finis
	rendevous();
	prep("Call t_rcvudata to receive the number of incoming test cases.");
	start
		t_rcvudata( tp[0].fd, udt[0], &flags );
		sscanf(udt[0]->udata.buf,"%d",&ncases);
		debug(("incoming test cases: %d",ncases));
	finis

	rendevous();

	testing("Call t_sndudata and t_rcvudata for the number of outgoing");
	testing("and incoming test cases respectively.");
	start
		for( i = 1; i <= max( info[1].tsdu, ncases ); i = bound(i) ) {

			if( i <= info[1].tsdu ) {
				memset(udt[1]->udata.buf, (char)i, i );
				udt[1]->udata.len = i;
				t_sndudata( tp[1].fd, udt[1] );
			}
			if( async )
				rendevous();
			if( i <= ncases ) {
				t_rcvudata( tp[0].fd, udt[0], &flags );
				if( async )
					casenum = i;
				else
					casenum = udt[0]->udata.len;
				expecting( VOID );	/* for memcmp */
				if( udt[0]->opt.maxlen != udt[1]->opt.maxlen )
					error("%s%d%s%d%s%d",
					  "Incorrect length of options returned for case of ",
					  casenum,
					  " bytes\nExpected:	", udt[1]->opt.len,
					  "\nReturned:	",udt[0]->opt.maxlen);

				if(memcmp(udt[0]->opt.buf,udt[1]->opt.buf,udt[0]->opt.len))
					error("%s%d%s",
					  "Incorrect options returned for case of ",
					  casenum," bytes");

				if( udt[0]->addr.len != tp[1].clen )
					error("%s%d%s%d%s%d",
					  "Incorrect length of addr returned for case of ",
					  casenum,
					  " bytes\nExpected:	", tp[1].clen,
					  "\nReturned:	",udt[0]->addr.len);

				if( memcmp( udt[0]->addr.buf, tp[1].conn, tp[1].clen ))
					error("%s%d%s",
					  "Incorrect address returned for case of ",
					  casenum, " bytes");
				if( async ) {
					if(udt[0]->udata.len!=min(i,udt[0]->udata.maxlen))
						error("%s%s%d%s%d",
						  "t_rcvudata returned wrong length of data\n",
						  "expected:	",min(i,udt[0]->udata.maxlen),
						  "\nreceived:	",udt[0]->udata.len);
				}
				else 
					if( casenum < 1 || casenum > ncases )
						error("%s%s%d%s%d",
						  "t_rcvudata returned wrong length of data\n",
						  "expected:	a value between 1 and ",ncases,
						  "\nreceived:	",udt[0]->udata.len);

				expecting( SUCCESS );
				for( j = 0; j < min( casenum , udt[0]->udata.maxlen ); j++)
					if( udt[0]->udata.buf[j] != (char)casenum ) {	
						error("%s%d%s%d%s",
						  "wrong data received at loc: ",j,
						  " in test of ",casenum," bytes");
						break;
					}
				k = casenum - udt[0]->udata.len;
				while( k > 0 ) {
					if( !(flags & T_MORE ) ) {
						error("T_MORE not set in flags when expected.");
						break;
					}
					t_rcvudata( tp[0].fd, udt[0], &flags );
					for( j=0; j<min( casenum, udt[0]->udata.maxlen ); j++ )
						if( udt[0]->udata.buf[j] != (char)casenum ) {
							error("%s%d%s%d%s",
							  "wrong data received at loc: ",j+casenum-k,
							  " in test of ",casenum," bytes");
							break;
					}
					if( udt[0]->addr.len )
						error("%s%s",
						  "non-zero length of addr received when ",
						  "calling t_rcvudata after a T_MORE indication.");
					if( udt[0]->opt.len )
						error("%s%s",
						  "non-zero length of addr received when ",
						  "calling t_rcvudata after a T_MORE indication.");
					k -= udt[0]->udata.len;
				}
			}
			if( flags & T_MORE )
				error("T_MORE flag set after all data has been received.");
		finis
	}
}
void do_open( id ) 
int id;
{
	prep("Call t_open to open the transport provider: '%s'.",
	 tp[id].provider );
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
		ret[id] = (struct t_bind *) t_alloc( tp[id].fd, T_BIND, T_ADDR );
		req[id] = (struct t_bind *) t_alloc( tp[id].fd, T_BIND, T_ADDR );
		udt[id] = (struct t_unitdata*) t_alloc( tp[id].fd, T_UNITDATA, T_ALL);
		ops[id] = (struct t_optmgmt*)t_alloc(tp[id].fd, T_OPTMGMT, T_ALL );
		if (RCVUDERR) 
			udr[id]=(struct t_uderr*)t_alloc(tp[id].fd, T_UDERROR, T_ALL );
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
void do_optmgmt( id ) 
int id;
{
	struct	t_optmgmt	req;

	req.opt.len = 0;
	req.opt.buf = NULL;
	req.flags = T_DEFAULT;

	prep("Call t_optmgmt.");
	start
		t_optmgmt( tp[id].fd, &req, ops[id] );
		if( !ops[id]->opt.len )
			warning("default opts not supported by this provider");
	finis
}
void do_free( id ) 
int id;
{
	cleanup("Call t_free to free the allocated structures.");
	start
		t_free( ret[id], T_BIND );
		t_free( req[id], T_BIND );
		t_free( udt[id], T_UNITDATA );
		t_free( ops[id], T_OPTMGMT );
		if (RCVUDERR)
			t_free( udr[id], T_UDERROR );
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
set_opt( id, buf, len )
int id;
char *buf;
int len;
{
	prep("Initialize the opt buffer.");
	start
		memcpy( udt[ id ]->opt.buf, buf, len );
		udt[ id ]->opt.len = len;
	finis
}
void sigfoo( sig )
int sig;
{
	if ( sig != SIGALRM )
		severr("Caught stray signal in signal handler.");
	got_sig = 1;
}
do_error() {
	prep("Call t_look to verify that no unit data error indication");
	prep("initially exists.");
	start
		expecting( 0 );
		t_look( tp[1].fd );
	finis
	prep("Call t_sndudata with a bad address.");
	start
		char *oldaddr, *oldopt;
		int oldolen, oldalen;

		udt[1]->udata.len=1;
		*udt[1]->udata.buf='Z';
		oldalen = udt[1]->addr.len;
		udt[1]->addr.len = TPBADLEN;
		oldaddr = udt[1]->addr.buf;
		udt[1]->addr.buf=TPBADADDR;
#ifdef UNDEF
		oldolen = udt[1]->opt.len;
		udt[1]->opt.len=0;
		oldopt = udt[1]->opt.buf;
		udt[1]->opt.buf=NULL;
#endif
		t_sndudata( tp[1].fd, udt[1] );
		udt[1]->addr.len = oldalen;
		udt[1]->addr.buf = oldaddr;
#ifdef UNDEF
		udt[1]->opt.len = oldolen;
		udt[1]->opt.buf = oldopt;
#endif
	finis		
	prep("Call t_look to verify that a unit data error indication");
	prep("currently exists.");
	start
		expecting( T_UDERR );
		t_look( tp[1].fd );
	finis
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
