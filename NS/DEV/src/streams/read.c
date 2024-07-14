#ident "@(#)read.c	1.5"
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

#define SMALL 10                        /* size of a small read/write */

int timeout();                          /* catches SIGALRM on timeout */
int     tflg;                           /* flag to test blocking */

main()
{
        char    wbuf[BUFSIZE];                  /* write buffer */
        char    rbuf[BUFSIZE];                  /* read buffer */
        int i;
        int fd, mux, mux_id;
        int rval;                       /* read return value */
        char    sctlmdata[BUFSIZ];
        struct  strbuf  sctlbuf, sdatabuf;

        setup("streams");

         abstract("Open the loop back driver %s for read/write.", DRIVER);
         start
                fd = open( DRIVER, O_RDWR);
        finis

         abstract("Verify the default read mode is byte stream.");
         start
                prep("Call write to send a %d byte message down stream.",SMALL);
                start
                        expecting( SMALL );
                        write(fd,wbuf,SMALL);
                finis
                
                prep("Call write to send another message down stream.");
                start
                        expecting( SMALL );
                        write(fd,&wbuf[SMALL],SMALL);
                finis

                testing("Call read to read %d bytes from the stream.",SMALL);
                start
                        expecting( SMALL*2 );
                        rval = read(fd, rbuf, SMALL*2);
                finis
                checkread(SMALL*2, rbuf, wbuf);
        finis

        abstract("Verify that in message-non-discard mode only nbytes are");
        abstract("retrieved and the remaining are left on stream. Send a");
        abstract("message down stream. Call read with the length set to");
        abstract("1/2 the size of the message sent. Then call read again");
        abstract("with the length set to 1/2 of the message. The second ");
        abstract("message returned should be the second half of the");
        abstract("original message sent.");
        start
                prep("Call ioctl with command set to I_SRDOPT and argument");
                prep("set to RMSGN.");
                start
                        expecting( 0 );
                        ioctl(fd, I_SRDOPT, RMSGN);
                finis
                prep("Call write to send the message of length %d.", SMALL);
                start
                        write(fd,wbuf,SMALL);
                finis

                testing("Call read with length of %d.",SMALL/2);
                start
                        expecting( SMALL/2 );
                        rval = read(fd, rbuf, SMALL/2);
                finis

                checkread(SMALL/2, rbuf, wbuf);

                testing("Call read with length of %d.",SMALL/2);
                start
                        expecting( SMALL/2 );
                        read(fd, rbuf, SMALL/2);
                finis

                checkread(SMALL/2, rbuf, &wbuf[SMALL/2]);
        finis

        abstract("Verify that in message-non-discard mode a message is read");
        abstract("until the message boundaries are met.  Send two messages ");
        abstract("down stream. Call read with the length set to double the");
	abstract("size of the first message sent. Verify message retrieved");
        abstract("was the first message sent down stream. ");
        start
                prep("Call write to send a message of length %d.", SMALL);
                start
                        expecting( SMALL );
                        write(fd,wbuf,SMALL);
                finis
                
                prep("Call write again to send another message.");
                start
                        write(fd,&wbuf[SMALL], SMALL);
                finis

                testing("Call read with length of %d. Should actually",SMALL*2);
                testing("get %d bytes of data returned.", SMALL);
                start
                        expecting( SMALL );
                        read(fd, rbuf, SMALL*2);
                finis

                checkread(SMALL, rbuf, wbuf);

                testing("Call read to read the second message.");
                start
                        read(fd, rbuf, SMALL);
                finis
        finis

        abstract("Verify that in message-discard mode only nbytes are");
        abstract("retrieved and the remaining are thrown away. Send two");
        abstract("messages down stream. Call read with the length set to");
        abstract("1/2 the size of the first message sent. Then call read ");
        abstract("again with the length set to 1/2 of the first message.");
        abstract("The second message returned should be the first half of the");
        abstract("second message sent.");
        start
                prep("Call ioctl with command set to I_SRDOPT and argument");
                prep("set to RMSGD.");
                start
                        expecting( 0 );
                        ioctl(fd, I_SRDOPT, RMSGD);
                finis
                
                prep("Call write to send a %d byte message down stream.",SMALL);
                start
                        expecting( SMALL );
                        write(fd,wbuf,SMALL);
                finis
                
                prep("Call write to send another message down stream.");
                start
                        expecting( SMALL );
                        write(fd,&wbuf[SMALL],SMALL);
                finis
                
                testing("Call read with length set to %d.", SMALL/2);
                start
                        expecting( SMALL/2 );
                        read(fd, rbuf, SMALL/2);
                finis
                
                checkread(SMALL/2, rbuf, wbuf);

                testing("Call read again with length set to %d.", SMALL/2);
                start
                        expecting( SMALL/2 );
                        read(fd, rbuf, SMALL/2);
                finis
                
                checkread(SMALL/2, rbuf, &wbuf[SMALL]);
        finis

        abstract("Verify that in message-discard mode a message is read ");
        abstract("until the message boundaries are met.  Send two messages ");
        abstract("down stream. Call read with the length set to the size");
        abstract("of the first message sent. Verify the message retrieved");
        abstract("was the one sent down stream.");
        start
                prep("Call write to send a message of length %d.", SMALL);
                start
                        expecting( SMALL );
                        write(fd,wbuf,SMALL);
                finis
                
                prep("Call write again to send another message.");
                start
                        write(fd,&wbuf[SMALL], SMALL);
                finis

                testing("Call read with length of %d. Should actually",SMALL/2);
                testing("get %d bytes of data returned.", SMALL);
                start
                        expecting( SMALL );
                        read(fd, rbuf, SMALL*2);
                finis

                checkread(SMALL, rbuf, wbuf);

                testing("Call read to read the second message.");
                start
                        read(fd, rbuf, SMALL);
                finis
        finis

         abstract("Verify that if O_NDELAY is not set and a read is attempted");
         abstract("on a stream that has no data currently available, read");
         abstract("will block.");
         start
                prep("Close the stream and open it for read/write. ");
                start
                        close( fd );
                        fd = open(DRIVER,O_RDWR);
                finis

                 prep("Set an alarm and see if the signal catching timeout");
                 prep("routine sets the timeout flag. Call signal with");
                 prep("SIGALRM and call alarm with argument of 5.");
                 start
                        signal(SIGALRM, timeout); 
                        tflg = 0;
			expecting( VOID );
                        alarm((unsigned)5);
                finis

                testing("Call read. Expecting it to fail with errno set to");
		testing("EINTR, due to the alarm");
                start
                        expecting( EINTR );
                        read(fd, rbuf, SMALL);
                finis

                prep("Call alarm with argument of 0.");
                start
			expecting( VOID );
                        alarm(0);
                finis

                testing("If the lapsed time is zero, print error.");
                start
                        if (tflg == 0)
                                error("Read failed to block.");
                finis
        finis

        abstract("Verify that if the stream is in message discard mode and");
        abstract("a message of zero length is on the stream head, read will");
        abstract("return a value of zero and the message is removed from the");
        abstract("stream.");
        start
                prep("Call ioctl with command set to I_SRDOPT, and arg set");
                prep("to RMSGD.");
                start
                        expecting( 0 );
                        ioctl(fd, I_SRDOPT, RMSGD);
                finis

                prep("Call write to write a zero byte message.");
                start
                        expecting( 0 );
                        write(fd, wbuf, 0);
                finis

                prep("Call write to write a %d byte message.", SMALL);
                start
                        expecting( SMALL );
                        write(fd, wbuf, SMALL);
                finis

                testing("Call read to retrieve the zero byte message.");
                start
                        expecting( 0 );
                        read(fd, rbuf, SMALL);
                finis

                testing("Verify that read throws away the zero length ");
                testing("message by writing another message in its place.");
                start
                        expecting( SMALL );
                        read(fd, rbuf, SMALL);
                finis

                checkread(SMALL, rbuf, wbuf);
        finis

        abstract("Verify that if the stream is in message nondiscard mode and");
        abstract("a message of zero length is on the stream head, read will");
        abstract("return a value of zero and the message is removed from the");
        abstract("stream.");
        start

                prep("Call ioctl with command set to I_SRDOPT, and arg set");
                prep("to RMSGN.");
                start
                        expecting( 0 );
                        ioctl(fd, I_SRDOPT, RMSGN);
                finis

                prep("Call write to write a zero byte message.");
                start
                        expecting( 0 );
                        write(fd, wbuf, 0);
                finis

                prep("Call write to write a %d byte message.", SMALL);
                start
                        expecting( SMALL );
                        write(fd, wbuf, SMALL);
                finis

                testing("Call read to retrieve the zero byte message.");
                start
                        expecting( 0 );
                        read(fd, rbuf, SMALL);
                finis

                testing("Verify that read throws away the zero length ");
                testing("message by writing another message in its place.");
                start
                        expecting( SMALL );
                        read(fd, rbuf, SMALL);
                finis

                checkread(SMALL, rbuf, wbuf);
        finis

        abstract("Verify that if the stream is in byte stream mode and");
        abstract("a message of zero length is on the stream head, read will");
        abstract("return a value of zero and the message is removed from the");
        abstract("stream.");
        start

                prep("Call ioctl with command set to I_SRDOPT, and arg set");
                prep("to RNORM.");
                start
                        expecting( 0 );
                        ioctl(fd, I_SRDOPT, RNORM);
                finis

                prep("Call write to write a zero byte message.");
                start
                        expecting( 0 );
                        write(fd, wbuf, 0);
                finis

                prep("Call write to write a %d byte message.", SMALL);
                start
                        expecting( SMALL );
                        write(fd, wbuf, SMALL);
                finis

                testing("Call read to retrieve the zero byte message.");
                start
                        expecting( 0 );
                        read(fd, rbuf, SMALL);
                finis

                cleanup("Call read to retrieve the %d byte message.", SMALL);
                start
                        expecting( SMALL );
                        read(fd, rbuf, SMALL);
                finis

                checkread(SMALL, rbuf, wbuf);
        finis

        abstract("Verify that read will fail and set errno to EBADF if invalid");
        abstract("file descriptor is passed as an argument.");
        start
                prep("Close the stream.");
                start
                        close( fd );
                finis

                testing("call read. Expecting failure with errno set to");
                testing("EBADF.");
                start
                        expecting( EBADF );
                        read(fd, rbuf, 0);
                finis

                cleanup("Open the stream for read/write.");
                start
                        fd = open(DRIVER, O_RDWR);
                finis
        finis

         abstract("Verify that if O_NDELAY is set and a read is attempted on");
         abstract("a stream that has no data currently available, read will");
         abstract("fail and set errno to EAGAIN.");
         start
                prep("Close the stream and open it for read/write with");
                prep("O_NDELAY set.");
                start
                        close( fd );
                        fd = open(DRIVER,O_RDWR|O_NDELAY);
                finis

                testing("Call read. Expecting failure with errno set to");
                testing("EAGAIN.");
                start
                        expecting( EAGAIN );
                        read(fd, rbuf, SMALL);
                finis

                cleanup("Open the stream for read/write.");
                start
                        fd = open(DRIVER, O_RDWR);
                finis
        finis

         abstract("Verify that if the stream is linked to a multiplexor, read");
         abstract("will fail and set errno to EINVAL.");
         start
                prep("Open the multiplexor driver for read/write.");
                start
                        mux = open(MUX,O_RDWR);
                finis

                prep("Call ioctl with command set to I_LINK.");
                start
                        expecting( VOID );
                        mux_id = ioctl(mux, I_LINK, fd);
                finis

                testing("Call read. Expecting failure with errno set to");
                testing("EINVAL.");
                start
                        expecting (EINVAL );
                        read(fd,rbuf,SMALL);
                finis
                
                cleanup("Close the multiplexor.");
                start
                        close( mux );
                finis

		cleanup("Close the loop back driver.");
		start
			close( fd );
		finis

		cleanup("Open the loop back driver for read/write.");
		start
			fd = open(DRIVER, O_RDWR);
		finis
        finis


        abstract("Verify that read will fail and set errno to EBADMSG if");
        abstract("the message waiting to be read is not a data message.");
        start
                filldata(SND,&sctlbuf,&sdatabuf,
                        BUFSIZE,0,sctlmdata,0);

                prep("Call putmsg to send a control message down stream.");
                start
                        putmsg(fd,&sctlbuf,0,0);
                finis

                testing("Call read. Expecting failure with errno set to ");
                testing("EBADMSG.");
                start
                        expecting( EBADMSG );
                        read(fd,rbuf, SMALL); 
                finis
        finis

        cleanup("Close the loop back driver.");
        start
                close( fd );
        finis
        done();
		/* NOTREACHED */
}

checkread (erval, rdata, wdata)
int erval;                              /* expected length of data read. */
char *rdata;                            /* data that was read */
char *wdata;                            /* data that was written */
{
        testing("Check that the data read from the stream head was that");
        testing("which was sent down stream.");
        start
		expecting( VOID );
                if (strncmp(rdata, wdata, erval))
                        error("Incorrect message read from the stream head.");
        finis
}
timeout()
{
        tflg++;
}
