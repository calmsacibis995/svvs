#ident "@(#)close.c	1.5"

#include <stdio.h>      
#include <fcntl.h>
#include <sv_base.h>
#include <sv_nse.h>
#include <svvs.h>       
#include <sv_env.h>
#include <sv_macro.h>
#include <sys/stropts.h>
#include <sv_streams.h>
#include <string.h>
#include <values.h>

main() {
	register	int	i, fd;
	char	wbuf[BUFSIZ];
	char	rbuf[BUFSIZ];
	long	etime, stime;

	setup("streams");

	abstract("Verify close does close a stream.");
	start
		prep("Open the loop back driver %s.", DRIVER);
		start
			fd = open(DRIVER,O_RDWR);
		finis

		prep("Write a message onto the stream.");
		start
			expecting(sizeof wbuf);
			write(fd,wbuf,sizeof wbuf);
		finis

		prep("Read a message from the stream.");
		start
			expecting(sizeof wbuf);
			read(fd,wbuf,sizeof wbuf);
		finis

		testing("Call close to close the stream.");
		start
			close(fd);
		finis

		testing("Call write again. This time it should fail with");
		testing("errno set to EBADF.");
		start
			expecting( EBADF );
			write(fd,wbuf,sizeof wbuf);
		finis

		testing("Call read again. This time it should fail with");
		testing("errno set to EBADF.");
		start
			expecting( EBADF );
			read(fd,wbuf,sizeof wbuf);
		finis
	finis

	abstract("Verify that close will fail and set errno to EBADF if the");
	abstract("argument is an invalid file descriptor.");
	start
		testing("Call close with the file descriptor for the closed");
		testing("stream. Expecting failure with errno set to EBADF.");
		start
			expecting( EBADF );
			close( fd );
		finis
	finis

	abstract("Verify that close waits when O_NDELAY is not set.");
	abstract("Fill the queue, close the stream, and time the length");
	abstract("of the close. Should take less than 15 seconds.");
	start
		prep("Open the loop back driver %s.", DRIVER);
		start
			fd = open(DRIVER,O_RDWR);
		finis

		prep("Write a message onto the stream.");
		start
			expecting( VOID );
			write(fd, wbuf, BUFSIZ);
		finis

		testing("Call time to get the initial time, call close");
		testing("to close the stream, and call time again to");
		testing("get the ending time.");
		start
			stime = time((long *)0);
			close(fd);
			etime = time((long *)0);
		finis

		testing("Check that the elapsed time is less than 15");
		testing("seconds, allowing 2 extra seconds in case");
		testing("the time stamp calls were made very close to");
		testing("the time a clock tick occurred.");
		start
			if ((etime - stime) > (15 + 2) )
			error("Elapsed time is %d.", etime-stime);
		finis
	finis


	abstract("Verify that close doesn't wait when O_NDELAY is set.");
	abstract("Fill the queue, close the stream, and time the length");
	abstract("of the close. ");
	start
		prep("Open the loop back driver %s.", DRIVER);
		start
			fd = open(DRIVER,O_RDWR|O_NDELAY);
		finis

		prep("Write a message onto the stream.");
		start
			expecting( VOID );
			write(fd, wbuf, BUFSIZ);
		finis

		prep("Get the current time.");
		start
			stime = time((long *)0);
		finis

		testing("Call close to close the stream.");
		start
			close(fd);
			etime = time((long *)0);
		finis

		testing("Check that the elapsed time is less than 2.");
		start
			if ((etime - stime) > 2)
			error("Elapsed time is %d.", etime-stime);
		finis
	finis
	done();
	/* NOTREACHED */
}
