#ident "@(#)t_connect.c	1.4"
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

int t_errno;

main()
{


	struct	t_call	*call;
	struct	t_call	*call2;
	struct	t_call	*sndcall;
	struct	t_call	*rcvcall;
	struct	t_bind	*ret;
	struct	t_bind	*req;
	struct	t_discon *discon;


	int	tfd1;
	int	tfd2;

	setup("t_connect");
	funct("t_connect");

	abstract("Verify t_connect will successfully return and that the");
	abstract("state will change to T_DATAXFER. Do this in synchronous");
	abstract("mode.");
	start
		prep("Initialize the semaphore mechanism.");
		start
			seminit( 2 );
		finis

		abort_on( ERROR );

	if(fork())
	{

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd1 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the req, ret and call structures for the");
		prep("t_bind and t_listen call. Copy a valid address to the");
		prep("req structure and set the qlen to 1.");
		start
			ret = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			call = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL );
			discon = (struct t_discon *)t_alloc(tfd1, T_DIS, T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			req->qlen = 1;
		finis

		prep("Bind the transport endpoint to the specified address.");
		start
			t_bind(tfd1, req, ret);
		finis

		prep("Call t_listen.");
		start
			v_sema();
			t_listen(tfd1, call);
		finis


		prep("Accept the connect request.");
		start
			t_accept(tfd1, tfd1, call);
		finis
				
		p_sema();
			
		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd1, discon);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd1 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd1 );
		finis
	}
	else
	{		/* child */

		p_sema();

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
		finis

		testing("Call t_connect synchronously.");
		start
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state is T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis to clear the disconnect indication.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis

		v_sema();

		done();   
	}
finis

	abstract("Verify that by calling t_connect with the rcvcall field");
	abstract("set to NULL, it will successfully return.");
	start
		abort_on( ERROR );

	if(fork())
	{

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd1 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the req, ret and call structures for the");
		prep("t_bind and t_listen call. Copy a valid address to the");
		prep("req structure and set the qlen to 1.");
		start
			ret = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			call = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL );
			discon = (struct t_discon *)t_alloc(tfd1, T_DIS, T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			req->qlen = 1;
		finis

		prep("Bind the transport endpoint to the specified address.");
		start
			t_bind(tfd1, req, ret);
		finis

		prep("Call t_listen.");
		start
			v_sema();
			t_listen(tfd1, call);
		finis


		prep("Accept the connect request.");
		start
			t_accept(tfd1, tfd1, call);
		finis
				
		p_sema();
			
		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd1, discon);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd1 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd1 );
		finis
	}
	else
	{		/* child */

		p_sema();

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
		finis

		testing("Call t_connect synchronously.");
		start
			t_connect(tfd2, sndcall, NULL);
		finis

		testing("Verify the current state is T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis to clear the disconnect indication.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis

		v_sema();

		done();   
	}
finis

	abstract("Verify the TNODATA error by opening the transport");
	abstract("endpoint with O_NDELAY set and calling t_connect.");
	abstract("Verify the provider is in the T_OUTCON state after");
	abstract("returning from t_connect call.");
	start
		break_on( ERROR );

		prep("Open two transport endpoints. Open one of them with");
		prep("the O_NDELAY flag set.");
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR|O_NDELAY, NULL);
			tfd1 = t_open(TPVCFILE1, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, call, rcvcall and sndcall");
		prep("structures for the t_bind, t_listen and t_connect");
		prep("calls. Copy a valid address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			call = (struct t_call *)t_alloc (tfd1, T_CALL,T_ALL);
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
		finis

		prep("Bind the passive transport endpoint.");
		start
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
			req->qlen=1;
			t_bind(tfd1, req, ret);
			prep("Check that the requested address was the one bound.");
			start
				expecting( VOID );
				if( ret->addr.len != TPVCALEN1 ||
				  memcmp( ret->addr.buf, TPVCADDR1, TPVCALEN1 ) )
					error("Wrong addr returned by t_bind");
			finis
		finis
		prep("Bind the active transport endpoint.");
		start
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			req->qlen=0;
			t_bind(tfd2, req, ret);
			prep("Check that the requested address was the one bound.");
			start
				expecting( VOID );
				if( ret->addr.len != TPVCALEN0 ||
				  memcmp( ret->addr.buf, TPVCADDR0, TPVCALEN0 ) )
					error("Wrong addr returned by t_bind");
			finis
		finis

		prep("Fill the sndcall structure for the t_connect call.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN1, TPVCCLEN1 );
			sndcall->addr.len = TPVCCLEN1;
		finis

		testing("Call t_connect asynchronously. Expecting failure");
		testing("with t_errno set to TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Call t_listen synchronously.");
		start
			t_listen(tfd1, call);
		finis

		testing("Verify the current state of the active endpoint");
		testing("is T_OUTCON.");
		start
			expecting( VOID );
			if( t_getstate( tfd2 ) == T_IDLE )
				warning( "%s\nt_getstate returned T_IDLE", ADVISORY );
			else {
				expecting( T_OUTCON );
				t_getstate( tfd2 );
			}
		finis

		cleanup("Call t_snddis and t_rcvdis to clear the");
		cleanup("connect indication.");
		start
			t_snddis( tfd2, NULL );
			t_rcvdis( tfd1, NULL );
		finis	

		cleanup("Unbind the two endpoints.");
		start
			t_unbind(tfd2);
			t_unbind( tfd1 );
		finis

		cleanup("Close the two endpoints.");
		start
			t_close( tfd2 );
			t_close( tfd1 );
		finis
	finis

	abstract("Check that calling t_connect with a file descriptor that is");
	abstract("not a transport endpoint will cause failure and set ");
	abstract("t_ernno to TBADF.");
	start
		break_on( ERROR );

		prep("Open a valid transport provider.");
		start
			tfd1 = t_open(TPVCFILE0, O_WRONLY, NULL);
		finis

		prep("Close the endpoint.");
		start
			t_close( tfd1 );
		finis

		testing("Call t_connect with the file descriptor of the");
		testing("closed endpoint. Expecting failure with t_errno");
		testing("set to TBADF.");
		start
			expecting( TBADF );
			t_connect(tfd1, sndcall, NULL);
		finis
	finis

	abstract("Verify the TOUTSTATE error. Open a valid trnasport");
	abstract("endpoint and call t_connect.");
	start
		break_on( ERROR );

		prep("Open a valid transport provider");
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the sndcall and rcvcall structures for the");
		prep("t_connect call.");
		start
			sndcall = (struct t_call *)t_alloc(tfd2, T_CALL, T_ALL);
			rcvcall = (struct t_call *)t_alloc(tfd2, T_CALL, T_ALL);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
		finis

		testing("Call t_connect. Expecting failure with t_errno set");
		testing("to TOUTSTATE.");
		start
			expecting( TOUTSTATE );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		cleanup("Close the transport provider.");
		start
			t_close( tfd2 );
		finis
	finis

	abstract("Verify the TBADADDR error by setting the len field of");
	abstract("the sndcall structure to the maximum length plus one.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = sndcall->addr.maxlen + 1;
		finis

		testing("Call t_connect synchronously. Expecting failure");
		testing("with t_errno set to TBADADDR");
		start
			expecting( TBADADDR );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis
	finis

	abstract("Verify the TBADOPT error by setting the len field of");
	abstract("the sndcall structure to the maximum length plus one.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
			sndcall->opt.len = sndcall->opt.maxlen + 1;
		finis

		testing("Call t_connect synchronously. Expecting failure");
		testing("with t_errno set to TBADOPT");
		start
			expecting( TBADOPT );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis
	finis

	abstract("Verify the TBADDATA error by setting the len field of");
	abstract("the sndcall structure to the maximum length plus one.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
			sndcall->udata.len = sndcall->udata.maxlen + 1;
		finis

		testing("Call t_connect synchronously. Expecting failure");
		testing("with t_errno set to TBADDATA");
		start
			expecting( TBADDATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis
	finis

	abstract("Verify the TBUFOVFLW error by setting the maxlen field of");
	abstract("the address for rcvcall structure to zero. Also verify ");
	abstract("the state of the provider is T_DATAXFER following the");
	abstract("call to t_connect.");
	start
		exit_on( ERROR );

		prep("Open the transport endpoint. ");
		start
			tfd1 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the req, ret and call structures for the");
		prep("t_bind and t_listen call. Copy a valid address to the");
		prep("req structures and set the qlen to 1.");
		start
			ret = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			call = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL );
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			req->qlen = 1;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd1, req, ret);
		finis

		abort_on( ERROR );

	if(fork())
	{

		prep("Call t_listen to listen for a connect indication on");
		prep("the endpoint.");
		start
			v_sema();
			t_listen(tfd1, call);
		finis

		testing("Call t_accept with fd=resfd to establish a ");
		testing("connection on the endpoint.");
		start
			t_accept(tfd1, tfd1, call);
		finis
				
		p_sema();

		cleanup("Receive the disconnect request.");
		start
			t_rcvdis(tfd1, discon);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(ret, T_BIND);
			t_free(req, T_BIND);
			t_free(call, T_CALL);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd1 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd1 );
		finis

	}
	else
	{		/* child */


		p_sema();
		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure and set the maximum length");
		prep("for the address in the rcvcall structure to zero.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
			rcvcall->addr.maxlen = 0;
		finis

		testing("Call t_connect synchronously. Expecting failure");
		testing("with t_errno set to TBUFOVFLW.");
		start
			expecting( TBUFOVFLW );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state of the provider is T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis to get the endpoint in the T_IDLE");
		cleanup("state.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis

		v_sema();

		done();
	}
finis

	abstract("Verify the TLOOK error. Open and bind a valid");
	abstract("transport provider. Fork another process. The parent calls");
	abstract("t_listen and the child calls t_connect. The parent then ");
	abstract("calls t_snddis causing the outstanding connect to fail");
	abstract("setting t_errno to TLOOK.");
	start
		abort_on( ERROR );

	if(fork())
	{

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd1 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the req, ret and call structures for the");
		prep("t_bind and t_listen call. Copy a valid address to the");
		prep("req structure and set the qlen to 1.");
		start
			ret = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			call = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL );
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			req->qlen = 1;
		finis

		prep("Bind the transport endpoint to the specified address.");
		start
			t_bind(tfd1, req, ret);
		finis

		prep("Call t_listen.");
		start
			v_sema();
			t_listen(tfd1, call);
		finis


		prep("Issue a t_snddis to refuse the connect request.");
		start
			t_snddis(tfd1, call);
		finis
				
		rendevous();
			
		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd1 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd1 );
		finis
	}
	else
	{		/* child */

		p_sema();

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd2 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd2, req, ret);
		finis

		prep("Fill the sndcall structure.");
		start
			memcpy( sndcall->addr.buf, TPVCCONN0, TPVCCLEN0 );
			sndcall->addr.len = TPVCCLEN0;
		finis

		testing("Call t_connect. Expecting failure with t_errno set ");
		testing("to TLOOK.");
		start
			expecting( TLOOK );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		p_sema();

		cleanup("Call t_rcvdis to clear the TLOOK error.");
		start
			t_rcvdis(tfd2, NULL);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd2 );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd2 );
		finis

		v_sema();

		done();
	}
finis


}
