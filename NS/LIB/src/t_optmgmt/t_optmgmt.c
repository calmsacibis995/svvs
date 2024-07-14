#ident "@(#)t_optmgmt.c	1.2"
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
	int	tfd2;
	int	i;
	struct	t_optmgmt 	req;
	struct	t_optmgmt 	ret;
	char	*req_opt_buf;
	char	*ret_opt_buf;
	char	*valid_options;
	char	*bad_options;

	unsigned int		valid_opt_len;

	char *malloc();

	setup("t_optmgmt");
	funct("t_optmgmt");

	prep("Allocate some buffer space.");
	start
		req_opt_buf = malloc(OMAXLEN);
		ret_opt_buf = malloc(OMAXLEN);
		valid_options = malloc(OMAXLEN);
		bad_options = malloc(OMAXLEN);
	finis

	memcpy(bad_options, BAD_OPTIONS, BADOPTLEN);

	abstract("Call t_optmgmt with the flag of T_DEFAULT in order to");
	abstract("get the valid options for the transport protocol. These");
	abstract("options will be used in subsequent calls to t_optmgmt to ");
	abstract("check the T_NEGOTIATE and T_CHECK flags. ");
	start
		exit_on( ERROR );
	prep("Open the transport endpoint \"%s\" for read only", TPVCFILE0);
	start
		tfd = t_open(TPVCFILE0, O_RDONLY, NULL);
	finis

	prep("Bind the transport endpoint, with req and ret equal to NULL.");
	start
		t_bind(tfd, NULL, NULL);
	finis
	
	prep("Fill the req and ret t_optmgmt structures. In the req ");
	prep("structure, set the len field to zero, the buf field to NULL ");
	prep("and the flag field to T_DEFAULT. In the ret structure, set ");
	prep("the maxlen field to the maximum len of the return buffer and");
	prep("the buf field to the address of the return buffer. ");
	start
		expecting( VOID );
		req.opt.len = 0;
		req.opt.buf = NULL;
		req.flags = T_DEFAULT;
		ret.opt.maxlen = OMAXLEN;
		ret.opt.buf = valid_options;
	finis

	testing("Call t_optmgmt to get the valid options for the endpoint.");
	start
		t_optmgmt(tfd, &req, &ret);
		valid_opt_len = ret.opt.len;
	finis
	finis




	abstract("Call t_optmgmt to negotiate the options for the provider.");
	abstract("Pass the valid options to the provider and check that it ");
	abstract("successfully returns. ");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In req, set ");
		prep("flags to T_NEGOTIATE, buf to the address of the");
		prep("valid options and len to the length of the valid");
		prep("options. In ret, set buf to the address of a working");
		prep("buffer and maxlen to the maximum length of the options.");
		start
			expecting( VOID );
			req.opt.len = valid_opt_len;
			req.opt.buf = valid_options;
			req.flags = T_NEGOTIATE;
			ret.opt.maxlen = OMAXLEN;
			ret.opt.buf = ret_opt_buf;
		finis

		testing("Call t_optmgmt to negotiate the options.");
		start
			t_optmgmt(tfd, &req, &ret);
		finis
	finis
			




	abstract("Verify the T_CHECK flag works successfully. Call t_optmgmt");
	abstract("with valid options and expect to get T_SUCCESS returned.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In req, set ");
		prep("flags to T_CHECK, buf to the address of the");
		prep("valid options and len to the length of the valid");
		prep("options. In ret, set buf to the address of a working");
		prep("buffer and maxlen to the maximum length of the options.");
		start
			expecting( VOID );
			req.opt.len = valid_opt_len;
			req.opt.buf = valid_options;
			req.flags = T_CHECK;
			ret.opt.maxlen = OMAXLEN;
			ret.opt.buf = ret_opt_buf;
		finis

		testing("Call t_optmgmt to verify the options are ");
		testing("supported by the transport provider.");
		start
			t_optmgmt(tfd, &req, &ret);
		finis

		testing("Check the flags field of the ret structure to ");
		testing("verify it is T_SUCCESS. ");
		start
			expecting( T_SUCCESS );
			i = ret.flags;
		finis
	finis
			




	abstract("Verify the T_CHECK flag works successfully. Call t_optmgmt");
	abstract("with invalid options and expect to get T_FAILURE returned.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In req, set ");
		prep("flags to T_CHECK, buf to the address of the ");
		prep("buffer containing the invalid options and len to the ");
		prep("length of the buffer. In ret, set buf to the address");
		prep("of a working buffer and maxlen to the maximum length");
		prep("of options.");
		start
			expecting( VOID );
			req.opt.len = BADOPTLEN;
			req.opt.buf = bad_options;
			req.flags = T_CHECK;
			ret.opt.maxlen = OMAXLEN;
			ret.opt.buf = ret_opt_buf;
		finis

		testing("Call t_optmgmt to verify the options are not ");
		testing("supported by the transport provider.");
		testing("Expecting failure with t_errno set to TBADOPT.");
		start
			expecting( TBADOPT );
			t_optmgmt(tfd, &req, &ret);
		finis

		testing("Check the flags field of the ret structure to ");
		testing("verify it is T_FAILURE. ");
		start
			expecting( T_FAILURE );
			i = ret.flags;
		finis
	finis
			



	abstract("Verify  the TBUFOVFLW error value by calling t_optmgmt");
	abstract("with flags element of the req structure set to T_DEFAULT ");
	abstract("and maxlen of the ret structure set to zero.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In the req ");
		prep("structure, set the len field to zero, the buf field to ");
		prep("NULL and the flag field to T_DEFAULT. In the ret struct,");
		prep("set the maxlen field to zero and the buf field to the ");
		prep("address of a working buffer.");
		start
			expecting( VOID );
			req.opt.len = 0;
			req.opt.buf = NULL;
			req.flags = T_DEFAULT;
			ret.opt.maxlen = 0;
			ret.opt.buf = ret_opt_buf;
		finis

		testing("Call t_optmgmt. Expecting failure with t_errno set ");
		testing("to TBUFOFLW.");
		start
			expecting( TBUFOVFLW );
			t_optmgmt(tfd, &req, &ret);
		finis
	finis




	abstract("Verify the TBADFLAG error by or-ing the three flags ");
	abstract("together and passing them to the transport provider.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In req, set ");
		prep("flags to T_NEGOTIATE|T_CHECK|T_DEFAULT, buf to");
		prep("the address of the valid options and len to the");
		prep("length of the valid options. In ret, set buf to the");
		prep("address of a working buffer and maxlen to the maximum");
		prep("length of the options.");
		start
			expecting( VOID );
			req.opt.len = valid_opt_len;
			req.opt.buf = valid_options;
			req.flags = T_NEGOTIATE|T_CHECK|T_DEFAULT;
			ret.opt.maxlen = OMAXLEN;
			ret.opt.buf = ret_opt_buf;
		finis

		testing("Call t_optmgmt. Expecting failure with t_errno set");
		testing("to TBADFLAG.");
		start
			expecting( TBADFLAG );
			t_optmgmt(tfd, &req, &ret);
		finis
	finis
			



	abstract("Verify the TBADOPT error by calling the t_optmgmt with");
	abstract("the flag set to T_NEGOTIATE and the buf element of req ");
	abstract("pointing to invalid options.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In req, set ");
		prep("flags to T_NEGOTIATE, buf to the address of the");
		prep("buffer containing the invalid options and len to the ");
		prep("length of the buffer. In ret, set buf to the address");
		prep("of a working buffer and maxlen to the maximum length");
		prep("of options.");
		start
			expecting( VOID );
			req.opt.len = BADOPTLEN;
			req.opt.buf = bad_options;
			req.flags = T_NEGOTIATE;
			ret.opt.maxlen = OMAXLEN;
			ret.opt.buf = ret_opt_buf;
		finis

		testing("Call t_optmgmt to verify the options are invalid.");
		start
			expecting( TBADOPT );
			t_optmgmt(tfd, &req, &ret);
		finis

	finis
			


	abstract("Verify the TOUTSTATE error by unbinding the transport");
	abstract("provider and calling t_optmgmt.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In the req ");
		prep("structure, set the len field to zero, the buf field to ");
		prep("NULL and the flag field to T_DEFAULT. In the ret struct,");
		prep("set the maxlen field to zero and the buf field to the ");
		prep("address of a working buffer.");
		start
			expecting( VOID );
			req.opt.len = 0;
			req.opt.buf = NULL;
			req.flags = T_DEFAULT;
			ret.opt.maxlen = 0;
			ret.opt.buf = ret_opt_buf;
		finis


		prep("Call t_unbind to unbind the transport provider.");
		start
			t_unbind( tfd );
		finis

		testing("Call t_optmgmt. Expecting failure with t_errno set");
		testing("to TOUTSTATE.");
		start
			expecting( TOUTSTATE );
			t_optmgmt(tfd, &req, &ret);
		finis
	finis

        abstract("Verify the TBADF t_errno by opening the /dev/console file");
	abstract("and passing the file descriptor to t_optmgmt. Use the ");
	abstract("T_DEFAULT flag to prevent the call from failing with ");
	abstract("t_errno set to TBADOPT instead.");
	start
		break_on( ERROR );
		prep("Fill the req and ret t_optmgmt structures. In the req ");
		prep("structure, set the len field to zero, the buf field to ");
		prep("NULL and the flag field to T_DEFAULT. In the ret struct,");
		prep("set the maxlen field to zero and the buf field to the ");
		prep("address of a working buffer.");
		start
			expecting( VOID );
			req.opt.len = 0;
			req.opt.buf = NULL;
			req.flags = T_DEFAULT;
			ret.opt.maxlen = 0;
			ret.opt.buf = ret_opt_buf;
		finis

		prep("Call open(BA_OS) to open /dev/console.");
		start
			tfd2 = open("/dev/console", O_WRONLY);
		finis

		testing("Call t_optmgmt. Expecting failure with t_errno set");
		testing("to TBADF.");
		start
			expecting( TBADF );
			t_optmgmt(tfd2, &req, &ret);
		finis
	finis

	abstract("Free the allocated buffer space and close the endpoints.");
	start
		prep("Free the allocated space.");
		start
			free(req_opt_buf);
			free(ret_opt_buf);
			free(valid_options);
			free(bad_options);
		finis


		prep("Call close(BA_OS) to close /dev/console.");
		start
			close( tfd2 );
		finis

		prep("Call t_close to close the endpoint.");
		start
			t_close(tfd);
		finis
	finis

	done();
}
	


