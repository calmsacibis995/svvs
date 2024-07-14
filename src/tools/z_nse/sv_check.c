#ident "@(#)sv_check.c	1.1"
#include <stdio.h>	
#include <fcntl.h>
#include <sv_base.h>
#include <sv_nse.h>
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>
#include <sys/stropts.h>
#include <string.h>

#define FILLCHAR	'x'

checkdata(sctlptr,rctlptr,sdataptr,rdataptr)
struct	strbuf	*sctlptr,*rctlptr;
struct	strbuf	*sdataptr,*rdataptr;
{
	expecting( VOID );
	if(rctlptr)
	{
		testing("Verify the length of the control message retrieved");
		testing("from the stream head is %d.", sctlptr->len);
		start
			if(sctlptr->len != rctlptr->len)
				error("Incorrect length retrieved.");
		finis

		testing("Verify the control message retrieved from the stream");
		testing("is %d \'%c\'.", sctlptr->len, FILLCHAR);
		start
			if(strncmp(sctlptr->buf,rctlptr->buf,sctlptr->len))
				error("Incorrect control message retrieved.");
		finis

	}

	if(rdataptr)
	{
		testing("Check that the control message retrieved from the");
		testing("stream head was that one sent down stream.");
		start
			if(strncmp(sdataptr->buf,rdataptr->buf,sdataptr->len))
				error("Incorrect control msg. retrieved.");
		finis

		testing("Check that the length of the control message");
		testing("retrieved from the stream head equals the length");
		testing("of the message sent down stream.");
		start
			if(sdataptr->len != rdataptr->len)
				error("Incorrect length retrieved.");
		finis
	}

	return(0);
}
