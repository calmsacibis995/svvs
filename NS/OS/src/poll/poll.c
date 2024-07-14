#ident "@(#)poll.c	1.8"

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
#include <poll.h>
#include <values.h>

#define	rendevous()	(v_sema(), psema())

void sigcatch();

int	fd, mux, mux_id;
struct	pollfd	fds[_NFILE];
main()
{
	int	retn;
	struct	strioctl	istr;
	char	errval;
	struct	strbuf	sctlbuf,rctlbuf;
	struct	strbuf	sdatabuf,rdatabuf;
	char	rctlmdata[BUFSIZE],rdatamdata[BUFSIZE];
	char	sctlmdata[BUFSIZE],sdatamdata[BUFSIZE];
	int	flags,pid;
	long	tvec,aftvec;
	register int	i,nfds;

	setup("poll");
	funct("poll");

	abstract("Open two drivers and initialize the semaphore mechanism.");
	start
	finis

	abstract("Verify that the POLLIN event option to poll properly");
	abstract("detects a zero-length message on the getmsg queue.");
	start
		break_on( ERROR );
		prep("Open the loop back driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		continue_on( ERROR );
		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

		sctlbuf.len = 0;

		prep("Call putmsg with a control message only and the length");
		prep("of the message set to 0.");
		start
			putmsg(fd,&sctlbuf,0,0);
		finis

		prep("Set the nfds field to 1; the fds[0].fd field to the");
		prep("file descriptor of the loop back driver and fds.events");
		prep("to POLLIN.");
		start
			nfds = 1;
			fds[0].fd = fd;
			fds[0].events = POLLIN;
		finis

		testing("Call poll. Expecting a return value of one.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s", pollerr);
		finis

		testing("Verify the events field is still set to POLLIN.");
		start
			if((fds[0].events&POLLIN) != POLLIN)
				error("%s", eventerr);
		finis
		cleanup("Close the loopback driver.");
		start
			close( fd );
		finis
	finis

	abstract("Verify that the POLLERR option of poll properly detects");
	abstract("an error message on the stream head.");
	start
		break_on( ERROR );

		prep("Set up signal handling routines for SIGALRM.");
		start
			signal(SIGALRM,sigcatch);
		finis

		prep("Call setsig to set SIGCLD");
		start
			sigset(SIGCLD,SIG_IGN);
		finis

		prep("Open the loop back driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		continue_on( ERROR );

		prep("Call ioctl to set a error condition in the");
		prep("driver. This will generate a error at the ");
		prep("stream head.");
		start
			expecting( VOID );
			istr.ic_cmd = I_SETERR;
			istr.ic_len = sizeof(char);
			istr.ic_timout = 0;
			errval = (char)ENXIO;
			istr.ic_dp = &errval;
			ioctl(fd,I_STR,&istr);
		finis

		expecting( VOID );
		alarm(15);

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;
		testing("Call poll with timeout set to -1.");
		start
			if((poll(fds,nfds,-1)) != 1)
				error("%s", pollerr);
		finis

		testing("Verify the revents field was set to POLLERR.");
		start
			if(fds[0].revents != POLLERR)
				error("%s", eventerr);
		finis

		expecting( VOID );
		alarm(0);

		cleanup("Close the loopback driver.");
		start
			expecting( SUCCESS );
			close( fd );
		finis

	finis

	abstract("Verify that poll properly detects a hangup message on the");
	abstract("stream head.");
	start
		break_on( ERROR );

		prep("Set up a signal handling routine for SIGALRM.");
		start
			 signal(SIGALRM,sigcatch);
		finis

		prep("Call setsig for SIGCLD.");
		start
			sigset(SIGCLD,SIG_IGN);
		finis

		prep("Open the loop back driver for read/write");
		prep("with O_NDELAY set.");
		start
			fd = open(DRIVER, O_RDWR|O_NDELAY);
		finis
		
		continue_on( ERROR );

		prep("Call ioctl to generate a hangup indication on");
		prep("the stream head.");
		start
			expecting( VOID );
			istr.ic_cmd = I_SETHANG;
			istr.ic_len = istr.ic_timout = 0;
			if((retn = ioctl(fd,I_STR,&istr)) == -1)
			error("bad ioctl in child");
		finis

		expecting( VOID );
		alarm(15);

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with timeout set to -1.");
		start
			if((retn = poll(fds,nfds,-1)) != 1)
				error("%s", pollerr);
		finis

		testing("Verify the revents field was set to POLLHUP.");
		start
		if(fds[0].revents != POLLHUP)
			error("%s", eventerr);
		finis
		cleanup("Close the loopback driver.");
		start
			close( fd );
		finis
	finis

	abstract("Verify that POLLOUT and POLLHUP are mutually exclusive.");
	start
		break_on( ERROR );

		prep("Open the loop back driver for read/write");
		prep("with O_NDELAY set.");
		start
			fd = open(DRIVER, O_RDWR|O_NDELAY);
		finis

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLOUT;

		testing("Call poll with timeout set to 0 and events set to");
		testing("POLLOUT.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s",pollerr);
		finis

		testing("Check that the revents field is set to POLLOUT.");
		start
			if(fds[0].revents != POLLOUT)
				error("%s", eventerr);
		finis

		prep("Call ioctl to generate a hangup request at the stream");
		prep("head.");
		start
			expecting( VOID );
			istr.ic_cmd = I_SETHANG;
			istr.ic_len = istr.ic_timout = 0;
			ioctl(fd,I_STR,&istr);
		finis

		prep("Call setsig for SIGALRM.");
		start
			sigset(SIGALRM,sigcatch);
		finis

		expecting( VOID );
		alarm(15);

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLOUT;

		testing("Call poll with events set to POLLOUT and timeout");
		testing("set to -1");
		start
			if((retn = poll(fds,nfds,-1)) != 1)
				error("%s", pollerr);
		finis

		expecting( VOID );
		alarm(0);

		testing("Check that the revents field is set to POLLHUP.");
		start
			if(fds[0].revents != POLLHUP)
				error("%s", eventerr);
		finis

		cleanup("Close the driver.");
		start
			close(fd);
		finis
	finis

	abstract("Verify that POLLHUP and the POLLIN and POLLPRI options ");
	abstract("are not mutually exclusive.");
	start
		break_on( ERROR );

		prep("Open the loopback driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);
		
		prep("Call putmsg.");
		start
			putmsg(fd,0,&sdatabuf,0);
		finis

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with events set to POLLIN and timeout");
		testing("set to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s", pollerr);
		finis

		testing("Check that the revents field is set to POLLIN.");
		start
			if(fds[0].revents != POLLIN)
				error("Revents was set to %d and not POLLIN.",
				fds[0].revents);
		finis

		prep("Call ioctl to issue a hangup request on the stream head.");
		start
			expecting( VOID );
			istr.ic_cmd = I_SETHANG;
			istr.ic_len = istr.ic_timout = 0;
			ioctl(fd,I_STR,&istr);
		finis

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with events set to POLLIN and timeout");
		testing("set to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s", pollerr);
		finis

		testing("Check that revents != POLLIN|POLLHUP");
		start
			if(fds[0].revents != (POLLIN|POLLHUP))
				error("%s", eventerr);
		finis

		prep("Close the loop back driver and open for read/write.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR);
		finis

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

		prep("Call putmsg with flags set to RS_HIPRI.");
		start
			putmsg(fd,&sctlbuf,0,RS_HIPRI);
		finis

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLPRI;

		testing("Call poll with events set to POLLPRI and timeout");
		testing("set to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s", pollerr);
		finis

		testing("Check that revents is equal to POLLPRI.");
		start
			if(fds[0].revents != POLLPRI)
				error("%s", eventerr);
		finis

		prep("Call ioctl to set generate a hangup signal at the");
		prep("stream head.");
		start
			expecting( VOID );
			istr.ic_cmd = I_SETHANG;
			istr.ic_len = istr.ic_timout = 0;
			ioctl(fd,I_STR,&istr);
		finis

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLPRI;

		testing("Call poll with events set to POLLPRI and timeout");
		testing("set to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s", pollerr);
		finis

		testing("Check that revents is set to POLLPRI|POLLHUP.");
		start
			if(fds[0].revents != (POLLPRI|POLLHUP))
				error("%s", eventerr);
		finis
		cleanup("Close the loopback driver.");
		start
			close( fd );
		finis

	finis


	abstract("Verify that the POLLNVAL option properly recognizes that ");
	abstract("the passed stream file descriptor does not belong to a ");
	abstract("valid stream.");
	start
		break_on( ERROR );
		prep("Open and close the loop back driver to obtain an invalid");
		prep("file descriptor.");
		start
			fd = open(DRIVER, O_RDWR);
			close( fd );
		finis

		nfds = 1;
		fds[0].fd = 0;
		fds[0].events = POLLOUT;

		testing("Call poll with the fd field set to the file");
		testing("descriptor of the closed driver and timeout set ");
		testing("to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s",pollerr);
		finis

		testing("Check that the revents was set to POLLINVAL.");
		start
			if((fds[0].revents&POLLNVAL) != POLLNVAL)
				error("%s", eventerr);
		finis

	finis


	abstract("Verify that poll will fail with errno set to EINTR if a");
	abstract("signal is received while in poll.");
	start
		break_on( ERROR );

		prep("Set up the signal handling routine.");
		start
			signal(SIGALRM,sigcatch);
		finis

		prep("Open the loop back drive for read and write");
		prep("with the O_NDELAY flag set.");
		start
			fd = open(DRIVER, O_RDWR|O_NDELAY);
		finis

		continue_on( ERROR );

		prep("Call alarm with an argument of 15.");
		start
			expecting( VOID );
			alarm(15);
		finis

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with events set to POLLIN and timeout");
		testing("set to -1. Expecting failure with errno set to");
		testing("EINTR.");
		start
			expecting( EINTR );
			poll(fds,nfds,-1);
		finis

		cleanup("Close the driver.");
		start
			close(fd);
		finis
	finis


	abstract("Verify that poll will fail and set errno to EFAULT  if a");
	abstract("bad address is specified for the fds array.");
	start
		break_on( ERROR );

		prep("Open the loopback driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		continue_on( ERROR );

		nfds=1;
		testing("Call poll with the fds argument set to -1, nfds");
		testing("set to 1 and timeout set to 0. Expecting failure");
		testing("with errno set to EFAULT.");
		start
			expecting( EFAULT );
			poll( -1, nfds,  0);
		finis

		cleanup("Close the driver.");
		start
			close( fd );
		finis
	finis

	abstract("Verify that poll will fail and set errno to EINVAL if an");
	abstract("invalid file descriptor count is specified.");
	start
		break_on( ERROR );
		prep("Open the loopback driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		continue_on( ERROR );

		nfds = -1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with events set to POLLIN, timeout set");
		testing("to 0 and nfds set to -1. Expecting failure with ");
		testing("errno set to EINVAL.");
		start
			expecting( EINVAL );
			poll(fds,nfds,0);
		finis

		nfds = 0;
		nfds = ~nfds;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with events set to POLLIN, timeout set");
		testing("to 0 and nfds set to all ones. Expecting failure ");
		testing("with errno set to EINVAL.");
		start
			expecting( EINVAL );
			poll(fds,nfds,0);
		finis

		nfds = _NFILE + 1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;

		testing("Call poll with events set to POLLIN, timeout set");
		testing("to 0 and nfds set to NOFILES plus one. Expecting ");
		testing("failure with errno set to EINVAL.");
		start
			expecting( EINVAL );
			poll(fds,nfds,0);
		finis

		cleanup("Close the driver.");
		start
			close( fd );
		finis

	finis


	abstract("Verify that the revents flag is set prior to the call");
	abstract("to poll, and reflects the proper events upon return from poll.");
	start
		break_on( ERROR );

		prep("Open the loopback driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		continue_on( ERROR );

		filldata(SND,&sctlbuf,&sdatabuf,
			BUFSIZE,BUFSIZE,sctlmdata,sdatamdata);

		nfds = 1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;
		fds[0].revents = 0;
		fds[0].revents = ~fds[0].revents;

		testing("Call poll with events set to POLLIN, revents set");
		testing("to all ones and timeout set to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 0)
				error("%s", pollerr);
		finis

		testing("Check that revents is equal to 0");
		start
			if((fds[0].revents) != 0)
				error("%s", eventerr);
		finis

		prep("Call putmsg.");
		start
			putmsg(fd,&sctlbuf,0,0);
		finis

		nfds = 1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;
		fds[0].revents = 0;
		fds[0].revents = ~fds[0].revents;

		testing("Call poll with events set to POLLIN, revents set");
		testing("to all ones and timeout set to 0.");
		start
			if((retn = poll(fds,nfds,0)) != 1)
				error("%s", pollerr);
		finis

		testing("Check that revents is set to POLLIN.");
		start
			if((fds[0].revents) != POLLIN)
				error("%s", eventerr);
		finis

		flags=0;
		filldata(RCV,&rctlbuf,&rdatabuf,
			BUFSIZE,BUFSIZE,rctlmdata,rdatamdata);

		prep("Call getmsg.");
		start
			getmsg(fd,&rctlbuf,&rdatabuf,&flags);
		finis

		cleanup("Close the driver.");
		start
			close( fd );
		finis
	finis

	abstract("Verify that poll properly handles invalid file descriptors.");
	start
		break_on( ERROR );

		prep("Open the loopback driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		continue_on( ERROR );

		nfds = 3;
		fds[0].fd = -1;
		fds[1].fd = _NFILE + 1;
		fds[2].fd = MAXSHORT;

		for(i=0; i < nfds; i++) {
			fds[i].events = POLLIN;
			fds[i].revents = 0;
			fds[i].revents = ~fds[i].revents;
		}

		testing("Call poll with nfds set to 3. The events field of ");
		testing("each file descriptor is set to POLLIN and the ");
		testing("revents field is set to all ones. The file ");
		testing("descriptors are set to -1, NOFILES + 1 and MAXSHORT,");
		testing("respectively. Check that the return value is 2.");
		start
			expecting( 2 );
			poll(fds,nfds,0);
		finis

		testing("Check the revents fields were set to 0, POLLNVAL");
		testing("and POLLNVAL, respectively.");
		start
			for(i=0; i < nfds; i++) {
				if(i == 0) {
					if(fds[i].revents != 0)
				   error("revents for fds[0] incorrectly set.");
				}
				else {
					if(fds[i].revents != POLLNVAL)
			       		 error("revents for fds[%d] incorrectly set.",i);
				}
			}
		finis

		cleanup("Close the driver.");
		start
			close( fd );
		finis
	finis

	abstract("Verify that poll properly waits timeout milliseconds for an");
	abstract("event to occur.");
	start
		break_on( ERROR );

		(int) signal(SIGALRM,sigcatch);

		prep("Open the loop back driver for read/write");
		prep("with O_NDELAY set.");
		start
			fd = open(DRIVER, O_RDWR|O_NDELAY); 
		finis

		continue_on( ERROR );

		expecting( VOID );
		alarm(60);

		nfds=1;
		fds[0].fd = fd;
		fds[0].events = POLLIN;
		time(&tvec);
/*
30 sec = 30000 msec
*/
		testing("Call poll with timeout set to 30000 (30 sec) and");
		testing("nfds set to 1.");
		start
			expecting( 0 );
			poll(fds,nfds,30000);
		finis

		testing("Check that the elapsed time is not less than 28");
		testing("sec and not greater than 32 seconds.");
		start
			time(&aftvec);
			aftvec -= tvec;

			if((aftvec < 28) || (aftvec > 32))
				error("Elapsed time is %d.", aftvec);
		finis

		cleanup("Close the loop back driver.");
		start
			close(fd);
		finis
	finis

	abstract("Verify that EINVAL is returned if a poll is attempted on");
	abstract("a stream linked below a multiplexor.");
	start
		break_on( ERROR );

		prep("Open the loop back driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis

		prep("Open the streams based multiplex driver \"%s\"", MUX);
		prep("for reading and writing.");
		start
			mux = open(MUX, O_RDWR);
		finis

		prep("Call ioctl to link the loop back driver to the");
		prep("multiplexor.");
		start
			expecting( VOID );
			mux_id = ioctl(mux,I_LINK,fd);
		finis

		fds[0].fd = fd;
		fds[0].events = POLLOUT;
		nfds=1;

		testing("Call poll with the file descriptor of the loop back");
		testing("driver. Expecting failure with errno set to EINVAL.");
		start
			expecting( 1 );
			poll(fds,nfds,0);
		finis

		testing("Check that the revents field is set to POLLNVAL.");
		start
			if(fds[0].revents != POLLNVAL)
				error("bad poll errno");
		finis

		cleanup("Call ioctl to unlink the loop back driver.");
		start
			expecting( VOID );
			ioctl(fd, I_UNLINK, mux_id);
		finis
		cleanup("Close the loop back driver.");
		start
			close( fd );
		finis

		cleanup("Close the multiplex driver.");
		start
			exit_on( ERROR );
			close( mux );
		finis
	finis
	done();
}
void
sigcatch(sig)
int	sig;
{
	gotsig = sig;
}

