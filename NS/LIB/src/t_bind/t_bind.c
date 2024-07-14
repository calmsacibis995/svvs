#ident "@(#)t_bind.c	1.3"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>
#include <string.h>

main()
{
	int	tfd;
	int	*tfd_arr;

	int	i;
	int	j;
	struct	t_info	info;
	struct	t_bind	req;
	struct	t_bind	ret;
	struct	t_bind	*reqptr;
	struct	t_bind	*retptr;
	char	*req_addr;
	char	*ret_addr;
	char	req_vc_addr[BUFSIZ];
	char	req_dg_addr[BUFSIZ];
	char	addr[BUFSIZ];
	char	*ptr;

	char	*malloc();

	setup("t_bind");
	funct("t_bind");
	
	memcpy(req_vc_addr, TPVCADDR0, TPVCALEN0);
	memcpy(req_dg_addr, TPDGADDR0, TPDGALEN0);

	reqptr = &req;
	retptr = &ret;

	prep("Allocate some buffer space.");
	start
		tfd_arr = (int *)malloc((TPVCMINOR0+2)*sizeof(int));
		req_addr= malloc(AMAXLEN);
		ret_addr= malloc(AMAXLEN);
	finis

	abstract("Check that a transport endpoint can be bound to a ");
	abstract("protocol address. Upon successful completion of t_bind, ");
	abstract("the state of the endpoint changes from T_UNBND to T_IDLE.");
	start
		exit_on( ERROR );

	prep("Open transport endpoint \"%s\" for read only", TPVCFILE0);
	start
		tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
	finis

	prep("Check that the current state of the endpoint is T_UNBND");
	start
		expecting( T_UNBND );
		t_getstate( tfd );
	finis

	prep("Set maxlen of the 'ret' structure to the maximum size of ");
	prep("addresses. Initialize the address buffer pointer with a ");
	prep("valid buffer.");
	start
		ret.addr.maxlen = AMAXLEN;
		ret.addr.buf = ret_addr;
	finis

	testing("Call t_bind to bind an address to the transport endpoint.");
	testing("The req argument is set to NULL and ret points to a ");
	testing("t_bind structure.");
	start
		t_bind(tfd, NULL, retptr);
	finis

	testing("Check that the current state of the endpoint is T_IDLE.");
	start
		expecting( T_IDLE );
		t_getstate( tfd );
	finis

	cleanup("Unbind the transport endpoint.");
	start
		t_unbind (tfd);
	finis

	cleanup("Close the transport endpoint.");
	start
		t_close( tfd );
	finis
	finis





	abstract("Check that calling t_bind with a file descriptor that is ");
	abstract("not a transport endpoint will cause failure and set ");
	abstract("t_ernno to TBADF.");
	start
		break_on( ERROR );

		prep("Call open(BA_OS) to open \"/dev/console\".");
		start
			tfd = open("/dev/console", O_WRONLY);
		finis

		testing("Call t_bind with the file descriptor for ");
		testing("\"/dev/console\" and with req and ret both equal ");
		testing("to NULL. Expecting failure and t_errno set to TBADF");
		start
			expecting( TBADF );
			t_bind(tfd, NULL, NULL);
		finis

		cleanup("Call close(BA_OS) to close \"/dev/console\".");
		start
			close( tfd );
		finis
	finis



	abstract("Check that attempting to bind a valid address to ");
	abstract("two different endpoints with qlen greater than zero ");
	abstract("will result in a different address being bound to the ");
	abstract("second endpoint. ");
	start
		break_on( ERROR );

		prep("Open two transport endpoints for read only.");
		start
			tfd_arr[0] = t_open(TPVCFILE0, O_RDONLY, NULL);
			tfd_arr[1] = t_open(TPVCFILE1, O_RDONLY, NULL);
		finis

		prep("Fill the req and ret structures.");
		start
			expecting( VOID );
			req.addr.len = TPVCALEN0;
			req.addr.buf = req_vc_addr;
			ret.addr.maxlen = AMAXLEN;
			ret.addr.buf = ret_addr;
			req.qlen = 1;
		finis

		prep("Call t_bind to bind the first endpoint to a valid address.");
		start
			t_bind(tfd_arr[0], reqptr, retptr);
		finis
		prep("Fill the req and ret structures for the second call to ");
		prep("t_bind. Set the req.qlen field greater than zero. Set ");
		prep("the above call and the addr.len field to the length ");
		prep("of that address. ");
		start
			expecting( VOID );
			req.qlen = 1;
			memcpy(req_addr, ret.addr.buf, ret.addr.len);
			req.addr.buf = req_addr;
			req.addr.len = ret.addr.len;
			ret.addr.buf = ret_addr;
		finis

		testing("Call t_bind to bind the second endpoint to the ");
		testing("address of the first endpoint.");
		start
			t_bind(tfd_arr[1], reqptr, retptr);
		finis

		testing("Check that the bound address is different than ");
		testing("that which was requested. ");
		start
			expecting( VOID );
			if(!memcmp(ret.addr.buf,req.addr.buf,ret.addr.len))
		error("Same address returned with qlen > 0 while doing multiple binds");
		finis

		cleanup("Unbind the two transport endpoints.");
		start
			t_unbind( tfd_arr[0] );
			t_unbind( tfd_arr[1] );
		finis

		cleanup("Close the two transport endpoints.");
		start
			t_close( tfd_arr[0] );
			t_close( tfd_arr[1] );
		finis

	finis


	abstract("While using a transport provider that supports ");
	abstract("connection-mode, check that calling t_bind with req ");
	abstract("and ret alternately equal to NULL, does not cause failure. ");
	start
		exit_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPVCFILE0);
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Fill the req structure with the configuration values.");
		start
			expecting( VOID );
			req.addr.len = TPVCALEN0;
			req.addr.buf = req_vc_addr;
			req.qlen = TPVCMINOR0-1;
		finis

		prep("Fill the ret structure with the configuration values.");
		start
			expecting( VOID );
			ret.addr.maxlen = AMAXLEN;
			ret.addr.buf = ret_addr;
		finis

		testing("Call t_bind with req and ret both pointing to a ");
		testing("t_bind structure.");
		start
			t_bind(tfd, reqptr, retptr);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		testing("Call t_bind with req pointing to a t_bind structure");
		testing("and ret equal to NUll. ");
		start
			t_bind(tfd, reqptr, NULL);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		testing("Call t_bind with req equal to NULL and ret pointing ");
		testing("to a t_bind structure.");
		start
			t_bind(tfd, NULL, retptr);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		testing("Call t_bind with req and ret equal to NULL.");
		start
			t_bind(tfd, NULL, NULL);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis






	abstract("While using a transport provider that supports ");
	abstract("connectionless-mode, check that calling t_bind with req ");
	abstract("and ret alternately equal to NULL, does not cause failure. ");
	start
		exit_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPDGFILE0);
		start
			tfd = t_open(TPDGFILE0, O_RDONLY, NULL);
		finis

		prep("Fill the req structure with the configuration values.");
		start
			expecting( VOID );
			req.addr.len = TPDGALEN0;
			req.addr.buf = req_dg_addr;
			req.qlen = TPDGMINOR0-1;
		finis

		prep("Fill the ret structure with the configuration values.");
		start
			expecting( VOID );
			ret.addr.maxlen = AMAXLEN;
			ret.addr.buf = ret_addr;
		finis

		testing("Call t_bind with req and ret both pointing to a ");
		testing("t_bind structure.");
		start
			t_bind(tfd, reqptr, retptr);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		testing("Call t_bind with req pointing to a t_bind structure");
		testing("and ret equal to NUll. ");
		start
			t_bind(tfd, reqptr, NULL);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		testing("Call t_bind with req equal to NULL and ret pointing ");
		testing("to a t_bind structure.");
		start
			t_bind(tfd, NULL, retptr);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		testing("Call t_bind with req and ret equal to NULL.");
		start
			t_bind(tfd, NULL, NULL);

			cleanup("Call t_unbind to unbind the endpoint.");
			start
				t_unbind( tfd );
			finis
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis






	abstract("While using a transport provider that supports ");
	abstract("connection-mode, call t_bind with the maxlen element ");
	abstract("of the ret structure equal to zero. Expecting ...");
	abstract(" -  failure with t_errno set to TBUFOVFLW ");
	abstract(" -  the state to change from T_UNBND to T_IDLE");
	abstract(" -  the count of the outstanding connect indications to be ");
	abstract("    set to zero. ");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPVCFILE0);
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Fill the ret structure with the configuration values.");
		start
			expecting( VOID );
			ret.addr.maxlen = 0;
			ret.addr.buf = ret_addr;
		finis

		testing("Call t_bind with req equal to NULL and ret pointing ");
		testing("to a t_bind structure with the maxlen field equal ");
		testing("to zero. Expecting failure with t_errno set to ");
		testing("TBUFOVFLW.");
		start
			expecting( TBUFOVFLW );
			t_bind(tfd, NULL, retptr);
		finis

		testing("Check that the state changed from T_UNBND to T_IDLE");
		start
			expecting( T_IDLE );
			t_getstate( tfd );
		finis

		testing("Check that the count of outstanding connect indications");
		testing("is zero.");
		start
			if(ret.qlen)
				error("The count of outstanding connect indications is %d.", ret.qlen);
		finis

		cleanup("Call t_unbind to unbind the endpoint.");
		start
			t_unbind( tfd );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis





	abstract("Verify that t_bind returns an address of the bound ");
	abstract("endpoint.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPVCFILE0);
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Fill the ret structure with the configuration values.");
		start
			expecting( VOID );
			ret.addr.maxlen = AMAXLEN;
			ret.addr.buf = ret_addr;
		finis

		testing("Call t_bind with req equal to NULL and ret pointing ");
		testing("to a t_bind structure. ");
		start
			t_bind(tfd, NULL, retptr);
		finis

		testing("Check that t_bind returned an address in the ");
		testing("designated address buffer.");
		start
			continue_on( ERROR );
			if(ret.addr.len == 0)
		   	      error("An address was not returned from t_bind.");
		finis
		cleanup("Call t_unbind to unbind the endpoint.");
		start
			t_unbind( tfd );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis




	abstract("Verify that t_bind successfully returns if the len field ");
	abstract("of the req structure is equal to zero but the address ");
	abstract("field contains a valid address. ");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPVCFILE0);
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Fill the req structure with the configuration values.");
		start
			expecting( VOID );
			req.addr.len = 0;
			req.qlen = TPVCMINOR0;
			req.addr.buf = req_vc_addr;
		finis

		prep("Fill the ret structure with the configuration values.");
		start
			expecting( VOID );
			ret.addr.maxlen = AMAXLEN;
			ret.addr.buf = ret_addr;
		finis

		testing("Call t_bind with req pointing to a t_bind structure");
		testing("and ret equal to NUll. ");
		start
			t_bind(tfd, reqptr, NULL);
		finis


		cleanup("Call t_unbind to unbind the endpoint.");
		start
			t_unbind( tfd );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis




	abstract("Check that binding an endpoint to an invalid address ");
	abstract("will cause failure and set t_ernno to TBADADDR.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPVCFILE0);
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Fill the req structure with the configuration values.");
		start
			expecting( VOID );
			memcpy(req_vc_addr, TPBADADDR, TPBADLEN);
			req.addr.len = TPBADLEN;
			req.addr.buf = req_vc_addr;
			req.qlen = 0;
		finis

		testing("Call t_bind with req pointing to a t_bind structure");
		testing("and ret equal to NULL. Expecting failure with ");
		testing("t_errno set to TBADADDR.");
		start
			expecting( TBADADDR );
			t_bind(tfd, reqptr, NULL);
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis









	abstract("Check that binding an endpoint twice will cause failure");
	abstract("and set t_ernno to TOUTSTATE.");
	start
		break_on( ERROR );

		prep("Open transport endpoint \"%s\" for read only.", TPVCFILE0);
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Call t_bind with req and ret both equal to NULL.");
		start
			t_bind(tfd, NULL, NULL);
		finis

		testing("Call t_bind with req and ret both equal to NULL.");
		testing("Expecting failure and t_errno set to TOUTSTATE.");
		start
			expecting( TOUTSTATE );
			t_bind(tfd, NULL, NULL);
		finis

		cleanup("Unbind the transport endpoint.");
		start
			t_unbind( tfd );
		finis

		cleanup("Close the transport endpoint.");
		start
			t_close( tfd );
		finis
	finis

	cleanup("Free the allocated buffer space.");
	start
		free(tfd_arr);
		free(req_addr);
		free(ret_addr);
	finis


}



