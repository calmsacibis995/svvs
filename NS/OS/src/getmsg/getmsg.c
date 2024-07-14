#ident "@(#)getmsg.c	1.3"

#include <stdio.h>	
#include <fcntl.h>
#include <sv_base.h>
#include <sv_nse.h>
#include <svvs.h>	
#include <sv_env.h>
#include <sv_macro.h>
#include <stropts.h>
#include <sv_streams.h>
#include <string.h>
#include <values.h>

#define	rendevous()	(v_sema(), psema())

void sigcatch();
main()
{
	int	fd,mux,mux_id;
	int	retn,pid;
	struct	strioctl	istr;
	struct	strbuf	sctlbuf,rctlbuf;
	struct	strbuf	sdatabuf,rdatabuf;
	char	rctlmdata[BUFSIZE],rdatamdata[BUFSIZE];
	char	sctlmdata[BUFSIZE],sdatamdata[BUFSIZE];
	int	flags;
	long	tvec, aftvec;


	setup("getmsg");
	funct("getmsg");


	abstract("Open two drivers and initialize the semaphore mechanism.");
	start
		exit_on( ERROR );
		prep("Open the loop back streams based driver");
		prep("\"%s\" for reading and writing.", DRIVER);
		start
			fd = open(DRIVER,O_RDWR);
		finis
			
		prep("Open the streams based multiplex driver \"%s\"", MUX);
		prep("for reading and writing.");
		start
			mux = open(MUX, O_RDWR);
		finis

		seminit( 2);
	finis

	abstract("Verify that if O_NDELAY is not set, and RS_HIPRI is");
	abstract("set, getmsg will block until a priority control");
	abstract("message is retrieved.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		abort_on( ERROR );
		switch((retn = fork()))
		{
		case 0:
			prep("Set flags to RS_HIPRI and get the current");
			prep("time.");
			start
				flags=0;
			finis

			testing("Call getmsg to block until a priority control ");
			testing("message is present at the stream head read ");
			testing("queue or until it times out.");
			start
				time(&tvec);
				getmsg(fd, &rctlbuf,&rdatabuf, &flags);
				time(&aftvec);
			finis
			
			testing("Check the current time and see if the time");
			testing("getmsg blocked is equal to the timeout time");
			testing("for the loop back driver.");
			start
				continue_on( ERROR );
				expecting( VOID );
				if((aftvec - tvec) < 15)
					error("Getmsg returned before it timed out.");
			finis
	
			continue_on( ERROR );
			checkdata(&sctlbuf,&rctlbuf, 0,0);

			testing("Verify flags is set to RS_HIPRI.");
			start
				if(flags != RS_HIPRI)
					error("Bad priority flag return");
			finis

			done();
		default:
			break;
		}

		sleep(30);
		prep("Send a priority control message down stream.");
		start
			putmsg(fd,&sctlbuf,0,RS_HIPRI);
		finis

		cleanup("Wait for the child process to complete.");
		start
			continue_on( ERROR );
			wait(&retn);
			retn >>= 8;
			if(retn != 0)
				error("Bad child return");
		finis

	finis

	abstract("Verify that if O_NDELAY flag is not set, getmsg will");
	abstract("block until a non priority control message is retrieved.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		abort_on( ERROR );
		switch((retn = fork()))
		{
		case 0:
			prep("Set flags to 0 and get the current time.");
			start
				flags=0;
			finis

			testing("Call getmsg to block until a non priority ");
			testing("control message is present at the stream ");
			testing("head read queue or until it times out.");
			start
				time(&tvec);
				getmsg(fd, &rctlbuf,&rdatabuf, &flags);
				time(&aftvec);
			finis
			
			testing("Check the current time and see if the time");
			testing("getmsg blocked is equal to the timeout time");
			testing("for the loop back driver.");
			start
				continue_on( ERROR );
				expecting( VOID );
				if((aftvec - tvec) < 15)
					error("Getmsg returned before it timed out.");
			finis
	
			continue_on( ERROR );
			checkdata(&sctlbuf,&rctlbuf, 0,0);

			testing("Verify flags is set to 0.");
			start
				if(flags != 0)
					error("Bad priority flag return");
			finis

			done();
		
		default:
			break;
		}

		sleep(30);
		prep("Send non priority control message down stream.");
		start
			putmsg(fd,&sctlbuf,0,0);
		finis

		cleanup("Wait for the child process to complete.");
		start
			continue_on( ERROR );
			wait(&retn);
			retn >>= 8;
			if(retn != 0)
				error("Bad child return");
		finis

	finis

	abstract("Verify that if O_NDELAY flag is not set, getmsg will");
	abstract("block until a non priority data message is retrieved.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		abort_on( ERROR );
		switch((retn = fork()))
		{
		case 0:
			prep("Set flags to 0 and get the current time.");
			start
				flags=0;
			finis

			testing("Call getmsg to block until a non priority ");
			testing("data message is present at the stream ");
			testing("head read queue or until it times out.");
			start
				time(&tvec);
				getmsg(fd, &rctlbuf,&rdatabuf, &flags);
				time(&aftvec);
			finis
			
			testing("Check the current time and see if the time");
			testing("getmsg blocked is equal to the timeout time");
			testing("for the loop back driver.");
			start
				continue_on( ERROR );
				expecting( VOID );
				if((aftvec - tvec) < 15)
					error("Getmsg returned before it timed out.");
			finis
	
			continue_on( ERROR );
			checkdata(0,0,&sdatabuf,&rdatabuf);

			testing("Verify flags is set to 0.");
			start
				if(flags != 0)
					error("Bad priority flag return");
			finis

			done();
		default:
			break;
		}


		sleep(30);
		prep("Send a non priority data message down stream.");
		start
			putmsg(fd,0,&sdatabuf,0);
		finis

		cleanup("Wait for the child process to complete.");
		start
			continue_on( ERROR );
			wait(&retn);
			retn >>= 8;
			if(retn != 0)
				error("Bad child return");
		finis

	finis

	abstract("Verify proper operation of getmsg with control and data ");
	abstract("messages.");
	start
		prep("Close the loop back driver and open for read/write.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);


		prep("Call putmsg to send the message down stream.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		flags=0;
		testing("Call getmsg with flags set to 0, and buffers");
		testing("for both control and data messages.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);
	finis


	abstract("Send a control message and data message down stream.");
	abstract("Call getmsg with a NULL control pointer.  Verify");
	abstract("getmsg returns MORECTL.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call putmsg to send the message down stream.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		flags=0;
		testing("Call getmsg with flags=0, control pointer set to");
		testing("NULL and an address to retrieve a data message.");
		testing("Expecting getmsg to return MORECTL.");
		start
			if((retn = getmsg(fd,0,&rdatabuf,&flags)) != MORECTL)
				error("%s", getmsgerr);
		finis

		checkdata(0,0,&sdatabuf,&rdatabuf);

		testing("Call getmsg to retrieve the control message. This");
		testing("is testing the fact that the message remained on ");
		testing("the queue.");
		start
			getmsg(fd,&rctlbuf,0,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,0,0);
	finis


	abstract("Verify if control maxlen is -1, the message is left on the");
	abstract("stream head read queue.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call putmsg to send the message down stream.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		flags=0;
		rctlbuf.maxlen = -1;
		testing("Call getmsg with flags=0 and maxlen of the ctlptr");
		testing("set to -1. Expecting getmsg to return MORECTL.");
		start
		       if((retn=getmsg(fd,&rctlbuf,&rdatabuf,&flags)) != MORECTL)
				error("%s", getmsgerr);
		finis

		checkdata(0,0,&sdatabuf,&rdatabuf);

		rctlbuf.maxlen = BUFSIZE;
		testing("Call getmsg to get the control message");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,0,0);
	finis


	abstract("Send a control message and data message down stream.");
	abstract("Call getmsg with a NULL data pointer.  Verify getmsg");
	abstract("returns MOREDATA.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call putmsg to send the message down stream.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		flags=0;
		testing("Call getmsg with flags=0, data pointer set to");
		testing("NULL and an address to retrieve a control message.");
		testing("Expecting getmsg to return MOREDATA.");
		start
			if((retn = getmsg(fd,&rctlbuf,0,&flags)) != MOREDATA)
				error("%s", getmsgerr);
		finis

		checkdata(&sctlbuf,&rctlbuf,0,0);

		testing("Call getmsg to retrieve the data message left on");
		testing("the stream head read queue.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(0,0,&sdatabuf,&rdatabuf);
	finis


	abstract("Verify flags is set to RS_HIPRI if a priority message is");
	abstract("received.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call putmsg to send the message down stream.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,RS_HIPRI);
		finis

		flags=0;
		testing("Call getmsg to retrieve the data and control");
		testing("messages.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);

		testing("Verify flags was set to RS_HIPRI.");
		start
			if(flags != RS_HIPRI)
				error("Bad priority flag returned by getmsg.");
		finis
	finis


	abstract("Verify errno gets set to EAGAIN if the O_NDELAY flag is");
	abstract("set and no messages are available.");
	start

		prep("Close the loop back driver and open it for ");
		prep("read/write with the O_NDELAY flag set.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR|O_NDELAY);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);


		flags=0;
		testing("Call getmsg with flags set to zero. Expecting");
		testing("failure with errno set to EAGAIN");
		start
			expecting( EAGAIN );
			getmsg(fd, &rctlbuf,&rdatabuf, &flags);
		finis

		cleanup("Close the loop back driver and open it for read/write.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR);
		finis
	finis


	abstract("Verify the ENOSTR errno. Call getmsg with a valid file");
	abstract("descriptor for a file other than a streams device.");
	start
		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Open the non stream file \"/etc/passwd\".");
		start
			retn = open("/etc/passwd", O_RDONLY);
		finis

		testing("Call getmsg with the file descriptor of the opened ");
		testing("file. Expecting failure with errno set to ENOSTR.");
		start
			expecting( ENOSTR );
			getmsg(retn, &rctlbuf,&rdatabuf, &flags);
		finis

		cleanup("Close the temporary file.");
		start
			close( retn );
		finis
	finis

	abstract("Verify if data maxlen is -1, the message is left on the");
	abstract("stream head read queue.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call putmsg to send the message down stream.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		flags=0;
		rdatabuf.maxlen = -1;
		testing("Call getmsg with flags=0 and maxlen of the data");
		testing("set to -1. Expecting getmsg to return MOREDATA.");
		start
		      if((retn=getmsg(fd,&rctlbuf,&rdatabuf,&flags)) != MOREDATA)
				error("%s", getmsgerr);
		finis

		checkdata(&sctlbuf,&rctlbuf,0,0);

		rdatabuf.maxlen = BUFSIZE;
		testing("Call getmsg to get the data message");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(0,0,&sdatabuf,&rdatabuf);
	finis


	abstract("Verify the EBADF errno. Call getmsg with a valid file");
	abstract("descriptor for a closed file and a streams based file");
	abstract("opened for reading only.");
	start
		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Close the loop back driver.");
		start
			close( fd );
		finis

		testing("Call getmsg with a file descriptor of a closed");
		testing("file. Expecting failure with errno set to EBADF.");
		start
			expecting( EBADF );
			getmsg(fd, &rctlbuf,&rdatabuf, &flags);
		finis

		cleanup("Open the loop back driver for read and write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis
	finis


	abstract("Verify errno is set to EINTR if a signal is received");
	abstract("while in getmsg.");
	start
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Set up a signal handling routine.");
		start
			signal(SIGALRM,sigcatch);
		finis

		prep("Call alarm with an argument of 15.");
		start
			alarm(15);
		finis

		flags=0;

		testing("Call getmsg. Expecting failure with errno set to EINTR.");
		start
			expecting( EINTR );
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis
	finis

	
	abstract("Verify errno is set to EINVAL if an illegal value is");
	abstract("specified for flags.");
	start
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		flags= RS_HIPRI+1;

		testing("Call getmsg with the flags value set to");
		testing("RS_HIPRI + 1. Expecting failure with errno set");
		testing("to EINVAL.");
		start
			expecting( EINVAL );
			getmsg(fd, &rctlbuf,&rdatabuf, &flags);
		finis
	finis


	abstract("Verify errno is set to EFAULT if a bad address is specified");
	abstract("for the ctl and data pointers.");
	start
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		flags=0;

		testing("Call getmsg with the address for the ctlptr set to");
		testing("-1 and flags set to zero. Expecting failure with ");
		testing("errno set to EFAULT.");
		start
			expecting( EFAULT );
			getmsg(fd, -1,&rdatabuf, &flags);
		finis

		testing("Call getmsg with the address for the dataptr set to");
		testing("-1 and flags set to zero. Expecting failure with ");
		testing("errno set to EFAULT.");
		start
			expecting( EFAULT );
			getmsg(fd, &rctlbuf,-1, &flags);
		finis
	finis

	abstract("Verify that getmsg will return MOREDATA(MORECTL) if there");
	abstract("are more data(control) messages on the queue after a call");
	abstract("to getmsg.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE-1,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call putmsg to send a control message down stream.");
		start
			putmsg(fd,&sctlbuf,0,0);
		finis

		flags=0;
		testing("Call getmsg to retrieve %d bytes of info.  from",
					BUFSIZE-1);
		testing("the stream head. Expecting getmsg to return MORECTL.");
		start
			if((retn=getmsg(fd,&rctlbuf,&rdatabuf,&flags))!= MORECTL)
				error("%s", getmsgerr);
		finis

		vcheckdata(&sctlbuf,&rctlbuf,BUFSIZE-1, 0,0,0);

		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call getmsg to read in the byte of info left on ");
		testing("the stream head.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		vcheckdata(&sctlbuf,&rctlbuf,1, 0,0,0);

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE-1,rctlmdata,rdatamdata);

		prep("Call putmsg to send the data message down stream.");
		start
			putmsg(fd,0,&sdatabuf,0);
		finis

		flags=0;

		testing("Call getmsg to retrieve %d bytes of info.  from",
					BUFSIZE-1);
		testing("the stream head. Expecting getmsg to return MOREDATA.");
		start
			if((retn=getmsg(fd,&rctlbuf,&rdatabuf,&flags))!=MOREDATA)
				error("%s", getmsgerr);
		finis

		vcheckdata(0,0,0,&sdatabuf,&rdatabuf,BUFSIZE-1);

		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,1,rctlmdata,rdatamdata);

		flags=0;

		testing("Call getmsg to read in the byte of info left on ");
		testing("the stream head.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		vcheckdata(0,0,0,&sdatabuf,&rdatabuf,1);

	finis


	abstract("Verify that errno is set to EINVAL if a getmsg is attempted");
	abstract("on a stream linked below a multiplexor.");
	start
		prep("Call ioctl to link the loop back driver to the ");
		prep("multiplexor. The ioctl cmd is I_LINK.");
		start
			expecting( VOID );
			mux_id = ioctl(mux,I_LINK,fd);
		finis

		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call getmsg and attempt to access the loop back");
		testing("driver. Expecting failure with errno set to EINVAL.");
		start
			expecting( EINVAL );
			getmsg(fd,&rctlbuf,&rdatabuf,0);
		finis

		cleanup("Call ioctl to unlink the loop back driver.");
		cleanup("The command used in I_UNLINK.");
		start
			ioctl(fd, I_UNLINK, mux_id);
		finis
	finis


	abstract("Close the two drivers.");
	start
		cleanup("Close the loop back driver.");
		start
			close( fd );
		finis

		cleanup("Close the multiplex driver.");
		start
			close( mux );
		finis
	finis


	done();
}

vcheckdata(sctlptr,rctlptr,ctlsize,sdataptr,rdataptr,datasize)
struct	strbuf	*sctlptr,*rctlptr;
struct	strbuf	*sdataptr,*rdataptr;
int	ctlsize,datasize;
{

	expecting( VOID );
	if(rctlptr)
	{
		testing("Check that the length of the control message");
		testing("retrieved from the stream head equals the length");
		testing("of the message sent down stream.");
		start
			if(rctlptr->len != ctlsize)
				error("Incorrect length retrieved.");
		finis

		testing("Check that the control message retrieved from the");
		testing("stream head was that one sent down stream.");
		start
			if(strncmp(sctlptr->buf,rctlptr->buf,ctlsize))
				error("Incorrect control msg. retrieved.");
		finis
	}

	if(rdataptr)
	{
		testing("Check that the data message retrieved from the");
		testing("stream head was that one sent down stream.");
		start
			if(strncmp(sdataptr->buf,rdataptr->buf,datasize))
				error("Incorrect control msg. retrieved.");
		finis

		testing("Check that the length of the data message");
		testing("retrieved from the stream head equals the length");
		testing("of the message sent down stream.");
		start
			if(rdataptr->len != datasize)
				error("Incorrect length retrieved.");
		finis
	}

	return(0);
}
void
sigcatch(sig)
int	sig;
{
	gotsig = sig;
}
