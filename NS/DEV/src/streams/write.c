#ident "@(#)write.c	1.4"
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


int timeout();                          /* catches SIGALRM on timeout */
int     tflg;                           /* flag to test blocking */
int fd, mux, mux_id;

main()
{
        char    wbuf[MAXBUFSIZE];                  /* write buffer */
        char    rbuf[MAXBUFSIZE];                  /* read buffer */
        struct  strioctl        istr;

        setup("streams");

         abstract("Open the loop back driver %s for read/write.", DRIVER);
         start
                fd = open( DRIVER, O_RDWR);
        finis

        abstract("Verify that write will fail and set errno to EBADF if ");
        abstract("invalid file descriptor is passed as an argument.");
        start
                prep("Close the stream.");
                start
                        close( fd );
                finis

                testing("Call write. Expecting failure with errno set to");
                testing("EBADF.");
                start
                        expecting( EBADF );
                        write(fd, wbuf, 0);
                finis

                cleanup("Open the stream for read/write.");
                start
                        fd = open(DRIVER, O_RDWR);
                finis
        finis

         abstract("Verify that if the stream is linked to a multiplexor, ");
         abstract("write will fail and set errno to EINVAL.");
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

                testing("Call write. Expecting failure with errno set to");
                testing("EINVAL.");
                start
                        expecting (EINVAL );
                        write(fd,wbuf,SMSG);
                finis
                
                cleanup("Close the multiplexor.");
                start
                        close( mux );
                finis

		cleanup("Close the loop back driver and open for read/write.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR);
		finis
        finis


        abstract("Verify that if O_NDELAY is set and the write queue is");
        abstract("full, write will fail and set errno to EAGAIN.");
        start
                prep("Close the stream and open it for read/write with");
                prep("O_NDELAY set.");
                start
                        close( fd );
                        fd = open(DRIVER, O_RDWR|O_NDELAY);
                finis

                prep("Write a message that is 6 times the size of the min. ");
                prep("packet size. This should fill the write queue.");
                start
                        expecting( SMSG*6 );
                        write(fd, wbuf, SMSG * 6);
                finis
                
                testing("Call write with nbytes set to one. Expecting failure");
                testing("with errno set to EAGAIN.");
                start
                        expecting( EAGAIN );
                        write(fd, wbuf, 1);
                finis
        finis

        abstract("Verify that if O_NDELAY is set and the write queue becomes");
        abstract("full while writing a buffer, verify that write will return");
	abstract("the number of bytes actually read.");
        start
                prep("Close the stream and open it for read/write with");
                prep("O_NDELAY set.");
                start
                        close( fd );
                        fd = open(DRIVER, O_RDWR|O_NDELAY);
                finis

                prep("Write a message that is 5 times the size of the min. ");
                prep("packet size. This should leave room in the write queue");
		prep("for one more message of min. packet size.");
                start
                        expecting( SMSG*5 );
                        write(fd, wbuf, SMSG * 5);
                finis
                
                testing("Call write with nbytes set to 2 times the min packet");
		testing("packet size. Expecting it to return min packet size");
		testing("bytes were written.");
                start
                        expecting( SMSG );
                        write(fd, wbuf, SMSG*2);
                finis
        finis

        abstract("Verify that if O_NDELAY is not set and the write queue is");
        abstract("full, write will block until it times out.");
        start
		prep("Close the loop back driver and open for read/write.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR);
		finis

                prep("Write a message that is 6 times the size of the min. ");
                prep("packet size. This should fill the write queue.");
                start
                        expecting( SMSG*6 );
                        write(fd, wbuf, SMSG * 6);
                finis
                
                prep("Set up a signal handling routine.");
                start
                        signal(SIGALRM, timeout);
                finis
                tflg = 0;

                prep("Call alarm with argument of 5.");
                start
			expecting( VOID );
                        alarm((unsigned)5);
                finis

                testing("Call write with nbytes set to one. Expecting failure");
		testing("with errno set to EINTR.");
                start
			expecting( EINTR );
                        write(fd, wbuf, 1);
                finis

                prep("Call alarm with argument of 0.");
                start
			expecting( VOID );
                        alarm(0);
                finis

                testing("Check that control was transferred to the signal ");
                testing("handling routine.");
                start
                        if (tflg == 0)
                                error("Write failed to block with full queue.");
                finis

        finis

        abstract("Verify that if min. packet size is zero and nbytes does");
        abstract("not fall within the packet size range, the buffer will ");
        abstract("be broken into max. packet size segments then sent down ");
        abstract("stream.");
        start
		prep("Close the loop back driver and open for read/write.");
		start
			close( fd );
			fd = open(DRIVER, O_RDWR);
		finis

                testing("Call write with nbytes set to 3 times the min. ");
                testing("packet size.");
                start
                        expecting( SMSG*3 );
                        write(fd, wbuf, SMSG*3);
                finis
                
                prep("Call read to read in the message sent down stream.");
                start
                        expecting( SMSG*3);
                        read(fd, rbuf, SMSG*3);
                finis

                checkwrite(SMSG*3, rbuf, wbuf);

        finis

        abstract("Verify that if min. packet size is non zero and nbytes");
        abstract("does not fall within the packet size range, write will ");
        abstract("fail and set errno to ERANGE.");
        start
                prep("Call ioctl to set the minimum packet size to a value");
                prep("other than zero.");
                start
                        istr.ic_cmd = I_SETRANGE;
                        istr.ic_len = istr.ic_timout = 0;
                        expecting( VOID );
                        ioctl(fd,I_STR,&istr);
                finis

                testing("Call write with nbytes set to the max. packet size");
                testing("plus one. Expecting failure with errno set to ");
                testing("ERANGE.");
                start
                        expecting( ERANGE );
                        write(fd,wbuf,MAXBUFSIZE+1);
                finis

                testing("Call write with nbytes set to the min. packet size");
                testing("minus one. Expecting failure with errno set to ");
                testing("ERANGE.");
                start
                        expecting( ERANGE );
                        write(fd,wbuf,MINSNDSIZE-1);
                finis

                cleanup("Call ioctl to un-set the range.");
                start
                        istr.ic_cmd = I_UNSETRANGE;
                        istr.ic_len = istr.ic_timout = 0;
                        expecting( VOID );
                        ioctl(fd,I_STR,&istr);
                finis
        finis
	clean();
        done();
		/* NOTREACHED */
}
timeout ()
{

        tflg++;
}
checkwrite (erval, rdata, wdata)
int erval;                              /* expected return value */
char *rdata;                            /* data that was read */
char *wdata;                            /* data that was written */
{
        testing("verify that the data read in is same as the data written.");
        start
                expecting( VOID );
                if (strncmp(rdata, wdata, erval))
                        error("Incorrect data retrieved from stream head.");
        finis
}
clean()
{
	cleanup("Close the loopback driver.");
	start
		close( fd );
	finis
}
