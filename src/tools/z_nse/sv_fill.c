#ident "@(#)sv_fill.c	1.2"
#include <stdio.h>	
#include <fcntl.h>
#include <sv_base.h>
#include <sv_nse.h>
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>
#include <sys/stropts.h>

#define FILLCHAR	'x'
#define RCV	2
#define SND     1

filldata(mode,ctlptr,dataptr,csize,dsize,cdata,ddata)
register char	*cdata,*ddata;
int	csize,dsize;
struct	strbuf	*ctlptr,*dataptr;
int	mode;
{
	register int	i;
	expecting( VOID );
	switch(mode)
	{
	case RCV:
	prep("Set up the ctlptr structure for the getmsg call. Set the");
	prep("maximum length field to %d, the length field to 0",csize);
	prep("and clear the buffer for the retrieved message.");
	start
		expecting( VOID );
		ctlptr->maxlen = csize;
		ctlptr->buf = cdata;
		ctlptr->len = 0;

		expecting( VOID );
		for(i=0; i < csize; i++)
			*cdata++ = 0;
	finis

	prep("Set up the dataptr structure for the getmsg call. Set the");
	prep("maximum length field to %d, the length field to 0",dsize);
	prep("and clear the buffer for the retrieved message.");
	start
		dataptr->maxlen = dsize;
		dataptr->buf = ddata;
		dataptr->len = 0;

		expecting( VOID );
		for(i=0; i < dsize; i++)
			*ddata++ = 0;
	finis
		break;


	case SND:
		
	prep("Set up the ctlptr structure for the putmsg call. Set the");
	prep("maximum length and length fields to %d and fill the ",csize);
	prep("buffer to be sent down stream with \'%c\`.", FILLCHAR);
	start
		ctlptr->buf = cdata;
		ctlptr->maxlen = csize;
		ctlptr->len = csize;

		expecting( VOID );
		for(i=0; i < csize; i++)
			*cdata++ = FILLCHAR;
	finis

	prep("Set up the dataptr structure for the putmsg call. Set the");
	prep("maximum length and length fields to %d and fill the ",dsize);
	prep("buffer to be sent down stream with \'%c\`.", FILLCHAR);
	start
		dataptr->buf = ddata;
		dataptr->maxlen = dsize;
		dataptr->len = dsize;

		expecting( VOID );
		for(i=0; i < dsize; i++)
			*ddata++ = FILLCHAR;
	finis

		break;

		default:
			error("Bad mode passed to filldata");
	}

	return(0);
}
