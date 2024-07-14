#ident "@(#)putmsg.c	1.8"
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


int	fd;
void	alrmcth();
void	clean();
int	loop();
main()
{
	register int	testcnt;
	int	retn;
	int	mux_id;
	int	mux;
	struct	strioctl	istr;
	struct	strbuf	sctlbuf,rctlbuf;
	struct	strbuf	sdatabuf,rdatabuf;
	char	rctlmdata[MAXBUFSIZE+1],rdatamdata[MAXBUFSIZE+1];
	char	sctlmdata[MAXBUFSIZE+1],sdatamdata[MAXBUFSIZE+1];
	int	flags;

	no_setpgrp();
	setup("putmsg");
	funct("putmsg");

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

	finis

	abstract("Verify proper operation of putmsg with control messages ");
	abstract("only with flags set to zero.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with datafield and flags set to 0");
		start
			putmsg(fd,&sctlbuf,0,0);
		finis

		flags=0;
		prep("Call getmsg with flags set to 0 to be sure the");
		prep("control message was sent down stream.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,0,0);
	finis
		

	abstract("Verify proper operation of putmsg with data message only");
	abstract("with flags set to zero.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with control field and flags set to 0.");
		start
			putmsg(fd,0,&sdatabuf,0);
		finis

		flags=0;
		prep("Call getmsg to retrieve the data sent downstream.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(0,0,&sdatabuf,&rdatabuf);
	finis

	abstract("Verify putmsg with a control message only and flags set");
	abstract("to RS_HIPRI.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with flags set to RS_HIPRI and");
		testing("dataptr set to NULL.");
		start
			putmsg(fd,&sctlbuf,0,RS_HIPRI);
		finis

		flags=RS_HIPRI;
		testing("Call getmsg to retrieve that message sent downstream.");
		start
			getmsg(fd,&rctlbuf,0,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,0, 0);

		testing("Check that the flags returned from the getmsg is ");
		testing("RS_HIPRI.");
		start
			if(flags != RS_HIPRI)
				error("bad priority flag return");
		finis
	finis

	abstract("Verify putmsg will fail and set errno to EINVAL if it");
	abstract("is called with a data message only and flags set to ");
	abstract("RS_HIPRI.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with control field set to zero and the");
		testing("flags set to RS_HIPRI. Expecting failure with errno");
		testing("set to EINVAL.");
		start
			expecting( EINVAL );
			putmsg(fd,0,&sdatabuf,RS_HIPRI);
		finis
	finis

	abstract("Verify putmsg with a control and data message with");
	abstract("flags set to RS_HIPRI.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with flags set to RS_HIPRI. ");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,RS_HIPRI);
		finis

		flags=RS_HIPRI;
		testing("Call getmsg to retrieve that message sent downstream.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);

		testing("Check that the flags returned from the getmsg is ");
		testing("RS_HIPRI.");
		start
			if(flags != RS_HIPRI)
				error("bad priority flag return");
		finis
	finis

	abstract("Verify putmsg with a control and data message with");
	abstract("flags set to 0.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with flags set to 0. ");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		flags=0;
		testing("Call getmsg to retrieve that message sent downstream.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,&sdatabuf, &rdatabuf);

		testing("Check that the flags returned from the getmsg is ");
		testing("0.");
		start
			if(flags != 0)
				error("bad priority flag return");
		finis
	finis

	abstract("Verify putmsg with neither a control or data message and ");
	abstract("with the flags set to 0.");
	start
		testing("Call putmsg with flags set to 0. ");
		start
			putmsg(fd,0,0,0);
		finis
	finis

	abstract("Verify putmsg with neither a control or data message and ");
	abstract("with the flags set to RS_HIPRI.");
	start
		testing("Call putmsg with flags set to RS_HIPRI. Expecting");
		testing("failure with errno set to EINVAL.");
		start
			expecting( EINVAL );
			putmsg(fd,0,0,RS_HIPRI);
		finis
	finis

	abstract("Verify that if a hangup occurs on the stream being");
	abstract("sent data, putmsg will fail and set errno to ENXIO.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with flags set to 0.");
		start
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis
		prep("Call getmsg to retrieve the messages.");
		start
			flags=0;
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		checkdata(&sctlbuf,&rctlbuf,0,0);

		prep("Call ioctl to generate a hangup request.");
		start
			istr.ic_cmd = I_SETHANG;
			istr.ic_len = istr.ic_timout = 0;
			ioctl(fd,I_STR,&istr);
		finis

		prep("Call putmsg with flags set to 0. Expecting failure");
		prep("with errno set to ENXIO.");
		start
			expecting( ENXIO );
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		clean();
	finis


	abstract("Verify errno gets set to ERANGE when a request exceeds");
	abstract("the maximum buffer size.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			MAXBUFSIZE+1,0,sctlmdata,sdatamdata);

		prep("Call ioctl to modify the range in the loop back driver.");
		start
			istr.ic_cmd = I_SETRANGE;
			istr.ic_len = istr.ic_timout = 0;
			ioctl(fd,I_STR,&istr);
		finis

		testing("Call putmsg with the length field of ctlptr");
		testing("structure set to a range that exceeds the range ");
		testing("defined in the loop back driver. Expecting failure");
		testing("with errno set to ERANGE.");
		start
			flags = 0;
			expecting( ERANGE );
			putmsg(fd, &sctlbuf,&sdatabuf, flags);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,MAXBUFSIZE+1,sctlmdata,sdatamdata);

		testing("Call putmsg with the length field of dataptr");
		testing("structure set to a range that exceeds the range ");
		testing("defined in the loop back driver. Expecting failure");
		testing("with errno set to ERANGE.");
		start
			expecting( ERANGE );
			putmsg(fd, 0,&sdatabuf, flags);
		finis

		cleanup("Reset the range in the loop back driver.");
		start
			istr.ic_cmd = I_UNSETRANGE;
			istr.ic_len = istr.ic_timout = 0;

			ioctl(fd,I_STR,&istr);
		finis

		clean();
	finis


	abstract("Verify errno gets set to ERANGE when a request is less");
	abstract("than the minimum buffer size.");
	start
		filldata(SND,&sctlbuf,&sdatabuf,
			MINSNDSIZE-1,0,sctlmdata,sdatamdata);


		prep("Call ioctl to modify the minimum buffer size in the");
		prep("loop back driver.");
		start
			istr.ic_cmd = I_SETRANGE;
			istr.ic_len = istr.ic_timout = 0;

			ioctl(fd,I_STR,&istr);
		finis

		testing("Call putmsg with the length field of ctlptr");
		testing("structure set to a range that exceeds the range ");
		testing("defined in the loop back driver. Expecting failure");
		testing("with errno set to ERANGE.");
		start
			flags = 0;
			expecting( ERANGE );
			putmsg(fd, &sctlbuf,&sdatabuf, flags);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,MINSNDSIZE-1,sctlmdata,sdatamdata);

		testing("Call putmsg with the length field of dataptr");
		testing("structure set to a range that exceeds the range ");
		testing("defined in the loop back driver. Expecting failure");
		testing("with errno set to ERANGE.");
		start
			putmsg(fd, 0,&sdatabuf, flags);
		finis

		prep("Call ioctl to modify the minimum buffer size in the");
		prep("loop back driver.");
		start
			istr.ic_cmd = I_UNSETRANGE;
			istr.ic_len = istr.ic_timout = 0;

			ioctl(fd,I_STR,&istr);
		finis

		clean();
	finis


	abstract("Verify that putmsg will fail and set errno to EAGAIN if ");
	abstract("putmsg is called while O_NDELAY is set and the write queue");
	abstract("is full.");
	start
		prep("Close the loop back driver and open it for reading");
		prep("and writing wwith the O_NDELAY flag set.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR|O_NDELAY);
		finis

		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		flags=0;
		testing("Call putmsg with flags set to zero. Expecting");
		testing("failure with errno set to EAGAIN.");
		start
			loop(fd, &sctlbuf,&sdatabuf);
		finis

		clean();
	finis

	abstract("Verify that EINVAL is returned if a putmsg is attempted on");
	abstract("a stream linked below a multiplexor.");
	start
		prep("Call ioctl to link the loop back driver to the ");
		prep("multiplexor. The ioctl cmd is I_LINK.");
		start
			expecting( VOID );
			mux_id = ioctl(mux,I_LINK,fd);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

		testing("Call putmsg and attempt to access the loop back");
		testing("driver. Expecting failure with errno set to EINVAL.");
		start
			expecting( EINVAL );
			putmsg(fd,&sctlbuf,&sdatabuf,0);
		finis

		cleanup("Call ioctl to unlink the loop back driver.");
		start
			ioctl(fd, I_UNLINK, mux_id);
		finis

		cleanup("Close the multiplex driver.");
		start
			close( mux );
		finis

		clean();

		cleanup("Open the streams based multiplex driver \"%s\"", MUX);
		cleanup("for reading and writing.");
		start
			mux = open(MUX, O_RDWR);
		finis

	finis

	abstract("Verify that putmsg will fail and set errno to EFAULT if");
	abstract("a bad address is specified for the ctl and data pointers.");
	start
		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

			flags=0;
		testing("Call putmsg with the flags set to 0 and the address");
		testing("of the ctlptr set to -1. Expecting failure with ");
		testing("errno set to EFAULT.");
		start
			expecting( EFAULT );
			putmsg(fd, -1,&rdatabuf,  flags);
		finis

		testing("Call putmsg with the flags set to 0 and the address");
		testing("of the dataptr set to -1. Expecting failure with ");
		testing("errno set to EFAULT.");
		start
			expecting( EFAULT );
			putmsg(fd, &rctlbuf,-1,  flags);
		finis
	finis

	abstract("Verify that putmsg will fail and set errno to EINVAL if");
	abstract("the flags is set to a number other than 0 and RS_HIPRI.");
	start
		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		testing("Call putmsg with flag set to RS_HIPRI + 1.",INFLAG);
		testing("Expecting failure with errno set to EINVAL.");
		start
			expecting( EINVAL );
			putmsg(fd, &rctlbuf,&rdatabuf, RS_HIPRI+1);
		finis
	finis

	abstract("Verify the EBADF errno. Call putmsg with a valid file");
	abstract("descriptor for a closed file and a streams based file");
	abstract("opened for reading only.");
	start
		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Close the streams driver and open it for reading only.");
		start
			close( fd );
			fd = open(DRIVER, O_RDONLY);
		finis

		testing("Call putmsg with the file descriptor of the read ");
		testing("only file. Expecting failure with errno set to");
		testing("EBADF.");
		start
			expecting( EBADF );
			putmsg(fd, &rctlbuf,&rdatabuf, 0);
		finis

		prep("Close the file.");
		start
			close( fd );
		finis

		testing("Call putmsg with a file descriptor of a closed");
		testing("file. Expecting failure with errno set to EBADF.");
		start
			expecting( EBADF );
			putmsg(fd, &rctlbuf,&rdatabuf, flags);
		finis

		cleanup("Open the loop back driver for read and write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis
	finis

	abstract("Verify the ENOSTR errno. Call putmsg with a valid file");
	abstract("descriptor for a file other than a streams device.");
	start
		filldata(SND,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Open the non stream file \"/dev/tty\" for read/write.");
		start
			retn = open("/dev/tty", O_RDWR);
		finis

		testing("Call putmsg with the file descriptor of the opened ");
		testing("file. Expecting failure with errno set to ENOSTR.");
		start
			expecting( ENOSTR );
			putmsg(retn, &rctlbuf,&rdatabuf, 0);
		finis

		cleanup("Close the /dev/tty.");
		start
			close( retn );
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


void
alrmcth()
{
	return;
}
void
clean()
{
	cleanup("Close the loop back driver and open for read/write.");
	start
		close( fd );
		fd = open(DRIVER, O_RDWR);
	finis
}
loop(fd,rctlbuf,rdatabuf)
int	fd;
struct	strbuf	*rctlbuf, *rdatabuf;
{
	register int	retn,flags=0;

	while((retn = putmsg(fd,rctlbuf,rdatabuf,0)) != -1)
	{
		flags++;
	}
	if(errno != EAGAIN)
		error("Errno was set incorrectly.");
	return(retn);
}

