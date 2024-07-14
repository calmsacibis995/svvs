#ident "@(#)t_rcvdis.c	1.3"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#define		rendevous()		(	v_sema(), p_sema() )

char *t_alloc();
int  t_free();

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
	{	NULL,	NULL,	0,	NULL,	0,	0,	-1,	},
};

#define NPROVIDERS	(sizeof( tfd )/sizeof(struct endpoint))

struct	t_info		*info[ NPROVIDERS ];			
struct	t_bind		*req[ NPROVIDERS ];
struct	t_bind		*ret[ NPROVIDERS ];
struct	t_call		*call[ NPROVIDERS ];
struct	t_call		*sndcall[ NPROVIDERS ];
struct	t_call		*rcvcall[ NPROVIDERS ];
struct	t_discon	*discon[ NPROVIDERS ];

char	data_buf[11];
char	data_buf2[11];

int	fd, i;
long	data_len[2];




main()
{
	setup("t_rcvdis");
	funct("t_rcvdis");

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

		prep("Initialize a pair of pipes for semaphores.");
		start
			seminit( 1 );
		finis

		prep("Open two transport providers.");
		start
			tfd[0].fd = t_open(TPVCFILE1, O_RDWR, info[0]);
			tfd[1].fd = t_open(TPVCFILE0, O_RDWR|O_NDELAY, info[1]);
		finis


		prep("Get the maximum amount of data allowed to be transferred");
		prep("by the two providers. ");
		prep("Check the discon field of the info structure. If the");
		prep("value is -1 or greater than 10, the data to be ");
		prep("transferred is \"0123456789\" and the length of the ");
		prep("data is 10.");
		prep("If the value is -2 or zero, the length of the data will");
		prep("be zero.");
		prep("If the value {n | 0 < n >= 10}, set the length of the");
		prep("data to n and the data will be set to the first n");
		prep("characters in \"0123456789\".");
		start
			expecting( VOID );
			strcpy(data_buf, "0123456789");
			if((info[0]->discon == -1) || (info[0]->discon > 10))
				data_len[0] = 10;

			if((info[0]->discon == -2) || (info[0]->discon == 0))
				data_len[0] = 0;

			if((info[0]->discon > 0) && (info[0]->discon <= 10))
				data_len[0] = info[0]->discon;

			if(data_len[0])
				data_buf[data_len[0]] = '\0';

			/* do the same for the second provider. */
			strcpy(data_buf2, "0123456789");
			if((info[1]->discon == -1) || (info[1]->discon > 10))
				data_len[1] = 10;

			if((info[1]->discon == -2) || (info[1]->discon == 0))
				data_len[1] = 0;

			if((info[1]->discon > 0) && (info[1]->discon <= 10))
				data_len[1] = info[1]->discon;

			if(data_len[1])
				data_buf2[data_len[1]] = '\0';
		finis


		prep("Allocate space for the req, ret, call, discon,");
		prep("rcvcall and sndcall structures. Copy the address");
		prep("to be bound into the addr buffer");
		start
		   for (i=0; i < 2; i++)
		   {
			req[i] = 
			   (struct t_bind *)t_alloc( tfd[i].fd, T_BIND, T_ALL );
			ret[i] =
			   (struct t_bind *)t_alloc( tfd[i].fd, T_BIND, T_ALL );
			call[i] =
			   (struct t_call *)t_alloc( tfd[i].fd, T_CALL, T_ALL );
			discon[i] =
			   (struct t_discon *)t_alloc(tfd[i].fd, T_DIS, T_ALL);
			rcvcall[i] =
			   (struct t_call *)t_alloc (tfd[i].fd, T_CALL,T_ALL);
			sndcall[i] =
			   (struct t_call *)t_alloc (tfd[i].fd, T_CALL,T_ALL);


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






	abstract("Call t_rcvdis with a file descriptor that is not an");
	abstract("endpoint. Verify the call will fail setting t_errno to");
	abstract("TBADF.");
	start
		break_on( ERROR );

		prep("Call open(BA_OS) to open \"/dev/console\".");
		start
			fd = open("/dev/console", O_RDONLY);
		finis

		testing("Call t_rcvdis with the file descriptor for ");
		testing("\"/dev/console\". Expecting failure and t_errno");
		testing("set to TBADF.");
		start
			expecting( TBADF );
			t_rcvdis(fd, NULL);
		finis

		cleanup("Call close(BA_OS) to close \"/dev/console\".");
		start
			close( fd );
		finis

	finis



	abstract("Verify the TNOTSUPPORT error. Open and bind a valid");
	abstract("data gram endpoint. Call t_rcvdis. Expect failure with");
	abstract("t_errno set to TNOTSUPPORT.");
	start
		break_on( ERROR );

		prep("Open a valid transport endpoint. ");
		start
			fd = t_open(TPDGFILE1, O_RDWR, NULL);
		finis

		prep("Call t_bind with req and ret set to NULL.");
		start
			t_bind(fd, NULL, NULL);
		finis

		testing("Call t_rcvdis. Expecting failure with t_errno");
		testing("set to TNOTSUPPORT.");
		start
			expecting( TNOTSUPPORT );
			t_rcvdis(fd, NULL);
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


	abstract("Verify the TNODIS error.");
	start
		prep("Establish a connection with both providers in the");
		prep("T_DATAXFER state. Call t_listen, t_accept and");
		prep("t_rcvconnect synchronously and t_connect ");
		prep("asynchronously.");
		start
			rendevous();
			expecting( TNODATA );
			t_connect(tfd[1].fd, sndcall[1], rcvcall[1]);
			t_listen(tfd[0].fd, call[0]);
			t_accept(tfd[0].fd, tfd[0].fd, call[0]);
			t_rcvconnect(tfd[1].fd, call[1]);
			rendevous();
		finis

		testing("The passive user calls t_rcvdis with the discon");
		testing("field set to NULL. Expecting failure with t_errno");
		testing("set to TNODIS.");
		start
			expecting( TNODIS );
			t_rcvdis(tfd[0].fd, NULL);
		finis

		rendevous();

		cleanup("Call t_snddis to get the active endpoint in the");
		cleanup("T_IDLE state.");
		start
			t_snddis(tfd[1].fd, NULL);
		finis
		
		cleanup("Call t_rcvdis to get the passive endpoint in the");
		cleanup("T_IDLE state.");
		start
			t_rcvdis(tfd[0].fd, NULL);
		finis
	finis



	abstract("Verify the TBUFOVFLW error. The active user issues a");
	abstract("t_snddis to the passive user while the maxlen field of ");
	abstract("the call structure is set to the maximum length. The ");
	abstract("passive user then calls t_rcvdis with the the maximum ");
	abstract("length for the incoming data set to 0.");
	start

		prep("Establish a connection with both providers in the");
		prep("T_DATAXFER state. Call t_listen, t_accept and");
		prep("t_rcvconnect synchronously and t_connect ");
		prep("asynchronously.");
		start
			rendevous();
			expecting( TNODATA );
			t_connect(tfd[1].fd, sndcall[1], rcvcall[1]);
			t_listen(tfd[0].fd, call[0]);
			t_accept(tfd[0].fd, tfd[0].fd, call[0]);
			t_rcvconnect(tfd[1].fd, call[1]);
			rendevous();
		finis

		prep("Set the length of the data to be transferred to the");
		prep("maximum length  and set the length for the receiving ");
		prep("data to 0.");
		start
			call[1]->udata.len = call[1]->udata.maxlen;
			discon[0]->udata.maxlen=0;
		finis

		prep("Call t_snddis to send data message to the passive");
		prep("user and to get the active user to the T_IDLE state.");
		start
			t_snddis(tfd[1].fd, call[1]);
		finis

		testing("Receive the disconnect request. Expecting failure");
		testing("with t_errno set to TBUFOVFLW.");
		start
			expecting( TBUFOVFLW );
			t_rcvdis(tfd[0].fd, discon[0]);
		finis

		testing("Verify the current state of the endpoint is T_IDLE.");
		start
			expecting( T_IDLE );
			getstate( tfd[0].fd );
		finis

finis




	case_2("active", "T_OUTCON");
	start
		exit_on( ERROR );
		prep("Establish a connection with the passive provider in");
		prep("the T_INCON state and the active provider in the ");
		prep("T_OUTCON state.");
		start
			rendevous();
			expecting( TNODATA );
			t_connect(tfd[1].fd, sndcall[1], rcvcall[1]);
			t_listen(tfd[0].fd, call[0]);
			rendevous();
		finis

		prep("The passive user calls t_snddis.");
		start
			t_snddis(tfd[0].fd, call[0]);
		finis

		testing("The active user calls t_rcvdis with the discon");
		testing("filed set to NULL.");
		start
			t_rcvdis(tfd[1].fd, NULL);
		finis

		getstate( tfd[1].fd );
	finis




	case_3("active", "T_OUTCON");
	start
		exit_on( ERROR );
		prep("Establish a connection with the passive provider in");
		prep("the T_INCON state and the active provider in the ");
		prep("T_OUTCON state.");
		start
			rendevous();
			expecting( TNODATA );
			t_connect(tfd[1].fd, sndcall[1], rcvcall[1]);
			t_listen(tfd[0].fd, call[0]);
			rendevous();
		finis

		prep("Fill the call structure with data and call t_snddis.");
		prep("Set the length of data to %d", data_len[0]);
		prep("and the data to be transferred to \"%s\".", data_buf);
		start
			memcpy(call[0]->udata.buf, data_buf, data_len[0]);
			call[0]->udata.len = data_len[0];
			t_snddis(tfd[0].fd, call[0]);
		finis

		testing("Call t_rcvdis.");
		start
			t_rcvdis(tfd[1].fd, discon[1]);
		finis

		testing("Verify the data transferred was \"%s\".", data_buf);
		start
			expecting( VOID );
			if(memcmp(discon[1]->udata.buf, data_buf, data_len[0]))
				error("The data was incorrectly transferred.");
		finis

		getstate( tfd[1].fd );

	finis

	case_3("passive", "T_INCON");
	start
		exit_on( ERROR );
		prep("Establish a connection with the passive provider in");
		prep("the T_INCON state and the active provider in the ");
		prep("T_OUTCON state.");
		start
			rendevous();
			expecting( TNODATA );
			t_connect(tfd[1].fd, sndcall[1], rcvcall[1]);
			t_listen(tfd[0].fd, call[0]);
			rendevous();
		finis

		prep("Fill the call structure with data and call t_snddis.");
		prep("Set the length of data to %d", data_len[1]);
		prep("and the data to be transferred to \"%s\".", data_buf);
		start
			memcpy(call[1]->udata.buf, data_buf, data_len[1]);
			call[1]->udata.len = data_len[1];
			discon[0]->udata.maxlen = data_len[1];
			t_snddis(tfd[1].fd, call[1]);
		finis

		rendevous();

		testing("Call t_rcvdis.");
		start
			t_rcvdis(tfd[0].fd, discon[0]);
		finis

		testing("Verify the data transferred was \"%s\".", data_buf);
		start
			expecting( VOID );
			if(memcmp(discon[0]->udata.buf, data_buf, data_len[1]))
				error("The data was incorrectly transferred.");
		finis

		getstate( tfd[0].fd );

	finis






	case_2("active", "T_DATAXFER");
	start
		exit_on( ERROR );
		xfer_state();
		null ((int) 0);
	finis

	case_2("passive", "T_DATAXFER");
	start
		exit_on( ERROR );
		xfer_state();
		null ((int) 1);
	finis


	case_3("active", "T_DATAXFER");
	start
		exit_on( ERROR );
		xfer_state();
		snd_data((int ) 0);
	finis


	case_3("passive", "T_DATAXFER");
	start
		exit_on( ERROR );
		xfer_state();
		snd_data((int) 1);
	finis


	case_2("active", "T_OUTREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Call t_sndrel to get the active user in the T_OUTREL");
		prep("state.");
		start
			t_sndrel( tfd[1].fd );
		finis

		null( (int) 0);
	finis

	case_2("passive", "T_OUTREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("The passive user calls t_sndrel to get to the T_OUTREL");
		prep("state.");
		start
			t_sndrel( tfd[0].fd );
		finis

		null( (int) 1);
	finis

	case_3("active", "T_OUTREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Call t_sndrel to get the active user to the T_OUTREL");
		prep("state.");
		start
			t_sndrel( tfd[1].fd );
		finis

		snd_data( (int) 0);
	finis

	case_3("passive", "T_OUTREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Call t_sndrel to get the passive user to the T_OUTREL");
		prep("state.");
		start
			t_sndrel( tfd[0].fd );
		finis

		snd_data( (int) 1);
	finis

	case_2("active", "T_INREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Passive user issues a t_sndrel.");
		start
			t_sndrel( tfd[0].fd );
		finis

		prep("Active user calls t_rcvrel to get to the T_INREL state.");
		start
			t_rcvrel( tfd[1].fd );
		finis

		null( (int) 0);
	finis

	case_3("active", "T_INREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Passive user issues a t_sndrel.");
		start
			t_sndrel( tfd[0].fd );
		finis

		prep("The active user calls t_rcvrel to get to the T_INREL");
		prep("state.");
		start
			t_rcvrel( tfd[1].fd );
		finis

		snd_data((int) 0);
	finis


	case_2("passive", "T_INREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Active user issues a t_sndrel.");
		start
			t_sndrel( tfd[1].fd );
		finis

		prep("The passive user calls t_rcvrel to get to the T_INREL");
		prep("state.");
		start
			t_rcvrel( tfd[0].fd );
		finis
		
		null( (int) 1);
	finis

	case_3("passive", "T_INREL");
	start
		exit_on( ERROR );
		xfer_state();
		prep("Active user issues a t_sndrel.");
		start
			t_sndrel( tfd[1].fd);
		finis


		prep("The passive user calls t_rcvrel to get to the T_INREL");
		prep("state.");
		start
			t_rcvrel( tfd[0].fd );
		finis

		snd_data((int) 1);
	finis




	abstract("Free the allocated space, unbind and close the two");
	abstract("providers.");
	start
		cleanup("Free space allocated for the req, ret, call, discon,");
		cleanup("rcvcall and sndcall structures. ");
		start
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




getstate( fd )
int	fd;
{
	testing("Verify the state of the endpoint is T_IDLE.");
	start
		expecting( T_IDLE );
		t_getstate( fd );
	finis
}

case_2(user, state)
char	*user, *state;
{
	abstract("The %s user calls t_rcvdis from the %s state", user, state);
	abstract("with the call argument set to NULL. Verify t_rcvdis");
	abstract("successfully returns.");
}

case_3(user, state)
char	*user, *state;
{
	abstract("The %s user calls t_rcvdis from the %s state.", user, state);
	abstract("Verify the transferred data is transferred to the %s", user);
	abstract("user.");
}

xfer_state()
{
	prep("Establish a connection with both providers in the");
	prep("T_DATAXFER state. Call t_listen, t_accept and");
	prep("t_rcvconnect synchronously and t_connect ");
	prep("asynchronously.");
	start
		rendevous();
		expecting( TNODATA );
		t_connect(tfd[1].fd, sndcall[1], rcvcall[1]);
		t_listen(tfd[0].fd, call[0]);
		t_accept(tfd[0].fd, tfd[0].fd, call[0]);
		t_rcvconnect(tfd[1].fd, call[1]);
		rendevous();
	finis
}




snd_data( flg )
int	flg;
{
	switch(flg)
	{
	case 0:    /* active user */
		prep("Fill the call structure with data and call t_snddis.");
		prep("Set the length of data to %d", data_len[0]);
		prep("and the data to be transferred to \"%s\".", data_buf);
		start
			memcpy(call[0]->udata.buf, data_buf, data_len[0]);
			call[0]->udata.len = data_len[0];
			t_snddis(tfd[0].fd, call[0]);
		finis

		testing("Call t_rcvdis.");
		start
			t_rcvdis(tfd[1].fd, discon[1]);
		finis

		testing("Verify the data transferred was \"%s\".", data_buf);
		start
			expecting( VOID );
			if(memcmp(discon[1]->udata.buf, data_buf, data_len[0]))
				error("The data was incorrectly transferred.");
		finis

		getstate( tfd[1].fd );

		break;


	case 1:		/* passive user */
		prep("Fill the call structure with data and call t_snddis.");
		prep("Set the length of data to %d", data_len[1]);
		prep("and the data to be transferred to \"%s\".", data_buf);
		start
			memcpy(call[1]->udata.buf, data_buf, data_len[1]);
			call[1]->udata.len = data_len[1];
			t_snddis(tfd[1].fd, call[1]);
		finis

		testing("Call t_rcvdis.");
		start
			t_rcvdis(tfd[0].fd, discon[0]);
		finis

		testing("Verify the data transferred was \"%s\".", data_buf);
		start
			expecting( VOID );
			if(memcmp(discon[0]->udata.buf, data_buf, data_len[1]))
				error("The data was incorrectly transferred.");
		finis

		getstate( tfd[0].fd );
	}
}



null( flg )
int	flg;
{
	switch(flg)
	{
	case 0:    /*active user */
		prep("The passive user calls t_snddis with the call");
		prep("argument set to NULL.");
		start
			t_snddis(tfd[0].fd, NULL);
		finis

		testing("The active user calls t_rcvdis with the discon");
		testing("field set to NULL.");
		start
			t_rcvdis( tfd[1].fd, NULL);
		finis

		getstate( tfd[1].fd );

		break;

	case 1:     /* passive user */
		prep("The active user calls t_snddis with the call");
		prep("argument set to NULL.");
		start
			t_snddis(tfd[1].fd, NULL);
		finis

		testing("The passive user calls t_rcvdis with the discon");
		testing("field set to NULL.");
		start
			t_rcvdis( tfd[0].fd, NULL);
		finis

		getstate( tfd[0].fd );

	}
}



