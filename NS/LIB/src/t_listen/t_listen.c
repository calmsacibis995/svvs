#ident  "@(#)t_listen.c	1.2"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

#define		rendevous()		(	v_sema(), p_sema() )
#define		WARNMSG1       "The number of connect indications supported"
#define		WARNMSG2       "by the transport provider does not exceed one. "

char *t_alloc();
int  t_free();

main()
{


	struct	t_call	*call;
	struct	t_call	*call2;
	struct	t_call	*sndcall;
	struct	t_call	*rcvcall;
	struct	t_call	*sndcall2;
	struct	t_call	*rcvcall2;
	struct	t_bind	*ret;
	struct	t_bind	*req;
	struct	t_discon *discon;


	int	tfd1;
	int	tfd2;

	setup("t_listen");
	funct("t_listen");



	abstract("Verify that t_listen successfully returns and that a");
	abstract("connection can be rejected. Open and bind a valid");
	abstract("transport provider. Fork another process. The parent calls");
	abstract("t_listen and the child calls t_connect. The parent then ");
	abstract("calls t_snddis followed by t_unbind and t_close.");
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


		testing("Issue a t_snddis to refuse the connect request.");
		start
			t_snddis(tfd1, call);
		finis
				
		rendevous();
			
		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(req, T_BIND);
			t_free(ret, T_BIND);
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

		prep("Call t_connect. Expecting failure with t_errno set to");
		prep("TLOOK. This will occur because the passive user will ");
		prep("refuse the connection causing an asynchronous event ");
		prep("to occur.");
		start
			expecting( TLOOK );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		p_sema();

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(req, T_BIND);
			t_free(ret, T_BIND);
			t_free(sndcall, T_CALL);
			t_free(rcvcall, T_CALL);
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




	abstract("Verify the state of the transport provider following a");
	abstract("t_listen call is T_INCON. If the provider can support more");
	abstract("than one connect indication, verify that calling t_listen");
	abstract("from a state of T_INCON, the provider remains in T_INCON ");
	abstract("state.");
	start
		abort_on( ERROR );

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
			call2 = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL );
			discon = (struct t_discon *)t_alloc(tfd1, T_DIS, T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			req->qlen = 1;
		finis

		prep("Bind the transport endpoint to the specified address.");
		start
			t_bind(tfd1, req, ret);
		finis

		prep("Verify the current state of the provider is T_IDLE.");
		start
			expecting( T_IDLE );
			t_getstate( tfd1 );
		finis

		if( ret->qlen > 1)
		{
			if(fork())		/* parent */
			{
				prep("Call t_listen.");
				start
					v_sema();
					t_listen(tfd1, call);
				finis

				prep("Call t_accept to accept the connect ");
				prep("request.");
				start
					t_accept(tfd1, tfd1, call);
				finis

				testing("Verify the current state of the ");
				testing("provider is T_INCON.");
				start
					expecting( T_INCON );
					t_getstate( tfd1 );
				finis

				rendevous();

				testing("Call t_listen for a second time.");
				start
					v_sema();
					t_listen(tfd1, call2);
				finis

				testing("Verify the current state of the ");
				testing("provider is T_INCON.");
				start
					expecting( T_INCON );
					t_getstate( tfd1 );
				finis

				prep("Issue a t_snddis to refuse the second ");
				prep("connect request.");
				start
					t_snddis(tfd1, call2);
				finis
						
				prep("Issue a t_rcvdis to disconnect the first");
				prep("connect.");
				start
					t_rcvdis(tfd1, discon);
				finis
						
				rendevous();
					
			cleanup("Free the allocated space.");
			start
				expecting( VOID );
				t_free(ret, T_BIND);
				t_free(req ,T_BIND );
				t_free(call, T_CALL );
				t_free(call2, T_CALL );
				t_free(discon, T_DIS);
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

				prep("Allocate the ret, req, rcvcall and ");
				prep("sndcall structures for the t_bind and");
				prep("t_connect calls. Copy a valid ");
				prep("address to the req structure.");
				start
					ret = (struct t_bind *)t_alloc ( tfd2,
							T_BIND, T_ALL );
					req = (struct t_bind *)t_alloc ( tfd2,
							T_BIND, T_ALL );
					rcvcall2 = (struct t_call *)t_alloc (
							tfd2, T_CALL,T_ALL);
					sndcall2 = (struct t_call *)t_alloc (
							tfd2, T_CALL,T_ALL);
					rcvcall = (struct t_call *)t_alloc (
							tfd2, T_CALL,T_ALL);
					sndcall = (struct t_call *)t_alloc (
							tfd2, T_CALL,T_ALL);
					memcpy( req->addr.buf, TPVCADDR1,
							TPVCALEN1 );
					req->addr.len = TPVCALEN1;
				finis

				prep("Bind the transport endpoint.");
				start
					t_bind(tfd2, req, ret);
				finis

				prep("Fill the sndcall structure.");
				start
					memcpy( sndcall->addr.buf, TPVCCONN0,
							TPVCCLEN0 );
					sndcall->addr.len = TPVCCLEN0;
				finis

				prep("Call t_connect.");
				start
					t_connect(tfd2, sndcall2, rcvcall2);
				finis

				prep("Call t_rcvconnect to receive the connect");
				prep("on the first endpoint.");
				start
					t_rcvconnect(tfd2, NULL);
				finis

				p_sema();

				prep("Call t_connect. Expecting failure with");
				prep("t_errno set to TLOOK. This will occur");
				prep("because the passive user will refuse");
				prep("the connection causing an asynchronous");
				prep("event to occur. ");
				start
					expecting( TLOOK );
					t_connect(tfd2, sndcall, rcvcall);
				finis

				p_sema();

				cleanup("Free the allocated space.");
				start
					expecting( VOID );
					t_free(ret, T_BIND);
					t_free(req ,T_BIND );
					t_free(sndcall, T_CALL );
					t_free(sndcall2, T_CALL );
					t_free(rcvcall, T_CALL );
					t_free(rcvcall2, T_CALL );
				finis

				cleanup("Call t_rcvdis to clear the TLOOK error.");
				start
					t_rcvdis(tfd2, NULL);
				finis

				cleanup("Call t_snddis to disconnect the first");
				cleanup("connection.");
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
		}
		else
		{
			if(fork())
			{		/* parent */
				prep("Call t_listen.");
				start
					v_sema();
					t_listen(tfd1, call);
				finis


				testing("Verify the state of the provider is");
				testing("T_INCON.");
				start
					expecting( T_INCON );
					t_getstate( tfd1 );
				finis

				prep("Issue a t_snddis to refuse the connect");
				prep("request.");
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

				prep("Open transport endpoint \"%s\".",
							TPVCFILE1);
				start
					tfd2 = t_open(TPVCFILE1, O_RDWR, NULL);
				finis

				prep("Allocate the ret, req, rcvcall and");
				prep("sndcall structures for the t_bind and");
				prep("t_connect calls. Copy a valid address");
				prep("to the req structure.");
				start
					ret = (struct t_bind *)t_alloc ( tfd2,
							T_BIND, T_ALL );
					req = (struct t_bind *)t_alloc ( tfd2,
							T_BIND, T_ALL );
					rcvcall = (struct t_call *)t_alloc (tfd2,
							T_CALL,T_ALL);
					sndcall = (struct t_call *)t_alloc (tfd2,
							T_CALL,T_ALL);
					memcpy( req->addr.buf, TPVCADDR1,
							TPVCALEN1 );
					req->addr.len = TPVCALEN1;
				finis

				prep("Bind the transport endpoint.");
				start
					t_bind(tfd2, req, ret);
				finis

				prep("Fill the sndcall structure.");
				start
					memcpy( sndcall->addr.buf, TPVCCONN0,
							TPVCCLEN0 );
					sndcall->addr.len = TPVCCLEN0;
				finis

				prep("Call t_connect. Expecting failure with");
				prep("t_errno set to TLOOK. This will occur");
				prep("because the passive user will refuse the");
				prep("connection causing an asynchronous");
				prep("event to occur. ");
				start
					expecting( TLOOK );
					t_connect(tfd2, sndcall, rcvcall);
				finis

				p_sema();

				cleanup("Free the allocated space.");
				start
					expecting( VOID );
					t_free(ret, T_BIND);
					t_free(req ,T_BIND );
					t_free(sndcall, T_CALL );
					t_free(rcvcall, T_CALL );
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
		}
	finis





	abstract("Check that calling t_listen with a file descriptor of a");
	abstract("transport endpoint that does not support connection mode,");
	abstract("failure will occur and t_errno will be set to TNOTSUPPORT.");
	start
		break_on( ERROR );

		prep("Open datagram transport provider \"%s\".", TPDGFILE0);
		start
			tfd1 = t_open(TPDGFILE0, O_RDWR, NULL);
		finis

		prep("Bind the transport provider.");
		start
			t_bind(tfd1, NULL, NULL);
		finis

		prep("Allocate the t_call structure for the t_listen call.");
		start
			call = (struct t_call *)t_alloc(tfd1, T_CALL, T_ALL);
		finis

		testing("Call t_listen. Expecting failure with t_errno set");
		testing("to TNOTSUPPORT.");
		start
			expecting( TNOTSUPPORT );
			t_listen(tfd1, call);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(call, T_CALL);
		finis

		cleanup("Unbind the transport provider.");
		start
			t_unbind( tfd1 );
		finis

		cleanup("Close the transport provider.");
		start
			t_close( tfd1 );
		finis
	finis




	abstract("Check that calling t_listen with O_NDELAY set and no");
	abstract("connect indications queued will cause failure with ");
	abstract("t_errno set to TNODATA.");
	start
		break_on( ERROR );

		prep("Open transport provider \"%s\" for read only.", TPVCFILE0);
		start
			tfd1 = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Bind the transport provider.");
		start
			t_bind(tfd1, NULL, NULL);
		finis

		prep("Allocate the call structure for the t_listen call.");
		start
			call = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL);
		finis

		prep("Execute fcntl(fildes, F_SETFL, O_NDELAY), where");
		prep("fildes is the file descriptor for the bound transport");
		prep("provider.");
		start
			fcntl(tfd1, F_SETFL, O_NDELAY);
		finis


		testing("Call t_listen while the transport provider is in");
		testing("asynchronous mode and there are no outstanding ");
		testing("connect requests. Expecting failure with t_errno");
		testing("set to TNODATA.");
		start
			expecting( TNODATA );
			t_listen(tfd1, call);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(call, T_CALL);
		finis

		cleanup("Unbind the transport provider.");
		start
			t_unbind( tfd1 );
		finis

		cleanup("Close the transport provider.");
		start
			t_close( tfd1 );
		finis
	finis



	abstract("Check that calling t_listen with a file descriptor that is ");
	abstract("not a transport endpoint will cause failure and set ");
	abstract("t_ernno to TBADF.");
	start
		break_on( ERROR );

		prep("Open a valid transport provider to get the protocol");
		prep("specific information in order to fill the call ");
		prep("structure. This is done so that t_listen does not fail");
		prep("because of invalid pointer.");
		start
			tfd2 = t_open(TPVCFILE1, O_RDONLY, NULL);
		finis

		prep("Allocate the call structure for the t_listen call.");
		start
			call = (struct t_call *)t_alloc(tfd2, T_CALL, T_ALL);
		finis

		prep("Call open(BA_OS) to open \"/etc/passwd\".");
		start
			tfd1 = open("/etc/passwd", O_RDONLY);
		finis

		testing("Call t_listen with the file descriptor for ");
		testing("\"/etc/passwd\". Expecting failure and t_errno");
		testing("set to TBADF.");
		start
			expecting( TBADF );
			t_listen(tfd1, call);
		finis

		cleanup("Call close(BA_OS) to close \"/etc/passwd\".");
		start
			close( tfd1 );
		finis

		cleanup("CLose the transport provider.");
		start
			t_close( tfd2 );
		finis
	finis





	abstract("Call t_listen with the maximum buffer size for the");
	abstract("returned address set to zero. Check that failure will");
	abstract("occur and t_errno will be set to TBUFOVFLW.");
	start
		break_on( ERROR );    /* TRACE change to abort_on */
	if(fork())    /* parent */
	{

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd1 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the req, ret and call structures for the");
		prep("t_bind and t_listen call. Copy a valid address to the");
		prep("req structure and set the qlen to 1. Set the maxlen");
		prep("field of the call structure to zero");
		start
			ret = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			req = (struct t_bind *)t_alloc( tfd1, T_BIND, T_ALL );
			call = (struct t_call *)t_alloc( tfd1, T_CALL, T_ALL );
			discon = (struct t_discon *)t_alloc(tfd1, T_DIS, T_ALL);
			memcpy( req->addr.buf, TPVCADDR0, TPVCALEN0 );
			req->addr.len = TPVCALEN0;
			call->addr.maxlen = 0;
			req->qlen = 1;
		finis

		prep("Bind the transport endpoint.");
		start
			t_bind(tfd1, req, ret);
		finis

		testing("Call t_listen. Expecting failure with t_errno set to");
		testing("TBUFOVFLW.");
		start
			v_sema();
			expecting( TBUFOVFLW );
			t_listen(tfd1, call);
		finis

		p_sema();
			
		cleanup("Call t_rcvdis to receive the disconnect request.");
		start
			t_rcvdis(tfd1, discon);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(req, T_BIND);
			t_free(ret, T_BIND);
			t_free(call, T_CALL);
			t_free(discon, T_DIS);
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

		prep("Asynchronously open transport endpoint \"%s\".", TPVCFILE1);
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

		prep("Call t_connect. Expecting failure with t_errno set to");
		prep("TNODATA because the provider is in asynchronous mode.");
		start
			expecting( TNODATA );
			t_connect(tfd2, sndcall, rcvcall);
		finis

		cleanup("Call t_snddis to get the provider back to the");
		cleanup("T_IDLE state.");
		start
			t_snddis(tfd2, NULL);
		finis

		cleanup("Free the allocated space.");
		start
			expecting( VOID );
			t_free(req, T_BIND);
			t_free(ret, T_BIND);
			t_free(sndcall, T_CALL);
			t_free(rcvcall, T_CALL);
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


	abstract("Verify the TLOOK error. The active user will call");
	abstract("t_connect asynchronously followed by a t_rcvconnect and ");
	abstract("a t_snddis. When the passive user calls t_listen, it ");
	abstract("should fail with t_errno set to TLOOK. Call t_rcvdis to");
	abstract("receive the disconnect request and clear the TLOOK error.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\".", TPVCFILE0);
		start
			tfd1 = t_open(TPVCFILE0, O_RDWR, NULL);
		finis

		prep("Allocate the req, ret, discon and call structures for ");
		prep("the t_bind, t_rcvdis and t_listen calls. Copy a valid");
		prep("address to the req structure and set the qlen to 1.");
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

		abort_on( ERROR );
		if(fork())
		{
			prep("Call t_listen. ");
			start
				v_sema();
				t_listen(tfd1, call);
			finis

			testing("Call t_listen again. Expecting failure with");
			testing("t_errno set to TLOOK.");
			start
				v_sema();
				expecting( TLOOK );
				t_listen(tfd1, call);
			finis

			cleanup("Issue a t_rcvdis to disconnect the first");
			cleanup("connection.");
			start
				t_rcvdis(tfd1, discon);
			finis

			rendevous();

			cleanup("Free the allocated space.");
			start
				expecting( VOID );
				t_free(req, T_BIND);
				t_free(ret, T_BIND);
				t_free(call, T_CALL);
				t_free(discon, T_DIS);
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

			prep("Asynchronously open a transport enpoint.");
			start
				tfd2 = t_open(TPVCFILE1, O_RDWR|O_NDELAY, NULL);
			finis

			prep("Allocate the ret, req, rcvcall and ");
			prep("sndcall structures for the t_bind and");
			prep("t_connect calls. Copy a valid ");
			prep("address to the req structure.");
			start
				ret = (struct t_bind *)t_alloc ( tfd2,
						T_BIND, T_ALL );
				req = (struct t_bind *)t_alloc ( tfd2,
						T_BIND, T_ALL );
				rcvcall = (struct t_call *)t_alloc (
						tfd2, T_CALL,T_ALL);
				sndcall = (struct t_call *)t_alloc (
						tfd2, T_CALL,T_ALL);
				memcpy( req->addr.buf, TPVCADDR1,
						TPVCALEN1 );
				req->addr.len = TPVCALEN1;
			finis

			prep("Bind the transport endpoint.");
			start
				t_bind(tfd2, req, ret);
			finis

			prep("Fill the sndcall structure.");
			start
				memcpy( sndcall->addr.buf, TPVCCONN0,
						TPVCCLEN0 );
				sndcall->addr.len = TPVCCLEN0;
			finis

			prep("Call t_connect asynchronously. Expecting");
			prep("t_errno to be set to TNODATA.");
			start
				expecting( TNODATA );
				t_connect(tfd2, sndcall, rcvcall);
			finis

			p_sema();

			prep("Call t_rcvconnect to put the endpoint in the");
			prep("T_DATAXFER state. Expecting failure with ");
			prep("t_errno set to TNODATA.");
			start
				expecting( TNODATA );
				t_rcvconnect(tfd2, NULL);
			finis

			prep("Call t_snddis to put the endpoint in the");
			prep("T_IDLE state and to generate an asynchronous");
			prep("event.");
			start
				t_snddis(tfd2, NULL);
			finis

			p_sema();

			cleanup("Free the allocated space.");
			start
				expecting( VOID );
				t_free(req, T_BIND);
				t_free(ret, T_BIND);
				t_free(sndcall, T_CALL);
				t_free(rcvcall, T_CALL);
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
