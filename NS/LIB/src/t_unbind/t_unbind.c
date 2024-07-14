#ident "@(#)t_unbind.c	1.2"
#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

main()
{
	int	tfd;
	struct	t_bind	req;
	struct	t_bind	ret;
	char	*rq_addr;
	char	*rt_addr;
	char *malloc();

	setup("t_unbind");
	funct("t_unbind");

	prep("Allocate some buffer space");
	start
		rq_addr = malloc(AMAXLEN);
		rt_addr = malloc(AMAXLEN);
	finis

	abstract("Check that t_unbind returns successfully by disabling ");
	abstract("a valid transport endpoint. ");
	start 
		exit_on( ERROR );

		prep("Call t_open to open a transport endpoint. ");
		start 
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis
	
		prep("Bind the transport endpoint to get it in the proper ");
		prep("state. Copy the address to be bound into the addr buffer.");
		start
			req.addr.len = TPVCALEN0;
			req.addr.buf = rq_addr;
			memcpy( rq_addr, TPVCADDR0, TPVCALEN0 );
			req.qlen = 1;
			ret.addr.maxlen = TPVCALEN0;
			ret.addr.buf = rt_addr;
		finis
	
		prep("Call t_bind to bind the endpoint.");
		start
			t_bind(tfd,&req,&ret);
		finis
	
		testing("Call t_unbind to unbind the valid endpoint.");
		start
			t_unbind(tfd);
		finis

		testing("Call t_getstate to verify the state of the ");
		testing("endpoint is T_UNBND.");
		start
			expecting(T_UNBND);
			t_getstate(tfd);
		finis
	
		cleanup("Close the endpoint.");
		start
			t_close (tfd );
		finis
	finis
	
	


	abstract("Verify the TBADF t_errno by opening an endpoint, ");
	abstract("closing it and then calling t_unbind to unbind the ");
	abstract("closed endpoint. ");
	start
		prep("Use t_open to open a transport endpoint.");
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis

		prep("Use t_close to close the same endpoint. ");
		start
			t_close( tfd );
		finis

		testing("Call t_unbind to unbind the closed endpoiunt. ");
		testing("Expecting failure with t_errno equal to TBADF. ");
		start
			expecting (TBADF);
			t_unbind( tfd );
		finis
	finis


	abstract("Verify the TOUTSTATE t_errno value by opening a valid ");
	abstract("transport endpoint and using t_unbind to unbind it. ");
	start
		prep("Call t_open to open a valid transport endpoint. ");
		start
			tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
		finis


		testing("Call t_unbind to unbind the valid endpoint. The ");
		testing("endpoint is currently in the T_UNBND state, ");
		testing("therefore, causing the call to t_unbind to fail with");
		testing("t_errno set to TOUTSTATE. ");
		start
			expecting( TOUTSTATE );
			t_unbind (tfd );
		finis

		cleanup("Close the endpoint.");
		start
			t_close( tfd );
		finis
	finis

	cleanup("Free the allocated buffer space.");
	start
		free(rq_addr);
		free(rt_addr);
	finis

	done();
}
