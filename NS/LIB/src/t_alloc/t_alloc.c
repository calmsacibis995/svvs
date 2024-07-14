/*
 *	t_alloc.c
 *
 *	a test of t_alloc(NS_LIB)
 *
 *	tabstops should be set to 4 to view this file
 *
 */

#ident	"@(#)t_alloc.c	1.3"

#define max(a,b)	((a>b)?a:b)

#include <sv_base.h>
#include <sv_nse.h>
#include <stdio.h>	
#include <fcntl.h>
#include <svvs.h>	
#include <sv_env.h>
#include <tiuser.h>
#include <sv_macro.h>

main() {
/*
 *	Try calling t_alloc with a variety of structure types and 
 *	fields within the structures to be allocated.  Check that the
 *	returned structures can be accessed.  Check the aux. fields for
 *	allocation if requested.
 *
 *  Note that some sanity checking on the transport provider is 
 *	done, in case some of the 'fields' buffers are not supported.
 *	Problems of this sort will generate warnings.
 *	
 *
 *	The following table is xref of type vs. valid fields:
 *
 *					T_ADDR		T_OPT		T_UDATA		T_ALL
 *				+-----------+-----------+-----------+-----------+
 *	T_BIND		|	  X		|			|			|	  X		|
 *	T_CALL		|	  X		|	  X		|	  X		|	  X		|
 *	T_OPTMGMT	|			|	  X		|			|	  X		|
 *	T_DIS		|			|			|	  X		|	  X		|
 *	T_UNITDATA	|	  X		|	  X		|	  X		|	  X		|
 *	T_UDERROR	|	  X		|	  X		|			|	  X		|
 *	T_INFO		|			|			|			|	  X		|
 *				+-----------+-----------+-----------+-----------+
 */

	setup("t_alloc");
	funct("t_alloc");
	abstract("Check t_alloc( fd, T_BIND, T_ADDR ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_bind, ");
	abstract("with the addr field allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the qlen member.  ");
	abstract("Check that the new struct t_bind has non-zero");
	abstract("addr and addr.maxlen.  Verify the space has");
	abstract("been allocated by reading and writing addr buffer.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_BIND, T_ADDR);
	finis

	abstract("Check t_alloc( fd, T_BIND, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_bind, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the qlen member.   ");
	abstract("Check that the new struct t_bind has non-zero");
	abstract("addr and addr.maxlen.  Verify the space has");
	abstract("been allocated by reading and writing the addr buffer.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_BIND, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_BIND, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_bind, ");
	abstract("with no fields allocated. Verify the ");
	abstract("structure has been allocated by reading and");
	abstract("writing the qlen member.  Check that ");
	abstract("the new struct t_bind has NULL addr.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_BIND, 0 );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_ADDR ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the addr field allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.");
	abstract("Check that the new struct t_call has non-zero");
	abstract("addr and addr.maxlen.  Verify the space has");
	abstract("been allocated by reading and writing the addr");
	abstract("buffer.  Check that the opt and udata fields are");
	abstract("NULL.  Free the allocated structure, and close");
	abstract("the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_ADDR );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the opt field allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.");
	abstract("Check that the new struct t_call has non-zero");
	abstract("opt and opt.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the opt");
	abstract("buffer. Check that the addr and udata fields are");
	abstract("NULL. Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_OPT );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the udata field allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.");
	abstract("Check that the new struct t_call has non-zero");
	abstract("udata and udata.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the udata");
	abstract("buffer.  Check that the addr and opt fields are");
	abstract("NULL. Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_ADDR | T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the addr and opt fields allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.");
	abstract("Check that the new struct t_call has non-zero");
	abstract("addr, addr.maxlen, opt and opt.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers.  Check that the udata field");
	abstract("is NULL.  Free the allocated structure, and close ");
	abstract("the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_ADDR|T_OPT );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_ADDR | T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the addr and udata fields allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.");
	abstract("Check that the new struct t_call has non-zero");
	abstract("addr, addr.maxlen, udata and udata.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers.  Check that the opt field");
	abstract("is NULL.  Free the allocated structure, and close ");
	abstract("the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_ADDR|T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_OPT | T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the opt and udata fields allocated as well.");
	abstract("Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.");
	abstract("Check that the new struct t_call has non-zero");
	abstract("opt, opt.maxlen, udata and udata.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers.  Check that the addr field");
	abstract("is NULL.   Free the allocated structure, and close ");
	abstract("the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_OPT|T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_ADDR | T_OPT | T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with the addr, opt, and udata fields allocated ");
	abstract("as well.  Verify the structure has been allocated ");
	abstract("by reading and writing the sequence member.  Check");
	abstract("that the new struct t_call has non-zero addr, ");
	abstract("addr.maxlen, opt, opt.maxlen, udata and ");
	abstract("udata.maxlen. Verify the space has been allocated ");
	abstract("by reading and writing these buffers.  Free the ");
	abstract("allocated structure, and close the transport");
	abstract("endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_ADDR|T_OPT|T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_CALL, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the sequence member.  Check that ");
	abstract("the new struct t_call has non-zero udata, ");
	abstract("udata.maxlen, addr, addr.maxlen, opt, and");
	abstract("opt.maxlen. Verify the space has been ");
	abstract("allocated by reading and writing these buffers.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_CALL, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_call, ");
	abstract("with no fields allocated.  Verify the ");
	abstract("structure has been allocated by reading and writing ");
	abstract("the sequence member.  Check that the new ");
	abstract("struct t_call has NULL udata, addr, and opt.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_CALL, 0 );
	finis

	abstract("Check t_alloc( fd, T_OPTMGMT, T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_optmgmt, ");
	abstract("with the opt field allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the flags member.  Check that ");
	abstract("the new struct t_optmgmt has non-zero");
	abstract("opt and opt.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the opt buffer.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_OPTMGMT, T_OPT );
	finis

	abstract("Check t_alloc( fd, T_OPTMGMT, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_optmgmt, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the flags member.  Check that ");
	abstract("the new struct t_optmgmt has non-zero");
	abstract("opt and opt.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the opt buffer.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_OPTMGMT, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_OPTMGMT, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_optmgmt, ");
	abstract("with no fields allocated.  Verify the ");
	abstract("structure has been allocated by reading and writing ");
	abstract("the flags member.  Check that the new ");
	abstract("struct t_optmgmt has NULL opt.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_OPTMGMT, 0 );
	finis

	abstract("Check t_alloc( fd, T_DIS, T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_discon, ");
	abstract("with the udata field allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the sequence and reason members.  ");
	abstract("Check that the new struct t_discon has non-zero");
	abstract("udata and udata.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the udata buffer.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_DIS, T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_DIS, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_discon, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the sequence and reason members.  ");
	abstract("Check that the new struct t_discon has non-zero");
	abstract("udata and udata.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the udata buffer.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_DIS, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_DIS, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_discon, ");
	abstract("with no fields allocated.  Verify the ");
	abstract("structure has been allocated by reading and writing ");
	abstract("the sequence and reason members.  Check ");
	abstract("that the new struct t_discon has NULL udata.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_DIS, 0 );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_ADDR ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the addr field allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero");
	abstract("addr and addr.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the addr");
	abstract("buffer. Check that the opt and udata fields are NULL.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_ADDR );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the opt field allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero");
	abstract("opt and opt.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the opt");
	abstract("buffer.  Check that the addr and udata fields are NULL.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_OPT );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the udata field allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero");
	abstract("udata and udata.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the udata ");
	abstract("buffer. Check that the addr and opt fields are NULL.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_ADDR | T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the addr and opt fields allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero");
	abstract("addr, addr.maxlen, opt, and opt.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers. Check that the udata field is");
	abstract("NULL.  Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_ADDR|T_OPT );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_ADDR | T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the addr and udata fields allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero");
	abstract("addr, addr.maxlen, udata, and udata.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers. Check that the opt field is");
	abstract("NULL.  Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_ADDR|T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_OPT | T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the opt and udata fields allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero");
	abstract("opt, opt.maxlen, udata, and udata.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers. Check that the addr field is");
	abstract("NULL.  Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_OPT|T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_ADDR | T_OPT | T_UDATA ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with the addr, opt, and udata fields allocated ");
	abstract("as well.  Check that the new struct t_unitdata has ");
	abstract("non-zero addr, addr.maxlen, opt, opt.maxlen, ");
	abstract("udata, and udata.maxlen. Verify the space has ");
	abstract("been allocated by reading and writing these buffers.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_ADDR|T_OPT|T_UDATA );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Check that the new struct t_unitdata has non-zero ");
	abstract("addr, addr.maxlen, opt, opt.maxlen, udata, and ");
	abstract("udata.maxlen. Verify the space has been allocated ");
	abstract("by reading and writing these buffers  Free");
	abstract("the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_UNITDATA, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_unitdata, ");
	abstract("with no fields allocated.  Check that the ");
	abstract("new struct t_unitdata has NULL addr, opt,");
	abstract("and udata.  Free the allocated structure, and ");
	abstract("close the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UNITDATA, 0 );
	finis

	abstract("Check t_alloc( fd, T_UDERROR, T_ADDR ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with the addr field allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the error member.  Check ");
	abstract("that the new struct t_uderr has non-zero");
	abstract("addr and addr.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the addr");
	abstract("buffer.  Check that the opt field is NULL.  Free");
	abstract("the allocated structure, and close the transport");
	abstract("endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UDERROR, T_ADDR );
	finis

	abstract("Check t_alloc( fd, T_UDERROR, T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with the opt field allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the error member.  Check");
	abstract("that the new struct t_uderr has non-zero");
	abstract("opt and opt.maxlen. Verify the space has");
	abstract("been allocated by reading and writing the opt");
	abstract("buffer.  Check that the addr field is NULL.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UDERROR, T_OPT );
	finis

	abstract("Check t_alloc( fd, T_UDERROR, T_ADDR | T_OPT ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with the addr and opt fields allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the error member.  Check that the ");
	abstract("new struct t_uderr has non-zero addr,");
	abstract("addr.maxlen, opt and opt.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers.  Free the allocated ");
	abstract("structure, and close the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UDERROR, T_ADDR|T_OPT );
	finis

	abstract("Check t_alloc( fd, T_UDERROR, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the error member.  Check that the ");
	abstract("new struct t_uderr has non-zero addr, ");
	abstract("addr.maxlen, opt and opt.maxlen. Verify ");
	abstract("the space has been allocated by reading and writing");
	abstract("these buffers.  Free the allocated ");
	abstract("structure, and close the transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UDERROR, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_UDERROR, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with no fields allocated.  Verify the ");
	abstract("structure has been allocated by reading and writing ");
	abstract("the error member.  Check that the ");
	abstract("new struct t_uderr has NULL addr and opt.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_UDERROR, 0 );
	finis

	abstract("Check t_alloc( fd, T_INFO, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with all relevant fields allocated as well.");
	abstract("Verify the structure has been allocated by");
	abstract("reading and writing the addr, options, tsdu, etsdu, ");
	abstract("connect, discon, and servtype members.");
	abstract("Free the allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_INFO, T_ALL );
	finis

	abstract("Check t_alloc( fd, T_INFO, 0 ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Use t_alloc to allocate a struct t_uderr, ");
	abstract("with no fields allocated.  Verify the ");
	abstract("structure has been allocated by reading and writing ");
	abstract("the addr, options, tsdu, etsdu, connect, ");
	abstract("discon, and servtype members.  Free the ");
	abstract("allocated structure, and close the ");
	abstract("transport endpoint.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, 0, T_INFO, 0 );
	finis

	abstract("Check t_alloc( bad_fd, T_BIND, T_ALL ).");
	abstract("Call t_open to obtain a file descriptor.");
	abstract("Call t_close to close the transport endpoint.");
	abstract("Call t_alloc with the closed file descriptor");
	abstract("which no longer refers to a transport endpoint.");
	abstract("Check that NULL is returned, and that t_errno ");
	abstract("is set to TBADF.");
	start
		break_on( ERROR );
		alloc_i( TPVCFILE0, TBADF, T_BIND, T_ALL );
	finis

	done();
}
alloc_i( provider, exp_terrno, stype, fields )
char	*provider;		/* transport provider name					*/
int		exp_terrno;		/* expected terrno value					*/
int		stype;			/* structure type							*/
int		fields;			/* fields which should be alloc				*/
{
	struct	t_info	info;
	struct	t_info	*ifptr = &info;
	int				tfp;				/* xport provider file des	*/
	int				myfields;			/* reconstruct field flags	*/
	int				size_problem;		/* flag if field size prob	*/	
	char			*aptr;				/* return val from t_alloc	*/

	char			*t_alloc();

/*
 *	note that error's will cause a break back to the calling pgm
 */
	prep("open the transport provider: \"%s\"", provider);
	start
		tfp = t_open( provider, O_RDWR, ifptr );
	finis

	if(exp_terrno == TBADF) {
		prep("close the transport provider");
		start
			expecting( SUCCESS );
			t_close(tfp);
		finis
	}			
	else {
		prep("Check the struct info returned from t_open to see if the");
		prep("requested fields can be allocated.  Sizes of -1 or -2");
		prep("will cause t_alloc to fail with TSYSERR/EINVAL, in");
		prep("which case a warning is generated here to indicate that");
		prep("this test case will not be able to allocate a buffer.");
		prep("Similiarly, a size of 0 generates a warning that no");
		prep("buffer will actually be allocated.  In both cases, the");
		prep("test should be executed again with a transport provider");
		prep("that supports non-negative buffer sizes.");
		start
/*	
 *			Can't assume T_ALL == T_ADDR|T_OPT|T_UDATA
 *
 *			If T_ALL is requested, reconstruct flags based upon request
 *				(see xref table at top of this pgm)
 *		 	do a sanity check on the requested fields, 
 *			only use documented ones
 *
 *	 		Then check size info for requested T_ADDR, T_OPT and T_UDATA 
 *			fields
 */

			myfields = fields;
			size_problem = 0; /* set if bad size will cause TSYSERR */
			switch( stype ) {
			case T_CALL:	
				if (fields = T_ALL) 
					myfields = T_ADDR | T_OPT | T_UDATA;
#ifdef	DEVELOPMENT
				if( myfields & ~(T_ADDR | T_OPT | T_UDATA) )
					warning("Invalid fields requested in this test case");
#endif
				if (myfields & T_ADDR)
					size_problem |= sizchk( info.addr, "T_ADDR", 0 );
				if (myfields & T_OPT)
					size_problem |= sizchk( info.options, "T_OPT", 0 );	
				if (myfields & T_UDATA) {
/*
 *					use '?' to suppress error from 2nd sizchk if 1st fails
 */
					size_problem |= (sizchk( info.connect, "T_UDATA", 0 ))?
						1:sizchk( info.discon, "T_UDATA", 0 );
				}
				break;
			case T_UNITDATA:
				if (fields = T_ALL) 
					myfields = T_ADDR | T_OPT | T_UDATA;
#ifdef	DEVELOPMENT
				if( myfields & ~(T_ADDR | T_OPT | T_UDATA) )
					warning("Invalid fields requested in this test case");
#endif
				if (myfields & T_ADDR)
					size_problem |= sizchk( info.addr, "T_ADDR", 0 );
				if (myfields & T_OPT)
					size_problem |= sizchk( info.options, "T_OPT", 0 );
				if (myfields & T_UDATA)
					size_problem |= sizchk( info.tsdu, "T_UDATA", 1 );
				break;
			case T_BIND:
				if (fields = T_ALL) 
					myfields = T_ADDR;
#ifdef	DEVELOPMENT
				if( myfields & ~T_ADDR )
					warning("Invalid fields requested in this test case");
#endif
				if (myfields & T_ADDR)
					size_problem = sizchk( info.addr, "T_ADDR", 0 );
				break;
			case T_OPTMGMT:
				if (fields = T_ALL) 
					myfields = T_OPT;
#ifdef	DEVELOPMENT
				if( myfields & ~T_OPT )
					warning("Invalid fields requested in this test case");
#endif
				if (myfields & T_OPT)
					size_problem = sizchk( info.options, "T_OPT", 0 );
				break;
			case T_DIS:
				if (fields = T_ALL) 
					myfields = T_UDATA;
#ifdef	DEVELOPMENT
				if( myfields & ~T_UDATA )
					warning("Invalid fields requested in this test case");
#endif
				if (myfields & T_UDATA)
					size_problem = sizchk( info.discon, "T_UDATA", 0 );
				break;
			case T_UDERROR:
				if (fields = T_ALL) 
					myfields = T_OPT | T_UDATA;
#ifdef	DEVELOPMENT
				if( myfields & ~(T_OPT | T_UDATA) )
					warning("Invalid fields requested in this test case");
#endif
				if (myfields & T_ADDR)
					size_problem |= sizchk( info.addr, "T_ADDR", 0 );
				if (myfields & T_OPT)
					size_problem |= sizchk( info.options, "T_OPT", 0 );
				break;
			case T_INFO:
				if (fields = T_ALL) 
					myfields = 0;
#ifdef	DEVELOPMENT
				if( myfields )
					warning("Invalid fields requested in this test case");
#endif
				/* no fields for T_INFO, hence no size check  */
				break;
			default:
#ifdef	DEVELOPMENT
				warning("Invalid structure type specified");
#endif
				return;
			}
		finis
	}
	testing("call t_alloc, expecting %s",
	  (exp_terrno|size_problem)?"failure":"success");
	start
		continue_on( ERROR );
		/* binding problem, must call expecting here */
		if(exp_terrno == TBADF) {
			expecting( TBADF );
		} else if( size_problem ) {
			expecting( TSYSERR );
			expecting( EINVAL );
		}
		aptr=t_alloc(tfp,stype,fields);
	finis
	if ( !exp_terrno && !size_problem ) {
		testing("Check that the structure has been allocated");
		testing("by attempting to read the allocated space");
		start
			continue_on( ERROR );
			check_struct(aptr,stype);
		finis
	}
	if ( !exp_terrno && !size_problem ) {
		testing("Check that the requested fields have been allocated");
		testing("with non-NULL buffer pointers, and that the buffers");
		testing("can be read.  Also check that buffers which were not");
		testing("requested have pointers set to NULL");
		start
			switch(stype) {

			case T_BIND:
/*
 *				Verify addr buffer allocation
 */
				chk_netbuf(myfields, "t_bind", T_ADDR,
					"t_addr", info.addr, 0,
					&(((struct t_bind *)aptr)->addr));
				break;
			case T_CALL:
/*
 *				Verify addr buffer allocation
 */
				chk_netbuf(myfields, "t_call", T_ADDR,
					"t_addr", info.addr, 0,
					&(((struct t_call *)aptr)->addr));
/*
 *				Verify opt buffer allocation
 */
				chk_netbuf(myfields, "t_call", T_OPT,
					"t_opt", info.options, 0,
					&(((struct t_call *)aptr)->opt));
/*
 *				Verify udata buffer allocation
 */
				chk_netbuf(myfields, "t_call", T_UDATA,
					"t_udata", max( info.connect, info.discon), 0,
					&(((struct t_call *)aptr)->udata));
				break;
			case T_UNITDATA:
/*
 *				Verify addr buffer allocation
 */
				chk_netbuf(myfields, "t_unitdata", T_ADDR,
					"t_addr", info.addr, 0,
					&(((struct t_unitdata *)aptr)->addr));
/*
 *				Verify opt buffer allocation
 */
				chk_netbuf(myfields, "t_unitdata", T_OPT,
					"t_opt", info.options, 0,
					&(((struct t_unitdata *)aptr)->opt));
/*
 *				Verify udata buffer allocation
 */
				chk_netbuf(myfields, "t_unitdata", T_UDATA,
					"t_udata", info.tsdu, 1,
					&(((struct t_unitdata *)aptr)->udata));
				break;

			case T_UDERROR:
/*
 *				Verify addr buffer allocation
 */
				chk_netbuf(myfields, "t_uderror", T_ADDR,
					"t_addr", info.addr, 0,
					&(((struct t_uderr *)aptr)->addr));
/*
 *				Verify opt buffer allocation
 */
				chk_netbuf(myfields, "t_uderror", T_OPT,
					"t_opt", info.options, 0,
					&(((struct t_uderr *)aptr)->opt));

				break;
			case T_OPTMGMT:
/*
 *				Verify opt buffer allocation
 */
				chk_netbuf(myfields, "t_optmgmt", T_OPT,
					"t_opt", info.options, 0,
					&(((struct t_optmgmt *)aptr)->opt));
				break;
			case T_DIS:
				chk_netbuf(myfields, "t_dis", T_UDATA,
					"t_udata", info.discon, 1,
					&(((struct t_optmgmt *)aptr)->opt));
				break;
			case T_INFO:
/*
 *				no buffers possible
 */
				break;
			}
		finis
		cleanup("free the allocated structure");
		start
			expecting( SUCCESS );
			t_free(aptr,stype);
		finis
	}
	if( exp_terrno != TBADF) {
		cleanup("close the transport provider");
		start
			expecting( SUCCESS );
			t_close(tfp);
		finis
	}
}
sizchk( size, name, min )
int size;
char *name;
int min;
{
	if( !min && !size )
		warning("%s%s%s%s",
		  "Struct info indicates a ",
		  name,
		  " field size of 0.\n",
		  "The validity check of the allocated buffer will be suppressed");
	if( size < min ) {
		warning("%s%s%s%s%s%s%s%s%s",
		  "Struct info indicates ",
		  name,
		  " field is unsupported by this transport provider\n",
		  "Test case will expect NULL return from t_alloc\n",
		  "and t_errno set to TSYSERR\n",
		  "This test should be re-run with a transport provider that ",
		  "supports fixed size ",
		  name,
		  " buffers" );
		return(1);
	}
	return(0);
}
check_struct( ptr, stype )
char	*ptr;	/* loc to test		*/
int		stype;	/* type of struct	*/
{
	struct	t_bind		bind;
	struct	t_call		call;
	struct	t_optmgmt	optmgmt;
	struct	t_discon	discon;
	struct	t_uderr		uderr;
	struct	t_info		info;

	switch( stype ) {
	case T_BIND:
		bind.qlen = ~((struct t_bind *)ptr)->qlen;
		((struct t_bind *)ptr)->qlen = bind.qlen;
		if(((struct t_bind *)ptr)->qlen != bind.qlen)
			error("qlen member of struct t_bind is not accessible");
		break;
	case T_CALL:
		call.sequence = ~((struct t_call *)ptr)->sequence;
		((struct t_call *)ptr)->sequence = call.sequence;
		if(((struct t_call *)ptr)->sequence != call.sequence)
			error("sequence member of struct t_call is not accessible");
		break;
	case T_OPTMGMT:
		optmgmt.flags = ~((struct t_optmgmt *)ptr)->flags;
		((struct t_optmgmt *)ptr)->flags = optmgmt.flags;
		if(((struct t_optmgmt *)ptr)->flags != optmgmt.flags)
			error("flags member of struct t_optmgmt is not accessible");
		break;
	case T_DIS:
		discon.sequence = ~((struct t_discon *)ptr)->sequence;
		((struct t_discon *)ptr)->sequence = discon.sequence;
		if( ((struct t_discon *)ptr)->sequence != discon.sequence )
			error("sequence member of struct t_discon is not accessible");
		discon.reason = ~((struct t_discon *)ptr)->reason;
		((struct t_discon *)ptr)->reason = discon.reason;
		if(((struct t_discon *)ptr)->reason != discon.reason)
			error("reason member of struct t_discon is not accessible");
		break;
	case T_UNITDATA:
		break;
	case T_UDERROR:
		((struct t_uderr *)ptr)->error = uderr.error;
		if(((struct t_uderr *)ptr)->error != uderr.error)
			error("error member of struct t_uderr is not accessible");
		break;
	case T_INFO:
		((struct t_info *)ptr)->addr = info.addr;
		if(((struct t_info *)ptr)->addr != info.addr)
			error("addr member of struct t_info is not accessible");
		((struct t_info *)ptr)->options = info.options;
		if(((struct t_info *)ptr)->options != info.options)
			error("options member of struct t_info is not accessible");
		((struct t_info *)ptr)->tsdu = info.tsdu;
		if(((struct t_info *)ptr)->tsdu != info.tsdu)
			error("tsdu member of struct t_info is not accessible");
		((struct t_info *)ptr)->etsdu = info.etsdu;
		if(((struct t_info *)ptr)->etsdu != info.etsdu)
			error("etsdu member of struct t_info is not accessible");
		((struct t_info *)ptr)->connect = info.connect;
		if(((struct t_info *)ptr)->connect != info.connect)
			error("connect member of struct t_info is not accessible");
		((struct t_info *)ptr)->discon = info.discon;
		if(((struct t_info *)ptr)->discon != info.discon)
			error("discon member of struct t_info is not accessible");
		((struct t_info *)ptr)->servtype = info.servtype;
		if(((struct t_info *)ptr)->servtype != info.servtype)
			error("servtype member of struct t_info is not accessible");
		break;
	}
}
chk_netbuf( fields, sname, testfield, bname, info_cnt, min_ok_val, nptr )
int fields;		/* fields passed to t_alloc: T_ADDR | T_OPT | T_UDATA	*/
char *sname;	/* structure name returned by t_alloc: "t_call", etc	*/
int testfield;	/* netbuf field under test: T_ADDR, T_OPT, T_UDATA		*/
char *bname;	/* str rep of testfield: "t_addr", "t_opt", "t_udata"	*/
int info_cnt;	/* corresponding value from info.whatever				*/
int min_ok_val;	/* minimum acceptable size for t_alloc to have worked	*/
struct netbuf *nptr;
{
	if( fields & testfield ) { /* requested this field be allocated	*/
		if( info_cnt <= min_ok_val ) {	/* size problem encountered		*/
			if( !min_ok_val && !info_cnt ) {
				message("%s%s%s%s",
				  "test of ",
				  bname,
				  " buffer allocation for struct ",
				  sname,
				  "\nwas suppressed due to buffer size of 0");
				return;
			}
			message("%s%s%s%s",
			  "test of ",
			  bname,
			  " buffer allocation for struct ",
			  sname,
			  "\nwas suppressed because ",
			  bname,
			  " is not supported by this transport provider");
			return;
		}
		if(!nptr->buf)
			error("%s field of struct %s has NULL buf pointer",
			  bname,sname);
		if(!nptr->maxlen)
			error("%s field of struct %s has maxlen of 0",bname,sname);
		else if(nptr->maxlen < info_cnt)
			error("%s%s%s%s%s%s%d%s%s%s%d",
			  bname,
			  " field of struct ",
			  sname,
			  " has maxlen less than\nthe value returned in ",
			  " the info structure by t_open\n",
			  "value returned by topen's info: ",
			  info_cnt,
			  "\n",
			  bname,
			  ".maxlen: ",
			  nptr->maxlen);
		if(nptr->len)
			error("%s field of struct %s has non-zero len\n%s.len: %d",
			  bname,sname,bname,nptr->len);
		if( nptr->buf && chk_mem( nptr->buf, info_cnt ) )
			error("%s field of struct %s has inaccessible buffer",
				bname,sname);
	}
	else {
/*
 *		No buffer was requested, 
 *		check that one was not allocated
 */
		if(!nptr->buf)
			error("%s field of struct %s has non-NULL buf pointer",
			  bname,sname);
		if(!nptr->maxlen)
			error("%s field of struct %s has non-zero maxlen",bname,sname);
	}
}
chk_mem(ptr, size)
char *ptr;
int size;
{
	register char c;
	register int i;

	for(c='\0',i=0;i<size;i++,c++,ptr++) {
		*ptr = c;		
		if( *ptr != c )
			return( 1 );
	}
	return( 0 );
}
