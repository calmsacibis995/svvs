#ident "@(#)t_accept.c	1.5"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>
#define		TMPFILE			"t_accept.tmp"
#define		rendevous()		(	v_sema(), p_sema() )

#define	max( a, b )		(( a > b ) ? a : b )
void	chk_unbnd();
void	chk_incon();
void	chk_outcon();
void	chk_idle();
void	chk_dataxfer();
void	do_open();
void	do_close();
void	do_alloc();
void	do_free();
void	do_bind();
void	do_unbind();
void	set_ndelay();
void	set_flags();

char *t_alloc();
int	pid;
struct endpoint {
	char	*provider;
	char	*addr;
	int		alen;
	char	*conn;
	int		clen;
	int		qlen;
	int		fd;
} tp[] = {
	{	NULL,	NULL,	0,	NULL,	0,	2,	-1,	},
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
struct	t_discon 	*discon[NPROVIDERS ];

main() {

	setup("t_accept");
	funct("t_accept");

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

	tp[2].provider = TPVCFILE2;	
	tp[2].addr = TPVCADDR2;
	tp[2].alen = TPVCALEN2;
	tp[2].conn = TPVCCONN2;
	tp[2].clen = TPVCCLEN2;

	tp[3].provider = TPVCFILE3;	
	tp[3].addr = TPVCADDR3;
	tp[3].alen = TPVCALEN3;
	tp[3].conn = TPVCCONN3;
	tp[3].clen = TPVCCLEN3;

	prep("Initialize the semaphore mechanism.");
	start
		seminit( 2 );
	finis

	prep("Initialize the endpoints.");
	start
		abort_on( ERROR );

		prep("Open, allocate, and bind the passive user which has");
		prep("qlen set to 2.");
		start
			do_open( 0 );
			do_alloc( 0 );
			do_bind( 0 );
			chk_idle( 0 );
		finis

		prep("Open, allocate, and bind the passive user which has");
		prep("qlen set to 1.");
		start
			do_open( 1 );
			do_alloc( 1 );
			do_bind( 1 );
			chk_idle( 1 );
		finis

		prep("Open, allocate, and bind the first active user.");
		start
			do_open( 2 );
			do_alloc( 2 );
			do_bind( 2 );
			chk_idle( 2 );
		finis

		prep("Open, allocate, and bind the second active user.");
		start
			do_open( 3 );
			do_alloc( 3 );
			do_bind( 3 );
			chk_idle( 3 );
		finis
	finis

	abstract("Verify that t_accept will accept a connection on a local");
	abstract("transport endpoint when fd=resfd.");
	start
		int pid;

		if( pid = fork()){
			prep("Call t_listen.  Verify that the returned address");
			prep("corresponds to the active user.");
			start
				v_sema();
				t_listen( tp[1].fd, rcall[2] );
				expecting( VOID );
				if( tp[2].clen != rcall[2]->addr.len ||
				  memcmp( rcall[2]->addr.buf, tp[2].conn, tp[2].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			chk_incon( 1 );
			testing("Call t_accept with fd=resfd to establish a ");
			testing("connection on the endpoint.");
			start
				t_accept(tp[1].fd, tp[1].fd, rcall[2] );
			finis
			p_sema();
			chk_dataxfer( 1 );
			cleanup("Call t_snddis.");
			start
				t_snddis(tp[1].fd, NULL );
			finis
			rendevous();
			cleanup("wait for the child process.");
			start
				expecting( pid );
				wait( (int *)0 );
			finis
		}
		else {
			p_sema();
			prep("Call t_connect to issue a connect request on the");
			prep("endpoint.");
			start
				memcpy( scall[2]->addr.buf, tp[1].conn, tp[1].clen );
				scall[2]->addr.len = tp[1].clen;
				t_connect(tp[2].fd, scall[2], rcall[1]);
				expecting( VOID );
				if( tp[1].clen != rcall[1]->addr.len ||
				  memcmp( rcall[1]->addr.buf, tp[1].conn, tp[1].clen ))
					warning("Wrong addr returned by t_connect.");
			finis
			chk_dataxfer( 2 );
			rendevous();
			cleanup("Call t_rcvdis.");
			start
				t_rcvdis(tp[2].fd, NULL);
			finis
			v_sema();
			done();
		}
		do_sync();
	finis


	abstract("Verify that t_accept will accept a connection on a local");
	abstract("endpoint other than that which was listening for a connect");
	abstract("request. Verify that after the connection, the endpoint on");
	abstract("which the connect request was made has a state of T_IDLE,");
	abstract("and the endpoint which the connect request was accepted");
	abstract("has a state of T_DATAXFER.");
	start
		if( pid = fork()){
			prep("Call t_listen.  Verify that the returned address");
			prep("corresponds to the active user.");
			start
				v_sema();
				t_listen( tp[0].fd, rcall[1] );
				expecting( VOID );
				if( tp[2].clen != rcall[1]->addr.len ||
				  memcmp( rcall[1]->addr.buf, tp[2].conn, tp[2].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			chk_incon( 0 );
			testing("Call t_accept with fd!=resfd to establish a ");
			testing("connection on the secondary passive endpoint.");
			start
				t_accept(tp[0].fd, tp[1].fd, rcall[1] );
			finis
			p_sema();
			chk_idle( 0 );
			chk_dataxfer( 1 );
			cleanup("Call t_snddis.");
			start
				t_snddis(tp[1].fd, NULL );
			finis
			rendevous();
			cleanup("wait for the child process.");
			start
				expecting( pid );
				wait( (int *)0 );
			finis
		}
		else {
			p_sema();
			prep("Call t_connect to issue a connect request on the");
			prep("endpoint.");
			start
				memcpy( scall[2]->addr.buf, tp[0].conn, tp[0].clen );
				scall[2]->addr.len = tp[0].clen;
				t_connect(tp[2].fd, scall[2], rcall[1]);
				expecting( VOID );
				if( tp[1].clen != rcall[1]->addr.len ||
				  memcmp( rcall[1]->addr.buf, tp[1].conn, tp[1].clen ))
					warning("Wrong addr returned by t_connect.");
			finis
			chk_dataxfer( 2 );
			rendevous();
			cleanup("Call t_rcvdis.");
			start
				t_rcvdis(tp[2].fd, NULL);
			finis
			v_sema();
			done();
		}
		do_sync();
	finis


	abstract("Check that calling t_accept with a file descriptor that is");
	abstract("not a transport endpoint will cause failure and set ");
	abstract("t_errno to TBADF.");
	start
		int fd;

		break_on( ERROR );

		prep("Open a temp file '%s'.", TMPFILE );
		start
			fd = temp_open( TMPFILE, O_RDWR | O_CREAT, 0666 );
		finis

		testing("Call t_accept with the file descriptor for ");
		testing("'%s'. Expecting failure and t_errno", TMPFILE);
		testing("set to TBADF.");
		start
			expecting( TBADF );
			t_accept( fd, fd, rcall[0] );
		finis

		cleanup("Call close(BA_OS) to close '%s'.", TMPFILE);
		start
			close( fd );
		finis
	finis

	abstract("Verify the TLOOK error. Open and bind a valid transport");
	abstract("endpoint. Call t_listen to listen for a connect request.");
	abstract("When the connect request has arrived, the active user");
	abstract("sends a second request, for which the passive user does");
	abstract("not listen.  When the second ");
	abstract("connect request has arrived, call t_accept with fd=resfd.");
	abstract("T_accept should fail with t_errno set to TLOOK.");
	start
		if( pid = fork()){
			prep("Call t_listen.  Verify that the returned address");
			prep("corresponds to the first active user.");
			start
				v_sema();
				t_listen( tp[0].fd, rcall[2] );
				expecting( VOID );
				if( tp[2].clen != rcall[2]->addr.len ||
				  memcmp( rcall[2]->addr.buf, tp[2].conn, tp[2].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			chk_incon( 0 );
			p_sema();
			testing("Call t_accept.");
			start
				expecting( TLOOK );
				t_accept(tp[0].fd, tp[1].fd, rcall[2] );				
			finis
			cleanup("Call t_listen.  Verify that the returned address");
			cleanup("corresponds to the second active user.");
			start
				t_listen( tp[0].fd, rcall[3] );
				expecting( VOID );
				if( tp[3].clen != rcall[3]->addr.len ||
				  memcmp( rcall[3]->addr.buf, tp[3].conn, tp[3].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			chk_incon( 0 );
			cleanup("Call t_snddis for the first active user.");
			start
				t_snddis(tp[0].fd, rcall[2] );
			finis
			rendevous();
			cleanup("Call t_snddis for the second active user.");
			start
				t_snddis(tp[0].fd, rcall[3] );
			finis
			rendevous();
			cleanup("wait for the child process.");
			start
				expecting( pid );
				wait( (int *)0 );
			finis

		}
		else {
			p_sema();
			prep("Call t_connect to issue a connect request from the");
			prep("first active endpoint.");
			start
				set_ndelay( 2 );
				memcpy( scall[2]->addr.buf, tp[0].conn, tp[0].clen );
				scall[2]->addr.len = tp[0].clen;
				expecting( TNODATA );
				t_connect(tp[2].fd, scall[2], rcall[0]);
			finis
			chk_outcon( 2 );
			prep("Call t_connect to issue a connect request from the");
			prep("second active endpoint.");
			start
				set_ndelay( 3 );
				memcpy( scall[3]->addr.buf, tp[0].conn, tp[0].clen );
				scall[3]->addr.len = tp[0].clen;
				expecting( TNODATA );
				t_connect(tp[3].fd, scall[3], rcall[0]);
			finis
			chk_outcon( 3 );
			rendevous();
			cleanup("Call t_rcvdis for the first active user.");
			start
				t_rcvdis(tp[2].fd, NULL);
			finis
			rendevous();
			cleanup("Call t_rcvdis for the second active user."); 
			start
				t_rcvdis(tp[3].fd, NULL);
			finis
			v_sema();
			done();
		}
		do_sync();
	finis

	abstract("Verify the TLOOK error. The passive user issues a");
	abstract("t_listen on the endpoint. The active user calls t_connect");
	abstract("asynchronously.  After the passive user returns from");
	abstract("t_listen, the active user call t_snddis.  The passive");
	abstract("then calls t_accept, expecting failure with t_errno set");
	abstract("to TLOOK.");
	start
		if( pid = fork()){
			prep("Call t_listen.  Verify that the returned address");
			prep("corresponds to the active user.");
			start
				v_sema();
				t_listen( tp[0].fd, rcall[2] );
				expecting( VOID );
				if( tp[2].clen != rcall[2]->addr.len ||
				  memcmp( rcall[2]->addr.buf, tp[2].conn, tp[2].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			p_sema();
			testing("Call t_accept. Expecting failure with");
			testing("t_errno set to TLOOK.");
			start
				expecting( TLOOK );
				t_accept(tp[0].fd, tp[1].fd, rcall[2] );				
			finis

			chk_aincon( 0 );
			cleanup("Call t_rcvdis to clear the TLOOK.");
			start
				t_rcvdis(tp[0].fd, NULL );
			finis

			cleanup("wait for the child process.");
			start
				expecting( pid );
				wait( (int *)0 );
			finis

			do_sync();
		}
		else {
			p_sema();
			set_ndelay( 2 );
			prep("Call t_connect to issue a connect request from the");
			prep("active endpoint.");
			start
				memcpy( scall[2]->addr.buf, tp[0].conn, tp[0].clen );
				scall[2]->addr.len = tp[0].clen;
				expecting( TNODATA );
				t_connect(tp[2].fd, scall[2], rcall[0]);
			finis
			chk_outcon( 2 );
			prep("Call t_snddis to generate an asynchronous event.");
			start
				t_snddis(tp[2].fd, NULL);
			finis

			v_sema();
			done();
		}
	finis


	abstract("Verify the TOUTSTATE error. Call t_accept from the T_IDLE");
	abstract("state. Expect failure with t_errno set to TOUTSTATE.");
	start
		break_on( ERROR );		

		testing("Call t_accept with fd=resfd. Expecting failure");
		testing("with t_errno set to TOUTSTATE.");
		start
			expecting( TOUTSTATE );
			t_accept(tp[0].fd, tp[0].fd, rcall[0]);
		finis
	finis


	abstract("Verify the TOUTSTATE error. Call t_accept with a resfd");
	abstract("which is not in the T_IDLE");
	abstract("state. Expect failure with t_errno set to TOUTSTATE.");
	start
		prep("Unbind the resfd endpoint.");
		start
			t_unbind( tp[1].fd );
		finis

		prep ("Fork a subtest");
		start
		if( pid = fork()){
			prep("Call t_listen.  Verify that the returned address");
			prep("corresponds to the active user.");
			start
				v_sema();
				t_listen( tp[0].fd, rcall[2] );
				expecting( VOID );
				if( tp[2].clen != rcall[2]->addr.len ||
				  memcmp( rcall[2]->addr.buf, tp[2].conn, tp[2].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			chk_incon( 0 );
			p_sema();
			testing("Call t_accept with fd!=resfd to establish a ");
			testing("connection on the secondary passive endpoint.");
			start
				expecting( TOUTSTATE );
				t_accept(tp[0].fd, tp[1].fd, rcall[2] );
			finis
			chk_incon( 0 );
			chk_unbnd( 1 );
			cleanup("Call t_snddis.");
			start
				t_snddis(tp[0].fd, rcall[2] );
			finis
			rendevous();
			cleanup("wait for the child process.");
			start
				expecting( pid );
				wait( (int *)0 );
			finis

			cleanup("Bind the resfd endpoint.");
			start
				do_bind( 1 );
			finis
			do_sync();
		}
		else {
			p_sema();
			set_ndelay( 2 );
			prep("Call t_connect to issue a connect request on the");
			prep("endpoint.");
			start
				memcpy( scall[2]->addr.buf, tp[0].conn, tp[0].clen );
				scall[2]->addr.len = tp[0].clen;
				expecting( TNODATA );
				t_connect(tp[2].fd, scall[2], rcall[0]);
			finis
			chk_outcon( 2 );
			rendevous();
			cleanup("Call t_rcvdis.");
			start
				t_rcvdis(tp[2].fd, NULL);
			finis
			v_sema();
			done();
		}
		finis  /*  end of fork  */
	finis

	abstract("Verify the TNOTSUPPORT error. Open and bind a valid");
	abstract("data gram endpoint. Call t_accept. Expect failure with");
	abstract("t_errno set to TNOTSUPPORT.");
	start
		int	fd;

		break_on( ERROR );

		prep("Open a valid transport endpoint. ");
		start
			fd = t_open(TPDGFILE0, O_RDWR, NULL);
		finis

		prep("Call t_bind with req and ret set to NULL.");
		start
			t_bind(fd, NULL, NULL);
		finis

		testing("Call t_accept with fd=resfd. Expecting failure");
		testing("with t_errno set to TNOTSUPPORT.");
		start
			expecting( TNOTSUPPORT );
			t_accept(fd, fd, rcall[0]);
		finis
				
		cleanup("Unbind the endpoint.");
		start
			t_unbind( fd );
		finis
		cleanup("Close the transport endpoint.");
		start
			t_close( fd );
		finis
	finis

	abstract("Verify the TBADDATA error.  Call t_accept with a");
	abstract("data size greater than the amount of data that can");
	abstract("be sent with a connect request.  The maximum data");
	abstract("amount of data that can be sent with a connect request");
	abstract("for this provider, as returned by the t_open call in");
	abstract("info.connect, is %d.", info[1].connect);
	start
		int pid;

		if( pid = fork()){
			prep("Call t_listen.  Verify that the returned address");
			prep("corresponds to the active user.");
			start
				v_sema();
				t_listen( tp[1].fd, rcall[1] );
				expecting( VOID );
				if( tp[2].clen != rcall[1]->addr.len ||
				  memcmp( rcall[1]->addr.buf, tp[2].conn, tp[2].clen ))
					warning("Wrong addr returned by t_listen.");
			finis
			chk_incon( 1 );
			testing("Call t_accept with an illegal data size.");
			start
				int	size;

				size = info[1].connect;
				if( !size )
					message(
					  "concept of data transfer with a conn. req. not supported for this provider.");
				else if( size == -1  )
					message(
					  "finite data transfer with a conn. req. not supported for this provider.");
				else if( size < -1  )
					message("data transfer with a conn. req. not supported for this provider.");
				else  {
					rcall[1]->udata.len = size + 1;
					expecting( TBADDATA );
					t_accept(tp[1].fd, tp[1].fd, rcall[1] );
				}
			finis
			chk_incon( 1 );
			cleanup("Call t_snddis.");
			start
				rcall[1]->udata.len = 0;
				t_snddis(tp[1].fd, rcall[1] );
			finis
			rendevous();
			cleanup("wait for the child process.");
			start
				expecting( pid );
				wait( (int *)0 );
			finis

			do_sync();
		}
		else {
			p_sema();
			set_ndelay( 2 );
			prep("Call t_connect to issue a connect request on the");
			prep("endpoint.");
			start
				memcpy( scall[2]->addr.buf, tp[1].conn, tp[1].clen );
				scall[2]->addr.len = tp[1].clen;
				expecting( TNODATA );
				t_connect(tp[2].fd, scall[2], rcall[1]);
			finis
			p_sema();
			cleanup("Call t_rcvdis.");
			start
				t_rcvdis(tp[2].fd, NULL);
			finis
			v_sema();
			done();
		}
	finis
	done();
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
		expecting( VOID );
		oflag = fcntl( tp[id].fd, F_GETFL, NULL );
		if( !(oflag & O_NDELAY ) )
		{
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
chk_aincon( id ) 
int id;
{
	chk_astate( tp[id].fd, T_INCON, T_IDLE );
}
chk_astate( fd, state, astate )
int fd;
int state;
int astate;
{
	char *st_name;
	char *st_aname;
	char *sname();

	st_name = sname( state );
	st_aname = sname( astate );
	prep("Call t_getstate, verify the current state is %s.", st_name);
	start
		expecting( VOID );
		if( t_getstate( fd ) == astate )
			warning("%s\nt_getstate returned %s.", ADVISORY, 
			  st_aname);
		else {
			continue_on( ERROR );
			expecting( state );
			t_getstate( fd );
		}
	finis
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

do_sync()
{
	cleanup("Call t_sync to  synchronize the endpoints.");
	start
		expecting( T_IDLE );
		t_sync( tp[0].fd );
		expecting( T_IDLE );
		t_sync( tp[1].fd );
		expecting( T_IDLE );
		t_sync( tp[2].fd );
		expecting( T_IDLE );
		t_sync( tp[3].fd );
	finis
}

spit_call( str, call )
char *str;
struct t_call *call;
{

	sv_printf("%s:\n",str);
	sv_printf("addr:	");
	spit_netbuf( call->addr.buf, call->addr.len, call->addr.maxlen );
 	sv_printf("opt:	");
	spit_netbuf( call->opt.buf, call->opt.len, call->opt.maxlen );
	sv_printf("udata:	");
	spit_netbuf( call->udata.buf, call->udata.len, call->udata.maxlen);
	sv_printf("sequence: %d\n",call->sequence);
}
spit_netbuf( buf, len, maxlen ) 
char	*buf;
int	len;
int	maxlen;
{
	int	i;
	sv_printf("buf: 0x%x	",buf);
	if( buf ) {
		for( i=0; i<len ; i++ )	
			sv_printf("\\%o",(buf[i]&0x00ff));
	}
	sv_printf("[%d:%d]\n",len,maxlen);
}
