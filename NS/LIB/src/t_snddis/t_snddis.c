#ident "@(#)t_snddis.c	1.5"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#define		rendevous()		( v_sema(), p_sema() )

struct endpoint {
	char	*provider;
	char	*addr;
	int	alen;
	char	*conn;
	int	clen;
	int	qlen;
	int	fd;
} tfd[] = {
	{	NULL,	NULL,	0,	NULL,	0,	1,	-1,	},
	{	NULL,	NULL,	0,	NULL,	0,	0,	-1,	},
};

#define NPROVIDERS	(sizeof( tfd )/sizeof(struct endpoint))

struct	t_info		info[ NPROVIDERS ];			
struct	t_bind		*req[ NPROVIDERS ];
struct	t_bind		*ret[ NPROVIDERS ];
struct	t_call		*call[ NPROVIDERS ];
struct	t_call		*sndcall[ NPROVIDERS ];
struct	t_call		*rcvcall[ NPROVIDERS ];
struct	t_discon	*discon[ NPROVIDERS ];

char	*users[] = {
	"passive",
	"active"
};

#define NUSERS	2

char	data_buf[11];
int		data_len;

struct states {
	int	u_state[ NUSERS ];
} states[] = {
	{	T_INCON,	T_OUTCON	},
	{	T_DATAXFER, T_DATAXFER	},
	{	T_INREL,	T_OUTREL	},
	{	T_OUTREL,	T_INREL		}
};

#define	NSTATES	4

main() {
	int	state_id;

	setup("t_snddis");
	funct("t_snddis");

	tfd[0].provider = TPVCFILE0;
	tfd[0].addr = TPVCADDR0;
	tfd[0].alen = TPVCALEN0;
	tfd[0].conn = TPVCCONN0;
	tfd[0].clen = TPVCCLEN0;

	tfd[1].provider = TPVCFILE1;
	tfd[1].addr = TPVCADDR1;
	tfd[1].alen = TPVCALEN1;
	tfd[1].conn = TPVCCONN1;
	tfd[1].clen = TPVCCLEN1;


	abstract("Initialize a semaphore system. Open two transport providers");
	abstract("for communication. Allocate space needed for the system ");
	abstract("calls. Fill the req and sndcall structures and bind the ");
	abstract("two transport endpoints.");
	start
		exit_on( ERROR );

		prep("Initialize the semaphore mechanism.");
		start
			seminit( 1 );
		finis

		prep("Open two transport providers.");
		start
			tfd[0].fd = t_open(TPVCFILE1, O_RDWR, &info[0]);
			tfd[1].fd = t_open(TPVCFILE0, O_RDWR, &info[1]);
		finis

		prep("Get the maximum amount of data allowed to be transferred");
		prep("by the two providers. ");
		prep("Check the discon field of the info structure. If the");
		prep("value is -1 or greater than 10, the data to be ");
		prep("transferred is \"0123456789\" and the length of the ");
		prep("data is 10.");
		prep("If the value is -2 or zero, the length of the data will");
		prep("be zero.");
		prep("If the value is between 1 and 9, set the length of the");
		prep("data to the maximum allowed, and use that many characters");
		prep("in \"0123456789\".");
		prep("The amount of data to be used will be the minimum of");
		prep("the values calculated for both providers.");
		start
			int id, min;
			expecting( VOID );
			min = 10;
			for( id = 0; id < NUSERS; id++ ){
				if((info[id].discon == -1) || (info[id].discon >= 10))
					data_len = 10;
				if((info[id].discon == -2) || !info[id].discon )
					data_len = 0;
				if((info[id].discon > 0) && (info[id].discon < 10))
					data_len = info[id].discon;
				if( data_len < min ) min = data_len;
			}
			data_len = min;
			strcpy(data_buf, "0123456789");
			if(data_len)
				data_buf[data_len] = '\0';
		finis

		prep("Allocate space for the req, ret, call, discon,");
		prep("rcvcall and sndcall structures. Copy the address");
		prep("to be bound into the addr buffer");
		start
			int i;
			char *t_alloc();
	
			for (i=0; i < 2; i++) {
				req[i] = (struct t_bind *)
					t_alloc( tfd[i].fd, T_BIND, T_ALL );
				ret[i] = (struct t_bind *)
					t_alloc( tfd[i].fd, T_BIND, T_ALL );
				call[i] = (struct t_call *)
					t_alloc( tfd[i].fd, T_CALL, T_ALL );
				discon[i] = (struct t_discon *)
					t_alloc(tfd[i].fd, T_DIS, T_ALL);
				rcvcall[i] = (struct t_call *)
					t_alloc (tfd[i].fd, T_CALL,T_ALL);
				sndcall[i] = (struct t_call *)
					t_alloc (tfd[i].fd, T_CALL,T_ALL);
				req[i]->addr.len = tfd[i].alen;
				memcpy( req[i]->addr.buf, tfd[i].addr, tfd[i].alen );
				req[i]->qlen = tfd[i].qlen;
		    }
		finis

		prep("Bind the two transport endpoints.");
		start
			t_bind(tfd[0].fd, req[0], ret[0]);
			t_bind(tfd[1].fd, req[1], ret[1]);
		finis

		prep("Check that the requested address was the one bound.");
		start
			expecting( VOID );
			if( tfd[0].alen != ret[0]->addr.len ||
			  memcmp( ret[0]->addr.buf, tfd[0].addr, tfd[0].alen ) )
				warning("Address bound was not address requested.");
			if( tfd[1].alen != ret[1]->addr.len ||
			  memcmp( ret[1]->addr.buf, tfd[1].addr, tfd[1].alen ) )
				warning("Address bound was not address requested.");
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy(sndcall[0]->addr.buf,ret[1]->addr.buf, ret[1]->addr.len);
			sndcall[0]->addr.len = ret[1]->addr.len;
	        memcpy(sndcall[1]->addr.buf,ret[0]->addr.buf,ret[0]->addr.len);
			sndcall[1]->addr.len = ret[0]->addr.len;
		finis

	finis

/*
 *	state_id:  
 *		0 - T_INCON (passive), T_OUTCON (active)
 *		1 - T_DATAXFER (both)
 *		2 - T_INREL (passive), T_OUTREL (active)
 *		3 - T_OUTREL (passive), T_INREL (active)
 */
	for( state_id = 0; state_id < NSTATES; state_id++ ) {
		tst_null( 1, 0, state_id );
		tst_null( 0, 1, state_id );
		tst_nodata( 1, 0, state_id );
		tst_nodata( 0, 1, state_id );
		tst_send( 1, 0, state_id );
		tst_send( 0, 1, state_id );
		tst_tlook( 1, 0, state_id );
		tst_tlook( 0, 1, state_id );
	}

	abstract("Call t_snddis with a file descriptor that is not a");
	abstract("endpoint. Verify the call will fail setting t_errno to");
	abstract("TBADF.");
	start
		int fd;
		break_on( ERROR );

		prep("Call open(BA_OS) to open \"/dev/console\".");
		start
			fd = open("/dev/console", O_WRONLY);
		finis

		testing("Call t_snddis with the file descriptor for ");
		testing("\"/dev/console\". Expecting failure and t_errno");
		testing("set to TBADF.");
		start
			expecting( TBADF );
			t_snddis(fd, NULL);
		finis

		cleanup("Call close(BA_OS) to close \"/dev/console\".");
		start
			close( fd );
		finis
	finis


	abstract("Verify the TNOTSUPPORT error. Open and bind a valid");
	abstract("data gram endpoint. Call t_snddis. Expect failure with");
	abstract("t_errno set to TNOTSUPPORT.");
	start
		int fd;
		break_on( ERROR );
		prep("Open a valid transport endpoint. ");
		start
			fd = t_open(TPDGFILE1, O_RDWR, NULL);
		finis

		prep("Call t_bind with req and ret set to NULL.");
		start
			t_bind(fd, NULL, NULL);
		finis

		testing("Call t_snddis. Expecting failure with t_errno");
		testing("set to TNOTSUPPORT.");
		start
			expecting( TNOTSUPPORT );
			t_snddis(fd, NULL);
		finis
				
		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( fd );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( fd );
		finis
	finis


	abstract("Call t_snddis from the T_IDLE state.  Verify");
	abstract("failure with t_errno set to TOUTSTATE.");
	start
		break_on( ERROR );

		testing("Call t_snddis with call set to NULL. Expecting ");
		testing("failure with t_errno set to TOUTSTATE.");
		start
			expecting( TOUTSTATE );
			t_snddis(tfd[0].fd, NULL);
		finis
				
	finis


	abstract("Verify the TBADDATA error. Set the length of the data");
	abstract("field to the maximum length plus one.");
	start
		break_on( ERROR );

		get_to_state( T_DATAXFER );

		prep("Set the length of the transferred data for the t_snddis");
		prep("call to the maximum length plus one.");
		start
			call[1]->udata.len = call[1]->udata.maxlen + 1;
		finis

		rendevous();

		testing("Call t_snddis. Expecting failure with t_errno set");
		testing("to TBADDATA.");
		start
			expecting( TBADDATA );
			t_snddis(tfd[1].fd, call[1]);
		finis

		cleanup("Call t_snddis to get the endpoint in T_IDLE state.");
		start
			t_snddis(tfd[1].fd, NULL);
		finis

		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd[0].fd, NULL);
		finis
	finis

	abstract("Free the allocated space, unbind and close the two");
	abstract("providers.");
	start
		cleanup("Free space allocated for the req, ret, call, discon,");
		cleanup("rcvcall and sndcall structures. ");
		start
			int i;
			for(i=0; i <2; i++)
			{
				t_free(req[i], T_BIND);
				t_free(ret[i], T_BIND); 
				t_free(call[i], T_CALL);
				t_free(discon[i], T_DIS);
				t_free(rcvcall[i], T_CALL);
				t_free(sndcall[i], T_CALL);
			}
		finis

		cleanup("Unbind the two providers.");
		start
			t_unbind( tfd[0].fd );
			t_unbind( tfd[1].fd );
		finis

		cleanup("Close the two providers.");
		start
			t_close( tfd[0].fd );
			t_close( tfd[1].fd );
		finis
	finis

	done();
}

tst_null(id1, id2, state_id) 
int id1, id2;
int state_id;
{
	char *sname();
/*
 *	cannot send disconnect with NULL to reject connect indication
 */
	if( states[state_id].u_state[id1] == T_INCON ) return;

	abstract("The %s user calls t_snddis from the %s state", 
		users[id1], sname( states[state_id].u_state[id1] ));
	abstract("with the call argument set to NULL. Verify t_snddis");
	abstract("successfully returns.");
	start
		exit_on( ERROR );
		
		get_to_state( states[state_id].u_state[1] );

		testing("Call t_snddis from the %s user with the call argument",
		  users[ id1 ]);
		testing("set to NULL.");
		start
			t_snddis(tfd[id1].fd, NULL);
		finis

		chk_idle( id1 );

		rendevous();

		cleanup("Receive the disconnect request.");
		start
			t_rcvdis( tfd[id2].fd, NULL );
		finis

		chk_idle( id2 );

		rendevous();

	finis
}
tst_nodata(id1, id2, state_id) 
int id1, id2;
int state_id;
{
	char *sname();

	abstract("The %s user calls t_snddis from the %s state", 
		users[id1], sname( states[state_id].u_state[id1] ));
	abstract("with the length of the data field set to zero.  Verify");
	abstract("t_snddis successfully returns.");
	start
		exit_on( ERROR );
		
		get_to_state( states[state_id].u_state[1] );

		testing("Call t_snddis from the %s user with the length of",
		  users[ id1 ]);
		testing("transferred data set to zero.");
		start
			call[id1]->udata.len = 0;
			t_snddis(tfd[id1].fd, call[id1]);
		finis

		chk_idle( id1 );

		rendevous();

		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd[id2].fd, discon[id2]);
		finis

		testing("Verify the length of data transferred is zero.");
		start
			continue_on( ERROR );
			if( discon[id2]->udata.len )
				error("The length of the received data buffer is non-zero.");
		finis

		chk_idle( id2 );

		rendevous();

	finis
}
tst_send(id1, id2, state_id) 
int id1, id2;
int state_id;
{
	char *sname();

	abstract("The %s user calls t_snddis from the %s state.", 
		users[id1], sname( states[state_id].u_state[id1] ));
	abstract("Verify data is transferred to the %s user.", users[id2]);
	start
		exit_on( ERROR );
		
		get_to_state( states[state_id].u_state[1] );

		testing("Call t_snddis from the %s user with %d bytes of data",
			users[ id1 ], data_len );
		testing("to be transferred.");
		start
			memcpy(call[id1]->udata.buf, data_buf, data_len);
			call[id1]->udata.len = data_len;
			t_snddis(tfd[id1].fd, call[id1]);
		finis

		chk_idle( id1 );

		rendevous();

		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd[id2].fd, discon[id2]);
		finis

		testing("Verify the length of data transferred is %d.", data_len);
		start
			continue_on( ERROR );
			if( discon[id2]->udata.len != data_len )
				error("The length of the received data buffer is not %d.",
				  data_len);
		finis

		testing("Verify the data transferred was \"%s\".", data_buf);
		start
			continue_on( ERROR );
			expecting( VOID );
			if(memcmp(discon[id2]->udata.buf, data_buf, data_len))
				error("The data was incorrectly transferred.");
		finis

		chk_idle( id2 );

		rendevous();

	finis
}
tst_tlook(id1, id2, state_id) 
int id1, id2;
int state_id;
{
	char *sname();

	abstract("The %s user calls t_snddis from the %s state.", 
		users[id1], sname( states[state_id].u_state[id1] ));
	abstract("Verify the occurrance of an asynchronous event on the");
	abstract("endpoint while in this state.");
	start
		exit_on( ERROR );
		
		get_to_state( states[state_id].u_state[1] );

		prep("Call t_snddis from the %s user to generate an", 
		  users[ id2 ]);
		prep("asynchronous event for the %s user.", users[ id1 ]);
		start
			if( states[state_id].u_state[id2] == T_INCON )
				t_snddis( tfd[id2].fd, call[id2] );
			else 
				t_snddis( tfd[id2].fd, NULL );
		finis

		chk_idle( id2 );

		rendevous();

		testing("Call t_snddis expecting failure with t_errno");
		testing("set to TLOOK.");
		start
			expecting( TLOOK );
			if( states[state_id].u_state[id1] == T_INCON )
				t_snddis( tfd[id1].fd, call[id1] );
			else 
				t_snddis( tfd[id1].fd, NULL );
		finis

		rendevous();

		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd[ id1 ].fd, NULL );
		finis

		chk_idle( id1 );

		rendevous();

	finis
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

			sndcall[1]->addr.len = tfd[0].clen;
			memcpy( sndcall[1]->addr.buf, tfd[0].conn, tfd[0].clen );
			expecting( TNODATA );
			t_connect( tfd[1].fd, sndcall[1], rcvcall[1] );

			clr_ndelay( 1 );

		finis
		prep("Call t_listen synchronously for passive user.");
		start
			t_listen( tfd[0].fd, call[0] );
		finis
		prep("Check the returned addr corresponds to active user.");
		start
			expecting( VOID );
			if( tfd[1].clen != call[0]->addr.len ||
			  memcmp( call[0]->addr.buf, tfd[1].conn, tfd[1].clen ) )
				warning("Wrong addr returned by t_listen.");
		finis
		chk_incon( 0 );
		chk_outcon( 1 );
		rendevous();
		if( state != T_OUTCON ) {
			prep("Call t_accept for passive user.");
			start
				t_accept( tfd[0].fd, tfd[0].fd, call[0] );
			finis
			prep("Call t_rcvconnect for the active user.");
			start
				t_rcvconnect( tfd[1].fd, rcvcall[1] );
			finis
			prep("Check that the active user addr is returned.");
			start
				expecting( VOID );
				if( tfd[0].clen != rcvcall[1]->addr.len ||
				  memcmp( rcvcall[1]->addr.buf, tfd[0].conn, tfd[0].clen ) )
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
				t_sndrel( tfd[0].fd );
				chk_outrel( 0 );
				rendevous();
				t_rcvrel( tfd[1].fd );
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
				t_sndrel( tfd[1].fd );
				chk_outrel( 1 );
				rendevous();
				t_rcvrel( tfd[0].fd );
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
set_ndelay( id ) 
int id;
{
	int	oflag;

	prep("Call fcntl to get the file status flags.  If the");
	prep("O_NDELAY file status flag is not set, call fcntl");
	prep("again to set it.");
	start
		int oflag;
		expecting( VOID );
		oflag = fcntl( tfd[id].fd, F_GETFL, NULL );
		if( !(oflag & O_NDELAY ) ) {
			expecting( SUCCESS );
			fcntl( tfd[id].fd, F_SETFL, oflag | O_NDELAY );
		}
	finis
}
clr_ndelay( id ) 
int id;
{
	int	oflag;

	prep("Call fcntl to get the file status flags.  If the");
	prep("O_NDELAY file status flag is set, call fcntl");
	prep("again to clear it.");
	start
		int oflag;

		expecting( VOID );
		oflag = fcntl( tfd[id].fd, F_GETFL, NULL );
		if( (oflag & O_NDELAY ) ) {
			expecting( SUCCESS );
			fcntl( tfd[id].fd, F_SETFL, oflag & ~O_NDELAY );
		}
	finis
}
chk_idle( id ) 
int id;
{
	chk_state( tfd[id].fd ,T_IDLE );
}
chk_incon( id ) 
int id;
{
	chk_state( tfd[id].fd, T_INCON );
}
chk_outcon( id ) 
int id;
{
	chk_state( tfd[id].fd, T_OUTCON );
}
chk_dataxfer( id ) 
int id;
{
	chk_state( tfd[id].fd, T_DATAXFER );
}
chk_inrel( id ) 
int id;
{
	chk_state( tfd[id].fd, T_INREL );
}
chk_outrel( id ) 
int id;
{
	chk_state( tfd[id].fd, T_OUTREL );
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
