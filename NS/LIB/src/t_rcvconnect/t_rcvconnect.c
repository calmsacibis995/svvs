#ident "@(#)t_rcvconnt.c	1.2"
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

main()
{
	struct	t_call	*call;
	struct	t_call	*call2;
	struct	t_call	*call3;
	struct	t_call	*sndcall;
	struct	t_call	*sndcall2;
	struct	t_call	*sndcall3;
	struct	t_call	*rcvcall;
	struct	t_call	*rcvcall2;
	struct	t_call	*rcvcall3;
	struct	t_bind	*ret;
	struct	t_bind	*ret2;
	struct	t_bind	*ret3;
	struct	t_bind	*req;
	struct	t_bind	*req2;
	struct	t_bind	*req3;
	struct	t_discon *discon;

	int	tfd1;
	int	tfd2;
	int	tfd3;
	int	oflags;

	setup("t_rcvconnect");
	funct("t_rcvconnect");



	abstract("Open the provider asynchronously and verify the state of");
	abstract("the endpoint before the t_rcvconnect call is T_OUTCON and");
	abstract("after the t_rcvconnect call is T_DATAXFER.");
	start
		prep("Initialize the semaphore mechanism.");
		start
			seminit( 2 );
		finis

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

		p_sema();

		prep("Call t_accept with fd=resfd to establish a ");
		prep("connection on the endpoint.");
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
		prep("Open transport endpoint \"%s\".", TPVCFILE1);
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR|O_NDELAY, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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

		prep("Call t_connect to issue a connect request on the");
		prep("endpoint. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state of the endpoint is T_OUTCON.");
		start
			expecting( T_OUTCON );
			t_getstate( tfd2 );
		finis

		prep("Clear the O_NDELAY flag.");
		start
			expecting( VOID );
			oflags = fcntl( tfd2, F_GETFL, NULL );
			if( (oflags & O_NDELAY ) )
			{
				expecting( SUCCESS );
				fcntl( tfd2, F_SETFL, oflags & ~O_NDELAY );
			}
		finis

		testing("Call t_rcvconnect synchronously.");
		start
			v_sema();
			t_rcvconnect(tfd2, rcvcall);
		finis

		testing("Verify the current state of the endpoint is ");
		testing("T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(ret, T_BIND);
			t_free(req, T_BIND);
			t_free(rcvcall, T_CALL);
			t_free(sndcall, T_CALL);
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





	abstract("Call t_rcvconnect asynchronously and verify the state of");
	abstract("the endpoint before the call is T_OUTCON and after the ");
	abstract("t_rcvconnect call is T_DATAXFER.");
	start
		break_on( ERROR );

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

		p_sema();

		prep("Call t_accept with fd=resfd to establish a ");
		prep("connection on the endpoint.");
		start
			t_accept(tfd1, tfd1, call);
		finis
				
		rendevous();

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
		prep("Open transport endpoint with the O_NDELAY flag set.");
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR|O_NDELAY, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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

		prep("Asynchronously call t_connect to issue a connect");
		prep("request on the endpoint. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state of the endpoint is T_OUTCON.");
		start
			expecting( T_OUTCON );
			t_getstate( tfd2 );
		finis

		rendevous();

		testing("Asynchronously call t_rcvconnect.");
		start
			t_rcvconnect(tfd2, rcvcall);
		finis

		testing("Verify the current state of the endpoint is ");
		testing("T_DATAXFER.");
		start
			continue_on( ERROR );
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(ret, T_BIND);
			t_free(req, T_BIND);
			t_free(rcvcall, T_CALL);
			t_free(sndcall, T_CALL);
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




	abstract("Call t_rcvconnect asynchronously. Call t_rcvconnect");
	abstract("before the connection is established and verify will get");
	abstract("TNODATA. Call t_rcvconnect after the connection is ");
	abstract("established and verify it returns successfully.");
	start
		break_on( ERROR );

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

		p_sema();

		prep("Call t_accept with fd=resfd to establish a ");
		prep("connection on the endpoint.");
		start
			t_accept(tfd1, tfd1, call);
		finis
				
		rendevous();

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
		prep("Open transport endpoint with the O_NDELAY flag set.");
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR|O_NDELAY, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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

		prep("Asynchronously call t_connect to issue a connect");
		prep("request on the endpoint. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state of the endpoint is T_OUTCON.");
		start
			expecting( T_OUTCON );
			t_getstate( tfd2 );
		finis

		testing("Asynchronously call t_rcvconnect. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_rcvconnect(tfd2, rcvcall);
		finis

		rendevous();

		testing("Call t_rcvconnect again, expecting success.");
		start
			t_rcvconnect(tfd2, rcvcall);
		finis

		testing("Verify the current state of the endpoint is ");
		testing("T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(ret, T_BIND);
			t_free(req, T_BIND);
			t_free(rcvcall, T_CALL);
			t_free(sndcall, T_CALL);
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




	abstract("Call t_rcvconnect synchronously with the call argument");
	abstract("set to NULL. Verify the state of the endpoint before the");
	abstract("call is T_OUTCON and after the t_rcvconnect call is");
	abstract("T_DATAXFER.");
	start
		break_on( ERROR );

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

		p_sema();

		prep("Call t_accept with fd=resfd to establish a ");
		prep("connection on the endpoint.");
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
		prep("Open transport endpoint \"%s\".", TPVCFILE1);
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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

		prep("Set the O_NDELAY flag.");
		start
			expecting( VOID );
			oflags = fcntl(tfd2, F_GETFL, 0);
			fcntl(tfd2, F_SETFL, oflags|O_NDELAY);
		finis

		prep("Asynchronously call t_connect to issue a connect ");
		prep("request on the endpoint. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state of the endpoint is T_OUTCON.");
		start
			expecting( T_OUTCON );
			t_getstate( tfd2 );
		finis

		prep("Clear the O_NDELAY flag.");
		start
			expecting( VOID );
			fcntl(tfd2, F_SETFL, oflags);
		finis

		testing("Call t_rcvconnect synchronously.");
		start
			v_sema();
			t_rcvconnect(tfd2, NULL);
		finis

		testing("Verify the current state of the endpoint is ");
		testing("T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(ret, T_BIND);
			t_free(req, T_BIND);
			t_free(rcvcall, T_CALL);
			t_free(sndcall, T_CALL);
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




	abstract("Call t_rcvconnect asynchronously with the call argument");
	abstract("set to NULL. Verify the state of the endpoint before the");
	abstract("call is T_OUTCON and after the t_rcvconnect call is");
	abstract("T_DATAXFER.");
	start
		break_on( ERROR );

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

		p_sema();

		prep("Call t_accept with fd=resfd to establish a ");
		prep("connection on the endpoint.");
		start
			t_accept(tfd1, tfd1, call);
		finis
				
		rendevous();

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
		prep("Open transport endpoint with the O_NDELAY flag set.");
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR|O_NDELAY, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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

		prep("Asynchronously call t_connect to issue a connect");
		prep("request on the endpoint. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		testing("Verify the current state of the endpoint is T_OUTCON.");
		start
			expecting( T_OUTCON );
			t_getstate( tfd2 );
		finis

		rendevous();

		testing("Asynchronously call t_rcvconnect.");
		start
			t_rcvconnect(tfd2, NULL);
		finis

		testing("Verify the current state of the endpoint is ");
		testing("T_DATAXFER.");
		start
			expecting( T_DATAXFER );
			t_getstate( tfd2 );
		finis

		cleanup("Call t_snddis.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(ret, T_BIND);
			t_free(req, T_BIND);
			t_free(rcvcall, T_CALL);
			t_free(sndcall, T_CALL);
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



	abstract("Check that calling t_rcvconnect with a file descriptor");
	abstract("that is not a transport endpoint will cause failure and ");
	abstract("set t_ernno to TBADF.");
	start
		break_on( ERROR );

		prep("Call open(BA_OS) to open \"/etc/passwd\".");
		start
			tfd1 = open("/etc/passwd", O_RDONLY);
		finis

		testing("Call t_rcvconnect with the file descriptor for ");
		testing("\"/etc/passwd\" and the call argument set to NULL.");
		testing("Expecting failure and t_errno set to TBADF.");
		start
			expecting( TBADF );
			t_rcvconnect(tfd1, NULL);
		finis

		cleanup("Call close(BA_OS) to close \"/etc/passwd\".");
		start
			close( tfd1 );
		finis

	finis



	abstract("Verify the TBUFOVFLW error by setting the maxlen field of");
	abstract("the address for call structure to zero. ");
	abstract("Verify the provider remained in the T_OUTCON state ");
	abstract("following the call to t_rcvconnect.");
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

		p_sema();

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
		prep("Open transport endpoint \"%s\".", TPVCFILE1);
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req structure.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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

		prep("Set the O_NDELAY flag.");
		start
			expecting( VOID );
			oflags = fcntl(tfd2, F_GETFL, 0);
			fcntl(tfd2, F_SETFL, oflags|O_NDELAY);
		finis

		prep("Call t_connect asynchronously. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		prep("Set the maximum length for the address in the rcvcall");
		prep("structure to zero.");
		start
			rcvcall->addr.maxlen = 0;
		finis
			
		prep("Clear the O_NDELAY flag.");
		start
			expecting( VOID );
			fcntl(tfd2, F_SETFL, oflags);
		finis

		testing("Verify the provider is in the T_OUTCON state before");
		testing("the call to t_rcvconnect. ");
		start
			expecting( T_OUTCON );
			t_getstate( tfd2 );
		finis

		testing("Call t_rcvconnect synchronously. Expecting failure");
		testing("with t_errno set to TBUFOVFLW.");
		start
			v_sema();
			expecting( TBUFOVFLW );
			t_rcvconnect(tfd2, rcvcall);
		finis

		testing("Verify the provider is in the T_DATAXFER state after");
		testing("the call to t_rcvconnect. ");
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




	abstract("Verify the TNOTSUPPORT error. Open and bind a valid");
	abstract("data gram endpoint. Call t_rcvconnect. Expect failure with");
	abstract("t_errno set to TNOTSUPPORT.");
	start
		break_on( ERROR );

		prep("Open a valid transport endpoint. ");
		start
			tfd1 = t_open(TPDGFILE0, O_RDWR, NULL);
		finis

		prep("Call t_bind with req and ret set to NULL.");
		start
			t_bind(tfd1, NULL, NULL);
		finis

		prep("Allocate the call structure for the t_rcvconnect call.");
		start
			call = (struct t_call *)t_alloc(tfd1, T_CALL, T_ALL);
		finis

		testing("Call t_rcvconnect. Expecting failure");
		testing("with t_errno set to TNOTSUPPORT.");
		start
			expecting( TNOTSUPPORT );
			t_rcvconnect(tfd1, call);
		finis
				
		cleanup("Free the allocated space.");
		start
			expecting( VOID );
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

		p_sema();

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

		prep("Open transport endpoint.");
		start
			tfd2 = t_open(TPVCFILE1, O_RDWR, NULL);
		finis

		prep("Allocate the ret, req, rcvcall and sndcall structures");
		prep("for the t_bind and t_connect calls. Copy a valid ");
		prep("address to the req strucutre.");
		start
			ret = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc ( tfd2, T_BIND, T_ALL );
			rcvcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			sndcall = (struct t_call *)t_alloc (tfd2, T_CALL,T_ALL);
			memcpy( req->addr.buf, TPVCADDR1, TPVCALEN1 );
			req->addr.len = TPVCALEN1;
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


		prep("Set the O_NDELAY flag.");
		start
			expecting( VOID );
			oflags = fcntl(tfd2, F_GETFL, 0);
			fcntl(tfd2, F_SETFL, oflags|O_NDELAY);
		finis

		prep("Call t_connect. Expecting TNODATA.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		prep("Clear the O_NDELAY flag.");
		start
			expecting( VOID );
			fcntl(tfd2, F_SETFL, oflags);
		finis

		testing("Call t_rcvconnect synchronously. Expecting failure");
		testing("with t_errno set to TLOOK.");
		start
			v_sema();
			expecting( TLOOK );
			t_rcvconnect(tfd2, rcvcall);
		finis

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
